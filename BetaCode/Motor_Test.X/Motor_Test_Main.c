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

/* ------------------------------------------------------------ */
/*				Main											*/

/* ------------------------------------------------------------ */

int main() {
    BB_BOARD_Init();
    DelayInit();
    DeviceInit();
    
    // Test PMD read write
    #if 0
    {
        // PORTWrite(IOPORT_E, 1);

        // I think Port E is Port JB on the Mx7, so
        // Below: Set all pins of JB to be outputs
        // Set chipkit pin 8 ()
        
        while (1) {
            PORTToggleBits(IOPORT_E, BIT_1);
            PORTToggleBits(IOPORT_G, BIT_15);       // LED 4
            DelayMs(500);
            if (PORTReadBits(IOPORT_E, BIT_0)) {
                PORTClearBits(IOPORT_G, BIT_12);
            } else {
                PORTSetBits(IOPORT_G, BIT_12);
            }
        }
    }
    #endif

    // Test PWM
    #if 0
    {
                T2CONbits.TCKPS = 2;     // Timer2 prescaler N=4 (1:4)
                PR2 = 1999;              // period = (PR2+1) * N * 12.5 ns = 100 us, 10 kHz
                TMR2 = 0;                // initial TMR2 count is 0
                OC1CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC1CON bits are defaults
                OC2CONbits.OCM = 0b110;  // PWM mode without fault pin; other OC2CON bits are defaults
                OC1RS = 500;             // duty cycle = OC1RS/(PR2+1) = 25%
                OC2RS = 500;             // duty cycle = OC2RS/(PR2+1) = 25%
                OC1R = 500;              // initialize before turning OC1 on; afterward it is read-only
                OC2R = 500;              // initialize before turning OC2 on; afterward it is read-only
                T2CONbits.ON = 1;        // turn on Timer2
                OC1CONbits.ON = 1;       // turn on OC1
                OC2CONbits.ON = 1;       // turn on OC2
        
                _CP0_SET_COUNT(0);       // delay 4 seconds to see the 25% duty cycle on a 'scope
                while(_CP0_GET_COUNT() < 4 * 40000000) {
                    ;
                }
                OC1RS = 500;            // set duty cycle to 25%
                OC2RS = 1500;            // set duty cycle to 75%
                
        // Now Testing to read a the PWM signal
        while (1) {
            ; // infinite loop
        }
    }
    #endif

    // Incrementally spin motor 1 forward
    int d;
    for (d = 0; d <= 2000; d = d+250) {
        //Motor1(d);
        DelayMs(500);
    }
    //Motor1(0);      // Stop motor 1
    
    // Sit and spin
    while(1) {
        ;
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

void DeviceInit() {
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

void DelayInit() {
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

void DelayMs(int cms) {
    int ims;

    for (ims = 0; ims < cms; ims++) {
        WriteTimer1(0);
        while (ReadTimer1() < cntMsDelay);
    }
}