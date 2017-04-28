/* BB_Motor.c
 * 
 * Pavlo Vlastos
 * 
 * Created February 22, 2017, 3:07PM
 * 
 */

/*******************************************************************************
 * PRIVATE #INCLUDES                                                           *
 ******************************************************************************/

#include "BB_BOARD.h"

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/**
 * Function: Leds_Init()
 * @param 
 * @return 
 * @brief Set and clears IO pins for the four LEDs on the Mx7
 **/
void Leds_Init()
{
    PORTSetPinsDigitalOut(IOPORT_G, BIT_12 | BIT_13 | BIT_14 | BIT_15);
    PORTClearBits(IOPORT_G, BIT_12 | BIT_13 | BIT_14 | BIT_15);

}

/**
 * Function: Turn_On_LED()
 * @param   portId, outputs - pass BB_LED_x to satisfy both parameters
 *      x is 1, 2, 3, or 4. See top of this h file
 * @return 
 * @brief Turn on specified LED
 **/
void Turn_On_LED(IoPortId portId, unsigned int outputs)
{
    PORTSetBits(portId, outputs);
}

/**
 * Function: Turn_Off_LED()
 * @param   portId, outputs - pass BB_LED_x to satisfy both parameters
 *      x is 1, 2, 3, or 4. See top of this h file
 * @return  None
 * @brief   Turn off specified LED
 **/
void Turn_Off_LED(IoPortId portId, unsigned int outputs)
{
    PORTClearBits(portId, outputs);
}

/**
 * Function: Turn_Off_All_LED()
 * @param   portId, outputs - pass BB_LED_x to satisfy both parameters
 *      x is 1, 2, 3, or 4. See top of this h file
 * @return  None
 * @brief   Turn off specified LED
 **/
void Turn_Off_All_LED() {
    Turn_Off_LED(BB_LED_1);
    Turn_Off_LED(BB_LED_2);
    Turn_Off_LED(BB_LED_3);
    Turn_Off_LED(BB_LED_4);
}

/**
 * Function: Toggle_LED()
 * @param   portId, outputs - pass BB_LED_x to satisfy both parameters
 *      x is 1, 2, 3, or 4. See top of this h file
 * @return  None
 * @brief   Toggle specified LED
 **/
void Toggle_LED(IoPortId portId, unsigned int outputs)
{
    PORTToggleBits(portId, outputs);
}
/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
