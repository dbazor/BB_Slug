/*
 * File:   MotorControllerMain.c
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

#define THETA_X_P 225
#define THETA_X_I 7
#define THETA_X_D 0

#define THETA_Y_P THETA_X_P
#define THETA_Y_I THETA_X_I
#define THETA_Y_D THETA_X_D

#define OMEGA_X_P 1
#define OMEGA_X_I 0
#define OMEGA_X_D .1

#define OMEGA_Y_P OMEGA_X_P
#define OMEGA_Y_I OMEGA_X_I
#define OMEGA_Y_D OMEGA_X_D

/* ------------------------------------------------------------ */
/*				Prototypes			*/
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Global Variables		*/
/* ------------------------------------------------------------ */
volatile float eCountRadians = 0;
volatile PIDControl thetaX;
volatile PIDControl thetaY;
volatile PIDControl omegaX;
volatile PIDControl omegaY;

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
    PID_Init(&thetaX, TRUE, xAngle, THETA_X_P, THETA_X_I, THETA_X_D);
    PID_Init(&thetaY, TRUE, yAngle, THETA_Y_P, THETA_Y_I, THETA_Y_D);
    PID_Init(&omegaX, TRUE, IMU_Get_Gyro_Y(), OMEGA_X_P, OMEGA_X_I, OMEGA_X_D);
    PID_Init(&omegaY, TRUE, IMU_Get_Gyro_X(), OMEGA_Y_P, OMEGA_Y_I, OMEGA_Y_D);

    // for the button
    int buttonPushed = 0;
    int count = 0;
    PORTSetPinsDigitalIn(IOPORT_G, BIT_6); 
    
    while (1) {
        
    }

    return 0;
}