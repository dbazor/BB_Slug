#ifndef BB_LEDS_H
#define	BB_LEDS_H

/*******************************************************************************
 * PRIVATE #INCLUDES                                                            *
 ******************************************************************************/
#include "BB_BOARD.h"
#include <plib.h>

/*******************************************************************************
 * #DEFINES                                                            *
 ******************************************************************************/
#define BB_LED_1 IOPORT_G, BIT_12
#define BB_LED_2 IOPORT_G, BIT_13
#define BB_LED_3 IOPORT_G, BIT_14
#define BB_LED_4 IOPORT_G, BIT_15

/*******************************************************************************
 * FUNCTION PROTOYPES                                                          *
 ******************************************************************************/

/**
 * Function: Leds_Init()
 * @param 
 * @return 
 * @brief Set and clears IO pins for the four LEDs on the Mx7
 **/
void Leds_Init();

/**
 * Function: Turn_On_LED()
 * @param   portId, outputs - pass BB_LED_x to satisfy both parameters
 *      x is 1, 2, 3, or 4. See top of this h file
 * @return 
 * @brief Turn on specified LED
 **/
void Turn_On_LED(IoPortId portId, unsigned int outputs);

/**
 * Function: Turn_Off_LED()
 * @param   portId, outputs - pass BB_LED_x to satisfy both parameters
 *      x is 1, 2, 3, or 4. See top of this h file
 * @return  None
 * @brief   Turn off specified LED
 **/
void Turn_Off_LED(IoPortId portId, unsigned int outputs);

/**
 * Function: Toggle_LED()
 * @param   portId, outputs - pass BB_LED_x to satisfy both parameters
 *      x is 1, 2, 3, or 4. See top of this h file
 * @return  None
 * @brief   Toggle specified LED
 **/
void Toggle_LED(IoPortId portId, unsigned int outputs);

#endif	/* BB_LEDS_H */ 