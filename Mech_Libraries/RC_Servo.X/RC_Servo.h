/*
 * File:   RC_Servo.h
 * Author: Elkaim
 *
 * Software module to drive up to 10 RC servos using the standard PWM signaling. The nominal
 * input for the servos is a PWM signal with a 20Hz frequency, and a high time from 1msec to
 * 2msec with 1.5msec being centered. Resolusion of module is 1uSec.
 *
 *                   ________________       50mSec period           _______________
 * _________________|  1.0 to 2.0 mS |_____________________________|  1.5mS center |_____
 *
 * The module must be started with the RC_Init() function. Each pin must be added to the module
 * using the RC_AddPin() function, and can be removed using the RC_RemovePin() function. When
 * done with the module, the module must be shut down using the RC_End() function.
 *
 * Pins are attached to the RC_Servo module using #defined RC_PORTxxx from the available
 * list of pins below (not all pins are available for RC servo use). All enabled pins will
 * start with a 1.5msec pulse width (centered).
 *
 * NOTE: This module uses TIMER4 for its internal timing and interrupts. Certain servos
 *       have a larger range and can be driven from 0.5mS to 2.5mse, in this case, change
 *       MINPULSE to 500 and MAXPULSE to 2500 below.  DO NOT do this with the CMPE118 servos,
 *       as this involves some risk.  You're welcome to try it with your own servos.
 *
 * RCSERVO_TEST (defined in the project) conditionally compiles the test harness for the code.
 * Make sure it is commented out for module useage.
 *
 * Created on December 7, 2011, 8:56 AM
 * Updated on July 24, 2013, 12:18 PM
 */

#ifndef RC_Servo_H
#define RC_Servo_H

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

#define MINPULSE 1000
#define MAXPULSE 2000

#define RC_PORTX03 0x001
#define RC_PORTX04 0x002
#define RC_PORTY06 0x004
#define RC_PORTY07 0x008
#define RC_PORTZ08 0x010
#define RC_PORTZ09 0x020
#define RC_PORTV03 0x040
#define RC_PORTV04 0x080
#define RC_PORTW07 0x100
#define RC_PORTW08 0x200


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @Function RC_Init(void)
 * @param none
 * @return SUCCESS or ERROR
 * @brief Initialize the RC module for use. Uses TIMER4 hardware and initializes
 *        the interrupt to occur every 50msec (20Hz)
 * @author Gabriel Hugh Elkaim, 2011.12.15 16:42 */
char RC_Init(void);

/**
 * @Function RC_AddPins(unsigned short int RCpins)
 * @param RCpins, use #defined RC_PORTxxx OR'd together for each RC servo pin
 * @return SUCCESS or ERROR
 * @brief Enables pins in argument and initializes all enabled RC servo pins to
 *        1.5mSec pulse width (0 degree deflection). Re-enabling a previously
 *        enabled pin returns ERROR. Pin will actually enable at the end of the
 *        50msec cycle.
 * @author Gabriel Hugh Elkaim, 2013.07.24 12:25 */
char RC_AddPins(unsigned short int RCpins);

/**
 * @Function RC_RemovePins(unsigned short int RCpins)
 * @param RCpins - use #defined RC_PORTxxx OR'd together for each RC servo pin
 * @return SUCCESS or ERROR
 * @brief Shuts down each designated pin and turns it into an INPUT. Returns ERROR
 *        if the pin is not currently enabled.
 * @author Gabriel Hugh Elkaim, 2013.07.24 12:25 */
char RC_RemovePins(unsigned short int RCpins);

/**
 * @Function RC_ListPins(void)
 * @param none
 * @return RCpins - the mask of the pins that are currently active
 * @brief Returns a short with a 1 in each bit for each of the RC_PORTxxx that
 *        is enabled. Returns 0x0000 if no pins are enabled.
 * @note To verify RC_PORTxxx is enabled, test (RC_PORTxxx && RcListPins()), if
 *       the result is FALSE, then the pin is currently disabled.
 * @author Gabriel Hugh Elkaim, 2013.07.25 16:05 */
unsigned short int RC_ListPins(void);

/**
 * @Function RcSetPulseTime(unsigned short int RCpin, unsigned short int pulseTime)
 * @param RCpin - use #defined RC_PORTxxx (only one)
 * @param pulseTime - pulse width in uSeconds from [1000 to 2000]
 * @return SUCCESS or ERROR
 * @brief Sets servo pulse to desired value from 1mSec to 2mSec in 1uSec increments.
 * @author Gabriel Hugh Elkaim, 2011.12.15 16:42 */
char RC_SetPulseTime(unsigned short int RCpin, unsigned short int pulseTime);

/**
 * @Function RcGetPulseTime(unsigned short int RCpin)
 * @param RCpin - use #defined RC_PORTxxx (only one)
 * @return pulseTime - Pulse width in uSeconds
 * @brief Gets the servo pulse width, provided for symmetry.
 * @author Gabriel Hugh Elkaim, 2011.12.15 16:42 */
short int RC_GetPulseTime(unsigned short int RCpin);

/**
 * @Function RC_ChangePending(void)
 * @param none
 * @return TRUE or FALSE
 * @brief returns TRUE if change in RC pins pending
 * @author Gabriel Hugh Elkaim, 2013.08.18 21:56 */
char RC_ChangePending(void);

/**
 * @Function RcEnd(void)
 * @param none
 * @return SUCCESS or ERROR
 * @brief Shuts down the RC servo module. Releases TIMER4 and shuts down all
 *        servo outputs.
 * @author Gabriel Hugh Elkaim, 2011.12.15 16:42 */
char RC_End(void);

#endif
