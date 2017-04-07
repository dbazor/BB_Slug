#ifndef BB_LEDS_H
#define	BB_LEDS_H

/*******************************************************************************
 * PRIVATE #INCLUDES                                                            *
 ******************************************************************************/
#include "BB_BOARD.h"
#include <plib.h>

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/



/*******************************************************************************
 * FUNCTION PROTOYPES                                                          *
 ******************************************************************************/
void Leds_Init();
void Turn_On_LED(IoPortId portId, unsigned int outputs);
void Turn_Off_LED(IoPortId portId, unsigned int outputs);
void Toggle_LED(IoPortId portId, unsigned int outputs);

#endif	/* BB_LEDS_H */ 