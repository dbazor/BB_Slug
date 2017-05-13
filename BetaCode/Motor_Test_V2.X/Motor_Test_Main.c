/*
 * File:   Morot_Test_Main.c
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

    int pwm = 300;

    //    // Tests:
    //    //  1) Test direction of motion against note book
    //    printf("Started Motor test\n");
    //    DelayMs(500);
    //    
    //    printf("    Testing direction of motion...\n");
    //    DelayMs(500);
    //    
    //    printf("    Testing motor 1\n");
    //    printf("        Forward\n");
    //    SetMotorSpeed(pwm, MOTOR_1);
    //    DelayMs(3000);
    //    MotorsStop();
    //    DelayMs(500);
    //    printf("        Backward\n");
    //    SetMotorSpeed(-pwm, MOTOR_1);
    //    DelayMs(3000);
    //    MotorsStop();
    //    DelayMs(500);
    //    printf("    Completed testing motor 1\n\n");
    //    DelayMs(3000);
    //    
    //    printf("    Testing motor 2\n");
    //    printf("        Forward\n");
    //    SetMotorSpeed(pwm, MOTOR_2);
    //    DelayMs(3000);
    //    MotorsStop();
    //    DelayMs(500);
    //    printf("        Backward\n");
    //    SetMotorSpeed(-pwm, MOTOR_2);
    //    DelayMs(3000);
    //    MotorsStop();
    //    DelayMs(500);
    //    printf("    Completed testing motor 2\n\n");
    //    DelayMs(3000);
    //    
    //    printf("    Testing motor 3\n");
    //    printf("        Forward\n");
    //    SetMotorSpeed(pwm, MOTOR_3);
    //    DelayMs(3000);
    //    MotorsStop();
    //    printf("        Backward\n");
    //    SetMotorSpeed(-pwm, MOTOR_3);
    //    DelayMs(3000);
    //    MotorsStop();
    //    printf("    Completed testing motor 3\n\n");
    //    DelayMs(3000);
    //    
    //    printf("    Testing SetMotor_XYZ()\n");
    //    DelayMs(3000);

    //  2) Test SetMotor_XYZ
    //  test x
    printf("        x\n\n");
    SetMotor_XYZ(pwm, 0, 0);
    DelayMs(3000);
    MotorsStop();
    DelayMs(3000);

    printf("        -x\n\n");
    SetMotor_XYZ(-pwm, 0, 0);
    DelayMs(3000);
    MotorsStop();
    DelayMs(3000);

    //  test y
    printf("        y\n\n");
    SetMotor_XYZ(0, pwm, 0);
    DelayMs(3000);
    MotorsStop();
    DelayMs(3000);

    printf("        -y\n\n");
    SetMotor_XYZ(0, -pwm, 0);
    DelayMs(3000);
    MotorsStop();
    DelayMs(3000);

    //  test z
    printf("        z\n\n");
    SetMotor_XYZ(0, 0, pwm);
    DelayMs(3000);
    MotorsStop();
    DelayMs(3000);

    printf("        -z\n\n");
    SetMotor_XYZ(0, 0, -pwm);
    DelayMs(3000);
    MotorsStop();
    DelayMs(3000);

    printf("Finished Motor test\n\n");
    DelayMs(3000);

    float euler_Pitch;
    float euler_Roll;
    float euler_Yaw;
    Quat q, vector;



    // END OF UNIT TEST
    printf("\n Testing tipping with IMU!\n\n");
    while (1) {
        IMU_Read_Euler_Angles();
        IMU_Read_Quat();
        IMU_Read_Calibration();
        IMU_Read_Gyro();

        printf("Sys_Cal :  %d \n", IMU_Get_Sys_Cal());
        printf("Gyro_Cal:  %d \n", IMU_Get_Gyro_Cal());
        printf("Acc_Cal :  %d \n", IMU_Get_Acc_Cal());
        printf("Mag_Cal :  %d \n", IMU_Get_Mag_Cal());


        euler_Yaw = IMU_Get_Euler_Yaw();
        euler_Pitch = IMU_Get_Euler_Pitch();
        euler_Roll = IMU_Get_Euler_Roll();
        printf("Euler Yaw  :  %f \n", euler_Yaw);
        printf("Euler Pitch:  %f \n", euler_Pitch);
        printf("Euler Roll :  %f \n", euler_Roll);

        IMU_Get_Quat(&q);
        printf("Quaternion : ");
        BB_Quat_Print(&q);

        BB_Quat_Normalize(&q);
        printf("Quaternion normed: ");
        BB_Quat_Print(&q);

        /* Quat tip vector */
        BB_Quat_Tip_Vector(&q, &vector);

        printf("tip vector: ");
        BB_Quat_Print(&vector);

        /* Test BB_QUAT_FIND_TIP_ANGLE_X */
        float xAngle = BB_Quat_Find_Tip_Angle_X(&vector);
        printf("\nTip angle x currently is: %frad and %fdeg\n", xAngle, RadiansToDegrees(xAngle));

        /* Test BB_QUAT_FIND_TIP_ANGLE_Y */
        float yAngle = BB_Quat_Find_Tip_Angle_Y(&vector);
        printf("\nTip angle y currently is: %frad and %fdeg\n", yAngle, RadiansToDegrees(yAngle));
        printf("\n\n\n\n\n");
        DelayMs(500);
    }
    while (1) {
        ;
    }

    return 0;

}
