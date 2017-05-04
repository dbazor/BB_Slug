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

    float quat_w;
    float quat_x;
    float quat_y;
    float quat_z;
    float euler_Pitch;
    float euler_Roll;
    float euler_Yaw;
    while (1) {
        IMU_Read_Euler_Angles();
        IMU_Read_Quaternion();
        IMU_Read_Calibration();
        
        quat_w = IMU_Get_Quat_W();
        quat_x = IMU_Get_Quat_X();
        quat_y = IMU_Get_Quat_Y();
        quat_z = IMU_Get_Quat_Z();
        printf("Quaternion W :  %f \n"  ,quat_w);
        printf("Quaternion X :  %f \n"  ,quat_x);
        printf("Quaternion Y :  %f \n"  ,quat_y);
        printf("Quaternion Z :  %f \n\n",quat_z);
        
        euler_Yaw = IMU_Get_Euler_Yaw();
        euler_Pitch = IMU_Get_Euler_Pitch();
        euler_Roll  = IMU_Get_Euler_Roll();
        printf("Euler Yaw  :  %f \n",euler_Yaw);
        printf("Euler Pitch:  %f \n",euler_Pitch);
        printf("Euler Roll :  %f \n",euler_Roll);
        
        printf("Sys_Cal :  %d \n",IMU_Get_Sys_Cal());
        printf("Gyro_Cal:  %d \n",IMU_Get_Gyro_Cal());
        printf("Acc_Cal :  %d \n",IMU_Get_Acc_Cal());
        printf("Mag_Cal :  %d \n",IMU_Get_Mag_Cal());
        DelayMs(1000);
    }

    return 0;

}
