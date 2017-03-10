/*
 * File:   Motor_Test_Main.c
 * Author: Pavlo Vlastos
 * Based on Talking_Test.c by Daniel Bazor
 * Created on February 18, 2017, 1:17 PM
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
/* ------------------------------------------------------------ */
/*				Definitions										*/
/* ------------------------------------------------------------ */
#define cntMsDelay 10000

/* ------------------------------------------------------------ */
/*				Prototypes										*/
/* ------------------------------------------------------------ */
void DeviceInit();
void DelayInit();
void DelayMs(int cms);
//extern interrupt void InputCapture2(void);


/* ------------------------------------------------------------ */
/*				Private Variables								*/
/* ------------------------------------------------------------ */
volatile int m1E1Count = 0;         // motor 1 encoder count
volatile int m2E2Count = 0;         // motor 2 encoder count
//volatile int m3E3Count = 0;         // motor 2 encoder count
//volatile int localm1E1Count = 0;

///* ------------------------------------------------------------ */
///*				Interrupt Sub-Routine             */
///* ------------------------------------------------------------ */

// ISR for input capture 2
//void __ISR(_INPUT_CAPTURE_2_VECTOR, IPL3SOFT) InputCapture2()
//{
//    int ic2time = IC2BUF; // clear fifo
//    
//    int encoder1A = PORTReadBits(ENCODER_1A);  // input capture for motor 1 encoder signal A
//    int encoder1B = PORTReadBits(ENCODER_1B);  // digital input for motor 1 encoder signal B
//
//    // if encoder A is high (greater than 0)
//    // and if encoder B is also high (greater than 0)
//    //      increment encoder count 
//    // else if A and B are not equal and are both not greater than zero
//    //      decrement encoder count
//    if (((encoder1A > 0) && (encoder1B > 0)) || (encoder1A == encoder1B)) {
//        m1E1Count++;
//        // localm1E1Count++;
//        // SetEncoder1Count(localm1E1Count);
//    } else if (encoder1A != encoder1B) {
//        m1E1Count--;
//        // localm1E1Count--;
//        // SetEncoder1Count(localm1E1Count);
//    }
//    IFS0bits.IC2IF = 0; // clear interrupt flag
//}

// ISR for input capture 3 (MOTOR 2)
//void __ISR(_INPUT_CAPTURE_3_VECTOR, IPL3SOFT) InputCapture3()
//{    
//    int ic3time = IC3BUF; // clear fifo
//
//    int encoder2A = PORTReadBits(ENCODER_2A); // input capture for motor 2 encoder signal A
//    int encoder2B = PORTReadBits(ENCODER_2B);  // digital input for motor 2 encoder signal B
//
//    // if encoder A is high (greater than 0)
//    // and if encoder B is also high (greater than 0)
//    //      increment encoder count 
//    // else if A and B are not equal and are both not greater than zero
//    //      decrement encoder count
//    if (((encoder2A > 0) && (encoder2B > 0)) || (encoder2A == encoder2B)) {
//        m2E2Count++;
//    } else if (encoder2A != encoder2B) {
//        m2E2Count--;
//    }
//        
//    IFS0bits.IC3IF = 0; // clear interrupt flag
//}

// ISR for input capture 5 (MOTOR 3)
//void __ISR(_INPUT_CAPTURE_5_VECTOR, IPL3SOFT) InputCapture5()
//{
//    int ic5time = IC5BUF; // clear fifo
//
//    int encoder3A = PORTReadBits(ENCODER_3A); // input capture for motor 3 encoder signal A
//    int encoder3B = PORTReadBits(ENCODER_3B);  // digital input for motor 3 encoder signal B
//
//    // see previous ISR for details
//    if (((encoder3A > 0) && (encoder3B> 0)) || (encoder3A == encoder3B)) {
//        m3E3Count++;
//    } else if (encoder3A != encoder3B) {
//        m3E3Count--;
//    }
//    IFS0bits.IC5IF = 0; // clear interrupt flag
//}

/* ------------------------------------------------------------ */
/*				Main											*/

/* ------------------------------------------------------------ */ 

int main()
{
    BB_BOARD_Init();
    DelayInit();
    DeviceInit();

    SetMotorSpeed(500, MOTOR_1);
    while (1) {
        
        int i = GetEncoder1Count(); 
        
        if (i >= 2240 || i <= -2240) {
           SetEncoder1Count(0);
            //PORTToggleBits(BB_LED_2);  
           //Turn_On_LED(BB_LED_2); 
           Toggle_LED(BB_LED_4);
        }
        
        int j = GetEncoder2Count(); 
        if (j >= 2240 || j <= -2240) {
            SetEncoder2Count(0);
            PORTToggleBits(BB_LED_3);
        }
        
        int k = GetEncoder3Count(); 
        
        if (k >= 2240 || k <= -2240) {
            SetEncoder3Count(0);
            PORTToggleBits(BB_LED_4);
        } 
       
    }

    return 0;
}

//
//

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

/*  DeviceInit()
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Set LD1 through LD4 as digital output
/* ------------------------------------------------------------ */

void DeviceInit()
{
    //On MX4ck LED1 is on RG12
    //		   LED2 is on RG13
    //		   LED3 is on RG14
    //		   LED4 is on RG15
    //Set ports for onboard LEDs to outputs
    PORTSetPinsDigitalOut(IOPORT_G, BIT_12 | BIT_13 | BIT_14 | BIT_15);
    PORTSetPinsDigitalOut(IOPORT_E, BIT_1);
    PORTSetPinsDigitalIn(IOPORT_E, BIT_0);
}

/* ------------------------------------------------------------ */

/***	DelayInit
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Initialized the hardware for use by delay functions. This
 **		initializes Timer 1 to count at 10Mhz.
/* ------------------------------------------------------------ */

void DelayInit()
{
    unsigned int tcfg;

    /* Configure Timer 1. This sets it up to count a 10Mhz with a period of 0xFFFF
     */
    tcfg = T1_ON | T1_IDLE_CON | T1_SOURCE_INT | T1_PS_1_8 | T1_GATE_OFF | T1_SYNC_EXT_OFF;
    OpenTimer1(tcfg, 0xFFFF);

}

/* ------------------------------------------------------------ */

/***	DelayMs
 **
 **	Parameters:
 **		cms			- number of milliseconds to delay
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Delay the requested number of milliseconds. Uses Timer1.
/* ------------------------------------------------------------ */

void DelayMs(int cms)
{
    int ims;

    for (ims = 0; ims < cms; ims++) {
        WriteTimer1(0);
        while (ReadTimer1() < cntMsDelay);
    }
}