/*
 * File:   State_Machine_Main.c
 * Author: BB Slug Team
 * 
 * PID control based off of material found in Embedded Computing and Mechatronics with the PIC32
 * Microcontroller Kevin M. Lynch, Nicholas Marchuk, Matthew L. Elwin
 * 
 * Created on April 12, 2017, 5:12 PM
 */
/************************************************************************/
/*  File Description:                                                   */
/************************************************************************/
#include "BB_BOARD.h"
#include <plib.h>
#include <stdio.h>
#include "BB_Motor.h"
#include "BB_Encoder.h"
#include "BB_LEDS.h"
#include "BB_UART.h"
#include <math.h>
#include "BB_IMU.h"
#include "BB_CONTROLLER.h"
#include "BB_QUAT.h"

/* ------------------------------------------------------------ */
/*				Definitions			*/
/* ------------------------------------------------------------ */
#define cntMsDelay      5000
#define thsndDelay      1000

#define LINEAR_X_P 0.1
#define LINEAR_X_I 0
#define LINEAR_X_D 0

#define LINEAR_Y_P LINEAR_X_P
#define LINEAR_Y_I LINEAR_X_I
#define LINEAR_Y_D LINEAR_X_D

#define THETA_X_P 170
#define THETA_X_I 0
#define THETA_X_D 1

#define THETA_Y_P THETA_X_P
#define THETA_Y_I THETA_X_I
#define THETA_Y_D THETA_X_D

#define OMEGA_X_P 1
#define OMEGA_X_I 0
#define OMEGA_X_D 0

#define OMEGA_Y_P OMEGA_X_P
#define OMEGA_Y_I OMEGA_X_I
#define OMEGA_Y_D OMEGA_X_D

#define L1 -2.2
#define L2 -3.6
#define L3 39.6
#define L4 11.5

#define ACC_MIN_X (200) // TODO need to test to see what the dead band will be for each virtual wheel
#define ACC_MIN_Y (100) 
#define ACC_DEADZONE (20) // TODO need to test to see what amount of acc we don't want to act on

#define DEADBAND_IN 0
#define PWM_SCALE 1
#define OMEGA2PWM (63.66 * PWM_SCALE)

/* ------------------------------------------------------------ */
/*				Prototypes			*/
/* ------------------------------------------------------------ */
void PID_GetUARTK();

/* ------------------------------------------------------------ */
/*				Global Variables		*/
/* ------------------------------------------------------------ */
volatile float eCountRadians = 0;

volatile PIDControl linearX;
volatile PIDControl linearY;
volatile PIDControl thetaX;
volatile PIDControl thetaY;
volatile PIDControl omegaX;
volatile PIDControl omegaY;

volatile PIDControl *controller2changeX;
volatile PIDControl *controller2changeY;
volatile double *k2changeX;
volatile double *k2changeY;

volatile BOOL printFlag;
double xAngleOffset;
double yAngleOffset;
static int omega2pwm = OMEGA2PWM;

/* ------------------------------------------------------------ */
/*                            Interrupt                         */

