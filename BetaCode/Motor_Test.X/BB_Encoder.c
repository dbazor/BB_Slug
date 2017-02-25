/* BB_Encoder.c
 * 
 * Pavlo Vlastos
 * 
 * Created February 22, 2017, 3:07PM
 * 
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include "BB_Encoder.h"
#include <xc.h>
#include <plib.h>

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/


/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

/*  Encoder_Init(void)
 **
 ** Notes:
 **     !!! IMPORTANT !!! Call this function inside of BB_BOARD_Init()
 **     because Input Capture (IC) initialization must be done when 
 **     interrupts are disabled.
 **
 **     NOT DONE YET
 **
 **	Parameters:
 **		
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		
/* ------------------------------------------------------------ */
void Encoder_Init(void)
{
    // Set up pins to read encoder B signals
    PORTSetPinsDigitalIn(IOPORT_E, BIT_4 | BIT_5 | BIT_6);
        
    // set up IC3 to use Timer3. 
    T3CONbits.TCKPS = 0x3;  // Timer3 1:8 prescaler; ticks at 10 MHz (each tick is 100ns)
    PR3 = TMR3_ROLLOVER; 
    
    // rollover value is also used in ISR to handle timer rollovers.
    TMR3 = 0;

    IC2CONbits.ICTMR = 0;   // IC1 uses Timer3
    IC2CONbits.ICM = 6;     // capture every edge
    IC2CONbits.FEDGE = 1;   // capture rising edge first
    IC2CONbits.ICI = 3;     // interrupt every 4th edge
    IFS0bits.IC2IF = 0;     // clear interrupt flag
    IPC2bits.IC2IP = 3;     // interrupt priority 3
    IEC0bits.IC2IE = 1;     // enable IC2 interrupt
    T3CONbits.ON = 1;       // turn on Timer3
    IC2CONbits.ON = 1;      // turn on IC1
    
}
