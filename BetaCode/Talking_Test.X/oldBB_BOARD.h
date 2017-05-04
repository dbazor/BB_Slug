/* 
 * File:   BB_BOARD.h
 * Author: Daniel Bazor
 *
 * Created on February 2, 2017, 12:10 AM
 */

#ifndef BB_BOARD_H
#define	BB_BOARD_H

#include <xc.h>                     // processor SFR definitions
#include <sys/attribs.h>            // __ISR macro
#include "BB_UART.h"
#include <stdint.h>
#include <GenericTypeDefs.h>
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

#define SYS_FREQ 80000000ul    // 80 million Hz

//suppresses annoying warnings
#ifndef _SUPPRESS_PLIB_WARNING
#define _SUPPRESS_PLIB_WARNING
#endif

#ifndef _DISABLE_OPENADC10_CONFIGPORT_WARNING
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
#endif

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


#endif	/* BB_BOARD_H */



