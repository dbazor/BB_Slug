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
void Leds_Init()
{
    PORTSetPinsDigitalOut(IOPORT_G, BIT_12 | BIT_13 | BIT_14 | BIT_15);
    PORTClearBits(IOPORT_G, BIT_12 | BIT_13 | BIT_14 | BIT_15);

}

void Turn_On_LED(IoPortId portId, unsigned int outputs)
{
    PORTSetBits(portId, outputs);
}

void Turn_Off_LED(IoPortId portId, unsigned int outputs)
{
    PORTClearBits(portId, outputs);
} 
void Toggle_LED(IoPortId portId, unsigned int outputs){ 
    PORTToggleBits( portId, outputs);
}
/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
