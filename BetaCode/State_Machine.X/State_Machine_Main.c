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

#define THETA_X_P 250
#define THETA_X_I 0
#define THETA_X_D 5

#define THETA_Y_P THETA_X_P
#define THETA_Y_I THETA_X_I
#define THETA_Y_D THETA_X_D

#define OMEGA_X_P 100
#define OMEGA_X_I 0
#define OMEGA_X_D 100

#define OMEGA_Y_P OMEGA_X_P
#define OMEGA_Y_I OMEGA_X_I
#define OMEGA_Y_D OMEGA_X_D

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

    char c = 0x00;

    while (1) {
        printf("\n\nPress 'r' for Reset, 'b' to Balance, and space-bar to Stop. \n");
        while (c != 'r' || c != 'b' || c != ' ') {
            c = GetChar();
            switch (c) {
            case 0x00:
                break;
            case stop:
                DisableIntT4; // turn off controller interrupt
                MotorsStop();
                printf("You chose Stop.\n");
                c = 0x00;
                break;
            case balancing:
                EnableIntT4; // turn on controller interrupt
                printf("You chose balancing.\n");
                PID_GetUARTK();
                c = 0x00;
                break;
            case reset:
                DisableIntT4; // turn off controller interrupt
                MotorsStop();
                printf("You chose reset.\n");
                PID_GetUARTK();
                c = 0x00;
                break;
            default:
                printf("Please try again.\n");
            }
        }
    }

    return 0;
}

void PID_GetUARTK()
{
    char c;
    while (c != 'q') { // while not escape
        printf("\nLinear PID values: ");
        PID_PrintK(&linearX); // reason for X only, is because X and Y are the same, for now.
        printf("\nTheta  PID values: ");
        PID_PrintK(&thetaX);
        printf("\nOmega  PID values: ");
        PID_PrintK(&omegaX);
        printf("\nPress 'q' at any time to reset.\n");
        printf("\nPlease enter 'l' for linear, 't' for theta, or 'w' for omega controller.\n");
        while (c != linear && c != theta && c != omega && c != 'q') {
            c = GetChar();
            if (c == linear) {
                controller2changeX = &linearX;
                controller2changeY = &linearY;
                printf("You chose the linear controller.\n");
            } else if (c == theta) {
                controller2changeX = &thetaX;
                controller2changeY = &thetaY;
                printf("You chose the theta controller.\n");
            } else if (c == omega) {
                controller2changeX = &omegaX;
                controller2changeY = &omegaY;
                printf("You chose the omega controller.\n");
            } else if (c != 0x00){
                printf("Please try again\n");
            }
        }
        printf("Please enter 'p', 'i', or 'd' for PID.\n");
        while (c != proportional && c != integral && c != derivative && c != 'q') {
            c = GetChar();
            if (c == proportional) {
                k2changeX = &controller2changeX->kp;
                k2changeY = &controller2changeY->kp;
                printf("You chose kp.\n");
            } else if (c == integral) {
                k2changeX = &controller2changeX->ki;
                k2changeY = &controller2changeY->ki;
                printf("You chose ki.\n");
            } else if (c == derivative) {
                k2changeX = &controller2changeX->kd;
                k2changeY = &controller2changeY->kd;
                printf("You chose kd.\n");
            } else if (c != 0x00) {
                printf("Please try again\n");
            }
        }
        double magnitude = 1;
        printf("Press 'c' to change a different value to change.\n");
        printf("Press 'j' and 'l' to change magnitude of incrementation. Currently: %f\n", magnitude);
        printf("Press 'i' and 'k' to increment and decrement by magnitude.\n");
        printf("Press escape to leave Reset\n");
        while (c != 'q' && c != 'c') {
            c = GetChar();
            if (c != 0x00) {
                printf("c: %c\n", c);
            }
            switch (c) {
            case 0x00:
                break;
            case 'i': // up
                *k2changeX = *k2changeX + magnitude;
                *k2changeY = *k2changeY + magnitude;
                printf("Value incremented to: %f\n", *k2changeX);
                break;
            case 'j': // left
                magnitude /= 10;
                printf("Magnitude decreased: %f\n", magnitude);
                break;
            case 'k': // down
                *k2changeX = *k2changeX - magnitude;
                *k2changeY = *k2changeY - magnitude;
                printf("Value decremented to: %f\n", *k2changeX);
                break;
            case 'l': // right
                magnitude *= 10;
                printf("Magnitude increased: %f\n", magnitude);
                break;
            default:
                printf("Please try again.\n");
            }
        }

    }
}