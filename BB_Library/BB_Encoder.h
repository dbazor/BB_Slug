#ifndef BB_ENCODER_H
#define	BB_ENCODER_H

/*******************************************************************************
 * PRIVATE #INCLUDES                                                            *
 ******************************************************************************/
#include "BB_BOARD.h"
#include "BB_Motor.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define TMR3_ROLLOVER 0xFFFF    // defines rollover count for IC2?s 16-bit Timer3
// DOUBLE CHECK THIS! 

#define VOLT_2_PWM (MAX_PWM / MAX_VOLTAGE)



/*******************************************************************************
 * FUNCTION PROTOYPES                                                          *
 ******************************************************************************/

//interrupt void InputCapture2_ISR(void);
void Encoder_Init(void);
int GetEncoderCount(UINT8 motorNum);
float GetEncoderRadians(UINT8 motorNum);
void SetEncoderCount(UINT8 motorNum, UINT8 value);

#endif	/* BB_ENCODER_H */ 