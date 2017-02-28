/* 
 * File:   Talking_Test.c
 * Author: Daniel Bazor
 *
 * Created on February 02, 2017, 10:27 AM
 */
/************************************************************************/
/*  File Description:							*/
/*		This project cycles through LD1 to LD4                  */
/*									*/
/************************************************************************/

#include "BB_BOARD.h"
#include <plib.h>
#include <stdio.h>

/* ------------------------------------------------------------ */
/*				Configuration Bits				                */
/* ------------------------------------------------------------ */


/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void BB_ReadUART1(char * string, int maxLength);
void BB_WriteUART1(const char * string);
void DeviceInit();
void DelayInit();
void DelayMs(int cms);

/* ------------------------------------------------------------ */
/*				Definitions										*/
/* ------------------------------------------------------------ */

#define	cntMsDelay	10000		//timer 1 delay for 1ms
#define MAX_MESSAGE_LENGTH 200

/* ------------------------------------------------------------ */
/*				Main											*/

/* ------------------------------------------------------------ */

int main()
{
    char message[MAX_MESSAGE_LENGTH] = "Testing! Testing! 1! 2! 3!";
    
    BB_BOARD_Init();

    //Set LD1 through LD4 as digital output
    DeviceInit();
    //initialize timer for delay
    DelayInit();

    /* Perform the main application loop.
     */

    while (1) {
        PutChar('x');
        //printf(" this is a mutha fucking test \n");

        BB_ReadUART1(message, MAX_MESSAGE_LENGTH); // get message from computer
        BB_WriteUART1(message); // send message back
        BB_WriteUART1("\r\n");
        
        //PutChar('T');
        
        //drive LD1 high
        PORTWrite(IOPORT_G, BIT_12);
        DelayMs(100);
        // drive LD2 high
        PORTWrite(IOPORT_G, BIT_13);
        DelayMs(100);
        //drive LD3 high
        PORTWrite(IOPORT_G, BIT_14);
        DelayMs(100);
        //drive LD4 high
        PORTWrite(IOPORT_G, BIT_15);
        DelayMs(100);




    }
}


// Read from UART3
// block other functions until you get a '\r' or '\n'
// send the pointer to your char array and the number of elements in the array

void BB_ReadUART1(char * message, int maxLength)
{
    char data = 0;
    int complete = 0, num_bytes = 0;
    // loop until you get a '\r' or '\n'
    while (!complete) {
        if (U1STAbits.URXDA) { // if data is available
            data = U1RXREG; // read the data
            if ((data == '\n') || (data == '\r')) {
                complete = 1;
            } else {
                message[num_bytes] = data;
                ++num_bytes;
                // roll over if the array is too small
                if (num_bytes >= maxLength) {
                    num_bytes = 0;
                }
            }
        }
    }
    // end the string
    message[num_bytes] = '\0';
}

// Write a character array using UART3

void BB_WriteUART1(const char * string)
{
    while (*string != '\0') {
        while (U1STAbits.UTXBF) {
            ; // wait until tx buffer isn't full
        }
        U1TXREG = *string;
        ++string;
    }
}
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