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
#include "BB_QUAT.h"

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
    // IMU_Init();
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


    // START UNIT TEST
    //

    // Below are the MATLAB dummy valued quaternions

    Quat q, r, result, qInv;
    
    /* Test Quat fill*/
    // fill a q the slow way
    q.w = 1;
    q.x = 2;
    q.y = 3;
    q.z = 4;
    // fill r with quat fill
    BB_Quat_Fill(1, 2, 3, 4, &r);
    // print both which should be the same
    printf("\nThe quaternion q should be the same as r.\n")
    printf("q: ");
    BB_Quat_Print(&q);
    printf(" r: ");
    BB_Quat_Print(&r);
    
    /* Test Quat Inv */
    BB_Quat_Inv(q, &qInv); // fill qInv with the inverse of q
    printf("\nThe quaternion qInv should have negated vector x y z values.\n")
    printf("q: ");
    BB_Quat_Print(&q);
    printf(" qInv: ");
    BB_Quat_Print(&qInv);
    
    /* Test Quat mult */
    BB_Quat_Fill(1, 0, 2, 3, &q); // q = [ 1 0 2 3 ]'
    BB_Quat_Fill(0, 1, 3, 1, &r); // r = [ 0 1 4 1 ]'
    BB_Quat_Mult(&q, &r, &result); // Multiply the two. Order matters.
    
    printf("\nThe quat mult result should be  .\n")
    printf("q: ");
    BB_Quat_Print(&q);
    printf(" qInv: ");
    BB_Quat_Print(&qInv);
    // Test Quat rotate
    // Test Quat tip vector
    // Test BB_QUAT_FIND_TIP_ANGLE_X
    // Test BB_QUAT_FIND_TIP_ANGLE_Y

    BB_Quat_Fill(1, 0, 2, 3, &q);
    BB_Quat_Fill(0, 1, 4, 1, &r);
    
    

    //
    // END OF UNIT TEST

    while (1) {
        //        IMU_Read_Euler_Angles();
        //        IMU_Read_Quaternion();
        //        IMU_Read_Calibration();

        //        quat_w = IMU_Get_Quat_W();
        //        quat_x = IMU_Get_Quat_X();
        //        quat_y = IMU_Get_Quat_Y();
        //        quat_z = IMU_Get_Quat_Z();
        //        printf("Quaternion W :  %f \n"  ,quat_w);
        //        printf("Quaternion X :  %f \n"  ,quat_x);
        //        printf("Quaternion Y :  %f \n"  ,quat_y);
        //        printf("Quaternion Z :  %f \n\n",quat_z);
        //        
        //        euler_Yaw = IMU_Get_Euler_Yaw();
        //        euler_Pitch = IMU_Get_Euler_Pitch();
        //        euler_Roll  = IMU_Get_Euler_Roll();
        //        printf("Euler Yaw  :  %f \n",euler_Yaw);
        //        printf("Euler Pitch:  %f \n",euler_Pitch);
        //        printf("Euler Roll :  %f \n",euler_Roll);
        //        
        //        printf("Sys_Cal :  %d \n",IMU_Get_Sys_Cal());
        //        printf("Gyro_Cal:  %d \n",IMU_Get_Gyro_Cal());
        //        printf("Acc_Cal :  %d \n",IMU_Get_Acc_Cal());
        //        printf("Mag_Cal :  %d \n",IMU_Get_Mag_Cal());
        DelayMs(1000);
    }

    return 0;

}
