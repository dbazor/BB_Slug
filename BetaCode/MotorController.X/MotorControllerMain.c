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
#include "BB_MOTOR_CONTROLLER.h"

/* ------------------------------------------------------------ */
/*				Definitions			*/
/* ------------------------------------------------------------ */
#define cntMsDelay      5000
#define thsndDelay      1000

/* ------------------------------------------------------------ */
/*				Prototypes			*/
/* ------------------------------------------------------------ */
void DeviceInit();
void DelayInit();
void DelayMs(unsigned t);
void DelayUs(unsigned t);


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

    PORTSetPinsDigitalOut(JC03); // PMOD Pin JC 03
    PORTClearBits(JC03);

    PID_Init(&motor1_pid, TRUE, MOTOR_1, MOTOR1_KP, MOTOR1_KI, MOTOR1_KD);
    PID_SetReference(&motor1_pid, 0); // set up as regulator
    PID_Print(&motor1_pid);

    printf("\nPID init done\n");
    DelayMs(5000); // delay for 5 seconds
    //    PIDControl printArray[10];
    UINT64 i = 0;
    UINT64 x = 700;
    while (1) {
        if (loopFlag) {
            loopFlag = FALSE;
            PID_Print(&motor1_pid);
        }
        //        printf("i = %d\n", i);
        if (i < x) {
            if (i == 0) {
                printf("\nChanging reference to 560\n");
            }
            PID_SetReference(&motor1_pid, 2240);

        } else if ((i >= x) && (i < (2 * x))) {
            if (i == 1 + x) {
                printf("\nChanging reference to 1120\n");
            }
            PID_SetReference(&motor1_pid, -2240);

        } else {
            i = 0;
        }
        i++;
    }
    printf("After while\n");
    //    for (count = 0; count < 10; count++) {
    //        printf("In for\n");
    //        PID_Print(&printArray[count]);
    //        DelayMs(1000);
    //    }
    while (1); // sit and spin

    return 0;
}
