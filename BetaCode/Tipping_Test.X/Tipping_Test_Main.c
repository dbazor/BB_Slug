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
#include "BB_MOTOR_CONTROLLER.h"

/* ------------------------------------------------------------ */
/*				Definitions			*/
/* ------------------------------------------------------------ */
#define cntMsDelay      5000
#define thsndDelay      1000

/* ------------------------------------------------------------ */
/*				Prototypes			*/
/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*				Global Variables		*/
/* ------------------------------------------------------------ */
volatile float eCountRadians = 0;

/* ------------------------------------------------------------ */
/*                            Main                              */

/* ------------------------------------------------------------ */
int main()
{
    BB_BOARD_Init();
    IMU_Init();
    PORTSetPinsDigitalOut(JC03); // PMOD Pin JC 03
    PORTClearBits(JC03);

    //PID_Init(&motor1_pid, TRUE, MOTOR_1, MOTOR1_KP, MOTOR1_KI, MOTOR1_KD);
    //    PIDControl printArray[10];

    float Scaler;
    float Quat_X;
    float Quat_Y;
    float Quat_Z;
    float euler_Pitch;
    float euler_Roll;
    float euler_Yaw;
    while (1) {
        IMU_Read_Euler_Angles();
        IMU_Read_Quaternion();
        Scaler = IMU_Get_Quat_Scalar();
        Quat_X = IMU_Get_Quat_X();
        Quat_Y = IMU_Get_Quat_Y();
        Quat_Z = IMU_Get_Quat_Z();
        printf(" Quaternion X      :  %f \n"  ,Quat_X);
        printf(" Quaternion Y      :  %f  \n" ,Quat_Y);
        printf(" Quaternion Z      :  %f \n"  ,Quat_Z);
        printf(" Quaternion Scaler :  %f \n\n",Scaler);
        euler_Pitch = IMU_Get_Euler_Pitch();
        euler_Roll  = IMU_Get_Euler_Roll();
        euler_Yaw = IMU_Get_Euler_Yaw();
        printf(" Euler Pitch        :  %f \n",euler_Pitch);
        printf(" Euler Roll         :  %f  \n",euler_Roll);
        printf(" Euler Yaw          :  %f \n",euler_Yaw);
        DelayMs(2000);
    }
    while (1); // sit and spin

    return 0;

}
