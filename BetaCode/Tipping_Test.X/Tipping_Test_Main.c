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

//    encodeVal e;
//    
//    printf("Setting encoder counts to 0, 10, -10. Should see: 17.32\n");
//    SetEncoderCounts(0, 10, -10);
//    GetEncoderXYZ(&e);
//    
//    printf("Setting encoder counts to 0, -10, 10. Should see: -17.32\n");
//    SetEncoderCounts(0, -10, 10);
//    GetEncoderXYZ(&e);
//    
//    printf("Setting encoder counts to 50, -50, -50. Should see: 100\n");
//    SetEncoderCounts(50, -50, -50);
//    GetEncoderXYZ(&e);
    
//    double x = 0;
//    double y = 0;
//    double z = 0;
//    printf("x = %f, y = %f, z = %f\n", x, y, z);
//    SetMotor_XYZ(x, y, z);
//    
//    x = 2001;
//    y = 100;
//    z = 100;
//    printf("\nx = %f, y = %f, z = %f\n", x, y, z);
//    SetMotor_XYZ(x, y, z);
//    
//    x = 100;
//    y = 2001;
//    z = 100;
//    printf("\nx = %f, y = %f, z = %f\n", x, y, z);
//    SetMotor_XYZ(x, y, z);
//    
//    x = 100;
//    y = 100;
//    z = 2001;
//    printf("\nx = %f, y = %f, z = %f\n", x, y, z);
//    SetMotor_XYZ(x, y, z);
//    
//    x = 2001;
//    y = 2000;
//    z = 100;
//    printf("\nx = %f, y = %f, z = %f\n", x, y, z);
//    SetMotor_XYZ(x, y, z);
//    
//    x = 2000;
//    y = 2001;
//    z = 100;
//    printf("\nx = %f, y = %f, z = %f\n", x, y, z);
//    SetMotor_XYZ(x, y, z);
//    
//    x = 2000;
//    y = 2000;
//    z = 100;
//    printf("\nx = %f, y = %f, z = %f\n", x, y, z);
//    SetMotor_XYZ(x, y, z);
    
    while (1) {
        
    }

    return 0;
}