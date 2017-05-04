/* 
 * File:   BOARD.h
 * Author: Daniel Bazor
 *
 * Created on February 2, 2017, 12:10 AM
 */

#ifndef BOARD_H
#define	BOARD_H

#include <stdint.h>
#include <GenericTypeDefs.h>
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

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
 * @author Max Dunne, 2013.09.15  */
void BOARD_Init();


/**
 * Function: BOARD_End(void)
 * @param None
 * @return None
 * @brief shuts down all peripherals except for serial and A/D. Turns all pins
 * into input
 * @author Max Dunne, 2013.09.20  */
void BOARD_End();

/**
 * Function: BOARD_GetPBClock(void)
 * @param None
 * @return PB_CLOCK - speed the peripheral clock is running in hertz
 * @brief returns the speed of the peripheral clock.  Nominally at 40Mhz
 * @author Max Dunne, 2013.09.01  */
unsigned int BOARD_GetPBClock();


#endif	/* BOARD_H */