/* ------------------------------------------------------------ */
void __ISR(_TIMER_4_VECTOR, IPL2SOFT) Timer4Handler(void)
{
    // clear the interrupt flag always
    mT4ClearIntFlag();

    static Quat q, result;
    static gyroAxis g;
    static encodeVal e;
    static double prevBallLocX = 0, prevBallLocY = 0;
    static int count = 0;

    // Get tip angles
    IMU_Read_Quat();
    IMU_Get_Quat(&q);
    BB_Quat_Tip_Vector(&q, &result);
    double thetaX = BB_Quat_Find_Tip_Angle_X(&result) - xAngleOffset; // in radians
    double thetaY = BB_Quat_Find_Tip_Angle_Y(&result) - yAngleOffset; // in radians

    // Get tip rates
    IMU_Read_Gyro();
    IMU_Get_Gyro(&g);
    double omegaX = g.y; // changed for readability
    double omegaY = g.x;

    // Get Ball location
    EncoderGetVirtualWheels(&e); // may want to accumulate here and reset the encoder values each call to prevent overflow on large counts
    double ballLocX = e.x;
    double ballLocY = e.y;

    // Get Ball speed
    // speed = change in location / change in time
    double ballVelX = (ballLocX - prevBallLocX) / SAMPLE_TIME;
    double ballVelY = (ballLocY - prevBallLocY) / SAMPLE_TIME;
    prevBallLocX = ballLocX;
    prevBallLocY = ballLocY;

    // Calc acceleration
    double accCommandX = (L1 * ballLocX) + (L2 * ballVelX) - (L3 * thetaY) - (L4 * omegaY); // angle and omega may be negative
    double accCommandY = (L1 * ballLocY) + (L2 * ballVelY) + (L3 * thetaX) + (L4 * omegaX);

#if (DEADBAND_IN)
    // check if acc is below dead band of motor but above min_acc set acc to acc_min
    if (abs(accCommandX) <= ACC_MIN_X) {
        if (accCommandX >= ACC_DEADZONE) {
            accCommandX = ACC_MIN_X;
        } else if (accCommandX <= -ACC_DEADZONE) {
            accCommandX = -ACC_MIN_X;
        }
    }
    if (abs(accCommandY) <= ACC_MIN_Y) {
        if (accCommandY >= ACC_DEADZONE) {
            accCommandY = ACC_MIN_Y;
        } else if (accCommandY <= -ACC_DEADZONE) {
            accCommandY = -ACC_MIN_Y;
        }
    }
#endif

    // Integrate acc to get velocity    
    double velWheelX = OMEGA2PWM * (-accCommandX * SAMPLE_TIME + ballVelX);
    double velWheelY = OMEGA2PWM * (-accCommandY * SAMPLE_TIME + ballVelY);

    count++;
    if (count % 100 == 0 && printFlag) {
        printf("ballLocX: %f, ballVelX: %f, thetaY: %f, omegaY: %f\n", ballLocX, ballVelX, thetaY, omegaY);
        printf("          %f,           %f,         %f,         %f\n", ballLocX*L1, ballVelX*L2, thetaY*L3, omegaY*L4);
        
        printf("ballLocY: %f, ballVelY: %f, thetaX: %f, omegaX: %f\n", ballLocY, ballVelY, thetaX, omegaX);
        printf("          %f,           %f,         %f,         %f\n", ballLocY*L1, ballVelY*L2, thetaX*L3, omegaX*L4);
        
        printf("accCommandX: %f, accCommandY: %f\n", accCommandX, accCommandY);
        //printf("DeadBandAccX: %f, DeadBandAccY: %f\n", accCommandX, accCommandY);
        printf("velWheelX: %f, velWheelY: %f\n\n\n", velWheelX, velWheelY);
    }
    // set motors
    MotorSet_XYZ(-velWheelX, -velWheelY, 0);
}
/* ------------------------------------------------------------ */
/*                            Main                              */

/* ------------------------------------------------------------ */
int main()
{
    BB_BOARD_Init();

    // IMU_Init();
    PORTSetPinsDigitalOut(JC03); // PMOD Pin JC 03
    PORTClearBits(JC03);

    // Overhead of getting tip angles
    //    static Quat q, result;
    //    IMU_Read_Quat();
    //    IMU_Get_Quat(&q);
    //    BB_Quat_Tip_Vector(&q, &result);
    //    float xAngle = BB_Quat_Find_Tip_Angle_X(&result); // in degrees
    //    float yAngle = BB_Quat_Find_Tip_Angle_Y(&result); // in degrees
    //
    //    IMU_Read_Gyro();

    // Init inner loops, middle, and outer loops
    SetEncoderCounts(0, 0, 0);
    //    PID_Init(&linearX, TRUE, 0, LINEAR_X_P, LINEAR_X_I, LINEAR_X_D);
    //    PID_Init(&linearY, TRUE, 0, LINEAR_Y_P, LINEAR_Y_I, LINEAR_Y_D);
    //
    //    PID_Init(&thetaX, TRUE, xAngle, THETA_X_P, THETA_X_I, THETA_X_D);
    //    PID_Init(&thetaY, TRUE, yAngle, THETA_Y_P, THETA_Y_I, THETA_Y_D);
    //
    //    PID_Init(&omegaX, TRUE, IMU_Get_Gyro_Y(), OMEGA_X_P, OMEGA_X_I, OMEGA_X_D);
    //    PID_Init(&omegaY, TRUE, IMU_Get_Gyro_X(), OMEGA_Y_P, OMEGA_Y_I, OMEGA_Y_D);

    char c = ' '; // init state is stop

    while (1) {
        switch (c) {
        case 0x00:
            c = GetChar();
            break;
        case stop:
            printf("\n\nPress 'r' for Reset, 'b' to Balance, and space-bar to Stop. \n");
            DisableIntT4; // turn off controller interrupt
            MotorsStop();
            SetEncoderCounts(0, 0, 0);
            printf("You chose Stop.\n");
            c = GetChar();
            break;
        case balancing:
            EnableIntT4; // turn on controller interrupt
            printf("You chose balancing.\n");
            PID_GetUARTK();
            c = ' ';
            break;
        case reset:
            DisableIntT4; // turn off controller interrupt
            MotorsStop();
            printf("You chose reset.\n");
            PID_GetUARTK();
            c = ' ';
            break;
        default:
            printf("Please try again.\n");
            c = GetChar();
        }

    }

    return 0;
}

