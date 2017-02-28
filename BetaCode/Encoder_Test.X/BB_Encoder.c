/* BB_Encoder.c
 * 
 * Pavlo Vlastos & Mikey Gonzalez & Daniel Bazor
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
static volatile int m1E1Count = 0; // motor 1 encoder count
static volatile int m2E2Count = 0; // motor 2 encoder count
static volatile int m3E3Count = 0; // motor 3 encoder count


/* ------------------------------------------------------------ */
/*				Interrupt Sub-Routine           */
/* ------------------------------------------------------------ */ 

//void __ISR(_INPUT_CAPTURE_2_VECTOR, IPL3SOFT) InputCapture2(void)
//{ 
//    int ic2time = IC2BUF; // clear fifo
//    int encoderA = PORTReadBits(IOPORT_D, BIT_9);
//    int encoderB = PORTReadBits(IOPORT_E, BIT_4);
//
//    // if encoder A is high
//    // and if encoder B is also high
//    // increment encoder count
//    // else if B is low
//    // decrement encoder count
//    if (encoderA == encoderB) {
//        m1E1Count++;
//    } else {
//        m1E1Count--;
//    }
//    IFS0bits.IC2IF = 0; // clear interrupt flag
//} 

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
    // Set up pins to read encoder A signals
    PORTSetPinsDigitalIn(IOPORT_D, BIT_9 | BIT_10 | BIT_12);
    PORTSetPinsDigitalIn(IOPORT_E, BIT_4 | BIT_5 | BIT_6);

    // set up IC3 to use Timer3. 
    T3CONbits.TCKPS = 0x3; // Timer3 1:8 prescaler; ticks at 10 MHz (each tick is 100ns)
    PR3 = TMR3_ROLLOVER;

    // rollover value is also used in ISR to handle timer rollovers.
    TMR3 = 0;

    IC2CONbits.ICTMR = 0;   // IC2 uses Timer3
    IC3CONbits.ICTMR = 0;   // IC3 uses Timer3
    IC5CONbits.ICTMR = 0;   // IC5 uses Timer3
    
    IC2CONbits.ICM = 0b110; // falling and rising for IC2
    IC3CONbits.ICM = 0b110; // falling and rising for IC3
    IC5CONbits.ICM = 0b110; // falling and rising for IC5
    
    IC2CONbits.FEDGE = 0;   // capture rising edge first for IC2
    IC3CONbits.FEDGE = 0;   // capture rising edge first for IC3
    IC5CONbits.FEDGE = 0;   // capture rising edge first for IC5
    
    IC2CONbits.ICI = 0;     // interrupt every 1st edge for IC2
    IC3CONbits.ICI = 0;     // interrupt every 1st edge for IC3
    IC5CONbits.ICI = 0;     // interrupt every 1st edge for IC5
    
    IFS0bits.IC2IF = 0;     // clear interrupt flag for IC2
    IFS0bits.IC3IF = 0;     // clear interrupt flag for IC3
    IFS0bits.IC5IF = 0;     // clear interrupt flag for IC5
    
    IPC2bits.IC2IP = 1;     // interrupt priority 1 for IC2
    IPC3bits.IC3IP = 2;     // interrupt priority 2 for IC3
    IPC5bits.IC5IP = 3;     // interrupt priority 3 for IC5
    
    IEC0bits.IC2IE = 1;     // enable IC2 interrupt
    IEC0bits.IC3IE = 1;     // enable IC3 interrupt
    IEC0bits.IC5IE = 1;     // enable IC5 interrupt
    
    T3CONbits.ON = 1;       // turn on Timer3
    
    IC2CONbits.ON = 1;      // turn on IC2
    IC3CONbits.ON = 1;      // turn on IC3
    IC5CONbits.ON = 1;      // turn on IC5
}
/*  
 **
 ** Notes:
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

int GetEncoder1Count(){ 
    return m1E1Count;
} 
/*  Setting variables function 
 **
 ** Notes:
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

void SetEncoder1Count(int value){ 
     m1E1Count = value;
}

/*  
 **
 ** Notes:
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

int GetEncoder2Count(){ 
    return m2E2Count;
} 
/*  Setting variables function 
 **
 ** Notes:
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

void SetEncoder2Count(int value){ 
     m2E2Count = value;
}

/*  
 **
 ** Notes:
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

int GetEncoder3Count(){ 
    return m3E3Count;
} 
/*  Setting variables function 
 **
 ** Notes:
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

void SetEncoder3Count(int value){ 
     m3E3Count = value;
}