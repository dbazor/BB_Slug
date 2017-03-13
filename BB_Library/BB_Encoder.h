#ifndef BB_ENCODER_H
#define	BB_ENCODER_H

/*******************************************************************************
 * PRIVATE #INCLUDES                                                            *
 ******************************************************************************/
#include "BB_BOARD.h"


/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define TMR3_ROLLOVER 0xFFFF    // defines rollover count for IC2?s 16-bit Timer3
// DOUBLE CHECK THIS! 



/*******************************************************************************
 * FUNCTION PROTOYPES                                                          *
 ******************************************************************************/

//interrupt void InputCapture2_ISR(void);
void Encoder_Init(void);
int GetEncoder1Count();
void SetEncoder1Count(int Value);
//void __attribute__( interrupt(IPL3SOFT), vector(_INPUT_CAPTURE_2_VECTOR)) InputCapture2();

#endif	/* BB_ENCODER_H */ 