void PID_GetUARTK()
{
    char c;
    while (c != ' ') { // while not escape
        printf("\nLinear PID values: ");
        PID_PrintK(&linearX); // reason for X only, is because X and Y are the same, for now.
        printf("\nTheta  PID values: ");
        PID_PrintK(&thetaX);
        printf("\nOmega  PID values: ");
        PID_PrintK(&omegaX);
        printf("\nPress space at any time to quit. Press 'p' to toggle interrupt printing.\n");
        printf("\nPlease enter 'l' for linear, 't' for theta, or 'w' for omega controller.\n");
        while (c != linear && c != theta && c != omega && c != ' ') {
            c = GetChar();
            switch (c) {
            case 0x00:
                break;
            case linear:
                controller2changeX = &linearX;
                controller2changeY = &linearY;
                printf("You chose the linear controller.\n");
                break;
            case theta:
                controller2changeX = &thetaX;
                controller2changeY = &thetaY;
                printf("You chose the theta controller.\n");
                break;
            case omega:
                controller2changeX = &omegaX;
                controller2changeY = &omegaY;
                printf("You chose the omega controller.\n");
                break;
            case 'p':
                // Toggle printFlag
                if (printFlag == TRUE) {
                    printFlag = FALSE;
                } else if (printFlag == FALSE) {
                    printFlag = TRUE;
                }
                break;
            default:
                printf("Please try again\n");
            }
        }
        printf("Please enter 'p', 'i', or 'd' for PID.\n");
        while (c != proportional && c != integral && c != derivative && c != ' ') {
            c = GetChar();
            switch (c) {
            case 0x00:
                break;
            case proportional:
                k2changeX = &controller2changeX->kp;
                k2changeY = &controller2changeY->kp;
                printf("You chose kp.\n");
                break;
            case integral:
                k2changeX = &controller2changeX->ki;
                k2changeY = &controller2changeY->ki;
                printf("You chose ki.\n");
                break;
            case derivative:
                k2changeX = &controller2changeX->kd;
                k2changeY = &controller2changeY->kd;
                printf("You chose kd.\n");
                break;
            default:
                printf("Please try again\n");
            }
        }
        double magnitude = 1;
        printf("Press 'c' to change a different value to change.\n");
        printf("Press 'j' and 'l' to change magnitude of incrementation. Currently: %f\n", magnitude);
        printf("Press 'i' and 'k' to increment and decrement by magnitude.\n");
        printf("Press space to leave Reset\n");
        while (c != ' ' && c != 'c') {
            c = GetChar();
            switch (c) {
            case 0x00:
                break;
            case 'i': // up
                *k2changeX = *k2changeX + magnitude;
                *k2changeY = *k2changeY + magnitude;
                printf("Value incremented to: %f\n", *k2changeX);
                break;
            case 'j': // left
                magnitude /= 10.0;
                printf("Magnitude decreased: %f\n", magnitude);
                break;
            case 'k': // down
                *k2changeX = *k2changeX - magnitude;
                *k2changeY = *k2changeY - magnitude;
                printf("Value decremented to: %f\n", *k2changeX);
                break;
            case 'l': // right
                magnitude *= 10.0;
                printf("Magnitude increased: %f\n", magnitude);
                break;
            case '0':
                *k2changeX = 0;
                *k2changeY = 0;
                printf("Value zeroed to: %f\n", *k2changeX);
                break;
            case ' ':
                printf("You chose to quit\n");
                break;
            default:
                printf("Please try again\n");
            }
        }

    }
}