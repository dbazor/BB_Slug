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

#define THETA_X_P 190
#define THETA_X_I 10
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


#define MOTOR_KP       1
#define MOTOR_KI       0  
#define MOTOR_KD       0

/* ------------------------------------------------------------ */
/*				Prototypes			*/
/* ------------------------------------------------------------ */
void PID_GetUARTK();
void PrintMatlabData();

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
volatile PIDControl dummyCtrl;
volatile PIDControl motorCtlr1;
volatile PIDControl motorCtlr2;
volatile PIDControl motorCtlr3;

volatile PrintData printData;

volatile PIDControl *controller2changeX;
volatile PIDControl *controller2changeY;
volatile PIDControl *controller2changeZ;
volatile double *k2changeX;
volatile double *k2changeY;
volatile double *k2changeZ;

volatile BOOL printFlag;

static double mXspeed = 8.0, mYspeed = 0;
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
    static Quat q, result;
    IMU_Read_Quat();
    IMU_Get_Quat(&q);
    BB_Quat_Tip_Vector(&q, &result);
    float xAngle = BB_Quat_Find_Tip_Angle_X(&result); // in degrees
    float yAngle = BB_Quat_Find_Tip_Angle_Y(&result); // in degrees

    IMU_Read_Gyro();

    // Init inner loops, middle, and outer loops
    SetEncoderCounts(0, 0, 0);
    PID_Init(&linearX, TRUE, 0, LINEAR_X_P, LINEAR_X_I, LINEAR_X_D);
    PID_Init(&linearY, TRUE, 0, LINEAR_Y_P, LINEAR_Y_I, LINEAR_Y_D);

    PID_Init(&thetaX, TRUE, xAngle, THETA_X_P, THETA_X_I, THETA_X_D);
    PID_Init(&thetaY, TRUE, yAngle, THETA_Y_P, THETA_Y_I, THETA_Y_D);

    PID_Init(&omegaX, TRUE, IMU_Get_Gyro_Y(), OMEGA_X_P, OMEGA_X_I, OMEGA_X_D);
    PID_Init(&omegaY, TRUE, IMU_Get_Gyro_X(), OMEGA_Y_P, OMEGA_Y_I, OMEGA_Y_D);

    PID_Init(&motorCtlr1, TRUE, 0, MOTOR_KP, MOTOR_KI, MOTOR_KD);
    PID_Init(&motorCtlr2, TRUE, 0, MOTOR_KP, MOTOR_KI, MOTOR_KD);
    PID_Init(&motorCtlr3, TRUE, 0, MOTOR_KP, MOTOR_KI, MOTOR_KD);

    char c = ' '; // init state is stop

    while (1) {
        switch (c) {
        case 0x00:
            c = GetChar();
            break;
        case stop:
            printf("\n\nPress 'r' for Reset, 'b' to Balance, and space-bar to Stop. \n");
            DisableIntT4; // turn off controller interrupt
            DisableIntT5;
            MotorsStop();
            printf("You chose Stop.\n");
            c = GetChar();
            break;
        case balancing:
            //EnableIntT4; // turn on controller interrupt      // uncomment to activate controller
            EnableIntT5;
            //MotorSet_XYZ(MAX_RAD_PER_SEC, MAX_RAD_PER_SEC, 0); // test motor controller
            printf("You chose balancing.\n");
            PID_GetUARTK();
            c = ' ';
            break;
        case reset:
            DisableIntT4; // turn off controller interrupt
            DisableIntT5;
            MotorsStop();
            printf("You chose reset.\n");
            PID_GetUARTK();
            c = ' ';
            break;
        case 'm':
            while (c != ' ') {
                static double magnitude = 1;
                static MotorSpeedsCmd mVcmd;
                if (printFlag) {
                    PrintMatlabData();
                }
                c = GetChar();
                switch (c) {
                case 'e': // X movement
                    mXspeed += magnitude;
                    printf("Value incremented to: %f\n", mXspeed);
                    break;
                case 'd':
                    mXspeed -= magnitude;
                    printf("Value decremented to: %f\n", mXspeed);
                    break;
                case 'r': // Y movement
                    mYspeed += magnitude;
                    printf("Value incremented to: %f\n", mYspeed);
                    break;
                case 'f':
                    mYspeed -= magnitude;
                    printf("Value decremented to: %f\n", mYspeed);
                    break;
                case 'a':
                    magnitude /= 10.0;
                    printf("Magnitude now: %f\n", magnitude);
                    break;
                case 's':
                    magnitude *= 10.0;
                    printf("Magnitude now: %f\n", magnitude);
                    break;
                case '0':
                    mXspeed = 0;
                    mYspeed = 0;
                    printf("Speeds zeroed\n");
                    break;
                case 'p':
                    // toggle print flag
                    if (printFlag == TRUE) {
                        printFlag = FALSE;
                    } else if (printFlag == FALSE) {
                        printFlag = TRUE;
                    }
                    break;
                }
                MotorSet_XYZ(mXspeed, mYspeed, 0);
                MotorGetCommand(&mVcmd);
                MotorSetSpeed((int) (RAD_PER_SEC_2_PWM * mVcmd.m1), MOTOR_1);
                MotorSetSpeed((int) (RAD_PER_SEC_2_PWM * mVcmd.m2), MOTOR_2);
                MotorSetSpeed((int) (RAD_PER_SEC_2_PWM * mVcmd.m3), MOTOR_3);
            }
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
        printf("\nmotorCtlr1  PID values: ");
        PID_PrintK(&motorCtlr1);
        printf("\nPress space at any time to quit. Press 'p' to toggle interrupt printing.\n");
        printf("\nPlease enter 'l' for linear, 't' for theta, 'w' for omega, or 'm' for motor controller.\n");
        while (c != linear && c != theta && c != omega && c != ' ') {
            PrintMatlabData();
            c = GetChar();
            switch (c) {
            case 0x00:
                break;
            case linear:
                controller2changeX = &linearX;
                controller2changeY = &linearY;
                controller2changeZ = &dummyCtrl;
                printf("You chose the linear controller.\n");
                break;
            case theta:
                controller2changeX = &thetaX;
                controller2changeY = &thetaY;
                controller2changeZ = &dummyCtrl; 
                printf("You chose the theta controller.\n");
                break;
            case omega:
                controller2changeX = &omegaX;
                controller2changeY = &omegaY;
                controller2changeZ = &dummyCtrl; 
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
            case 'm':
                controller2changeX = &motorCtlr1;
                controller2changeY = &motorCtlr2;
                controller2changeZ = &motorCtlr3;
                printf("You chose the motor controller.\n");
                break;
            default:
                printf("Please try again\n");
            }
        }
        printf("Please enter 'p', 'i', or 'd' for PID.\n");
        while (c != proportional && c != integral && c != derivative && c != ' ') {
            PrintMatlabData();
            c = GetChar();
            switch (c) {
            case 0x00:
                break;
            case proportional:
                k2changeX = &controller2changeX->kp;
                k2changeY = &controller2changeY->kp;
                k2changeZ = &controller2changeZ->kp;
                printf("You chose kp.\n");
                break;
            case integral:
                k2changeX = &controller2changeX->ki;
                k2changeY = &controller2changeY->ki;
                k2changeZ = &controller2changeZ->ki;
                printf("You chose ki.\n");
                break;
            case derivative:
                k2changeX = &controller2changeX->kd;
                k2changeY = &controller2changeY->kd;
                k2changeZ = &controller2changeZ->kd;
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
            PrintMatlabData();
            c = GetChar();
            switch (c) {
            case 0x00:
                break;
            case 'i': // up
                *k2changeX = *k2changeX + magnitude;
                *k2changeY = *k2changeY + magnitude;
                *k2changeZ = *k2changeZ + magnitude;
                printf("Value incremented to: %f\n", *k2changeX);
                break;
            case 'j': // left
                magnitude /= 10.0;
                printf("Magnitude decreased: %f\n", magnitude);
                break;
            case 'k': // down
                *k2changeX = *k2changeX - magnitude;
                *k2changeY = *k2changeY - magnitude;
                *k2changeZ = *k2changeZ - magnitude;
                printf("Value decremented to: %f\n", *k2changeX);
                break;
            case 'l': // right
                magnitude *= 10.0;
                printf("Magnitude increased: %f\n", magnitude);
                break;
            case '0':
                *k2changeX = 0;
                *k2changeY = 0;
                *k2changeZ = 0;
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

void PrintMatlabData()
{
    static int count;
    static BOOL forward = 1;
    static motorVelocity mV;
    if ((count % 10000) == 0) {
        if (forward) {
            MotorSet_XYZ(mXspeed, mYspeed, 0);
            forward = 0;
        } else {
            MotorSet_XYZ(-mXspeed, -mYspeed, 0);
            forward = 1;
        }
    }

    if (printData.ready2print) {
        //            printf("%d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n",
        //                    printData.count, printData.angleX, printData.angleY, printData.thetaOutX,
        //                    printData.thetaOutY, printData.gyroX, printData.gyroY,
        //                    printData.omegaOutX, printData.omegaOutY, printData.encoderX, printData.encoderY);
        printf("%d, %f, %f, %f, %f, %f, %f, %f, %f, %f\n",
                printData.count,
                printData.m1Speed,
                printData.m2Speed,
                printData.m3Speed,
                printData.m1Cmd,
                printData.m2Cmd,
                printData.m3Cmd,
                printData.m1Output,
                printData.m2Output,
                printData.m3Output);
        printData.ready2print = FALSE;
    }
}