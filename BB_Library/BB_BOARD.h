/* 
 * File:   BB_BOARD.h
 * Author: Daniel Bazor
 *
 * Created on February 2, 2017, 12:10 AM
 */
//suppresses annoying warnings
#ifndef _SUPPRESS_PLIB_WARNING
#define _SUPPRESS_PLIB_WARNING
#endif

#ifndef _DISABLE_OPENADC10_CONFIGPORT_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#endif

#ifndef BB_BOARD_H
#define	BB_BOARD_H

#include <xc.h>                     // processor SFR definitions
#include <sys/attribs.h>            // __ISR macro
#include <plib.h>
#include <stdint.h>
#include <GenericTypeDefs.h> 
#include <plib.h> 
#include <stdio.h> 

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

#define SYS_FREQ (80000000ul)    // 80 million Hz
#define PB_DIV   1
#define ERROR   -1

#define BB_LED_1 IOPORT_G, BIT_12
#define BB_LED_2 IOPORT_G, BIT_13
#define BB_LED_3 IOPORT_G, BIT_14
#define BB_LED_4 IOPORT_G, BIT_15

#define ENCODER_1A IOPORT_D, BIT_9
#define ENCODER_2A IOPORT_D, BIT_10
#define ENCODER_3A IOPORT_D, BIT_12

#define ENCODER_1B IOPORT_E, BIT_4
#define ENCODER_2B IOPORT_E, BIT_5
#define ENCODER_3B IOPORT_E, BIT_6

#define	GetSystemClock()              (80000000ul)
#define	GetPeripheralClock()          (GetSystemClock()/(1 << OSCCONbits.PBDIV))
#define	GetInstructionClock()         (GetSystemClock())

/*****************************************************************************/

/**
 * Function: BOARD_Init(void)
 * @param None
 * @return None
 * @brief Set the clocks up for the board, initializes the serial port, and turns
 * on the A/D subsystem for battery monitoring
 * @author Daniel Bazor 2017.02.02  */
void BB_BOARD_Init();


/**
 * Function: BOARD_End(void)
 * @param None
 * @return None
 * @brief shuts down all peripherals except for serial and A/D. Turns all pins
 * into input
 * @author Daniel Bazor 2017.02.02  */
void BB_BOARD_End();

/**
 * Function: BOARD_GetPBClock(void)
 * @param None
 * @return PB_CLOCK - speed the peripheral clock is running in hertz
 * @brief returns the speed of the peripheral clock.  Nominally at 40Mhz
 * @author Daniel Bazor 2017.02.02 */
unsigned int BB_BOARD_GetPBClock();

/* ------------------------------------------------------------ */

/***	DelayMs
 **
 **	Parameters:
 **		t -> number of milliseconds to delay
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

void DelayMs(unsigned t);

/***	DelayUs
 **
 **	Parameters:
 **		t -> number of microseconds to delay
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Delay the requested number of microseconds. Uses Timer1.
/* ------------------------------------------------------------ */

void DelayUs(unsigned t);

#endif	/* BB_BOARD_H */



