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
 * PUBLIC VARIABLES                                                             *
 ******************************************************************************/
typedef struct encodeVal {
    double x;
    double y;
    double rot; // rotation
} encodeVal;

/*******************************************************************************
 * FUNCTION PROTOYPES                                                          *
 ******************************************************************************/

//interrupt void InputCapture2_ISR(void);
/**
 * Function: Encoder_Init(void)
 * @param None
 * @return None
 * @brief Sets input capture pins and IO pins to read encoder signals
 *      Also sets up IC2, IC3, and IC5 to all use Timer 3. Sets to captuer
 *      on falling and rising edge. See function for more settings initialized
 * @author Pavlo Vlastos */
void Encoder_Init(void);

/**
 * Function: GetEncoderCount(UINT8 motorNum)
 * @param UINT8 motorNum
 * @return Integer representing Encoder Count
 * @brief Get the encoder count of a specified motor
 **/
int GetEncoderCount(UINT8 motorNum);

/**
 * Function: GetEncoderRadians(UINT8 motorNum)
 * @param UINT8 motorNum
 * @return Float representing Encoder Count in radians
 * @brief Get the encoder count in radians of a specified motor
 **/
float GetEncoderRadians(UINT8 motorNum);

/**
 * Function: GetEncoderXYZ
 * @param 
 * @return 
 * @brief
 **/
void GetEncoderXYZ(encodeVal *e);

/**
 * Function: SetEncoderCount(UINT8 motorNum, UINT8 value)
 * @param UINT8 motorNum, UINT8 value
 * @return None
 * @brief Set the encoder count of a specified motor, to a desired value
 **/
void SetEncoderCount(UINT8 motorNum, UINT8 value);

/**
 * Function: SetEncoderCounts
 * @param 
 * @return None
 * @brief Set the encoder count of all motors to desired values
 **/
void SetEncoderCounts(int count1, int count2, int count3);

#endif	/* BB_ENCODER_H */ 