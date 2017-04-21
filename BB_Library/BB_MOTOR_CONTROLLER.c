/*
 * File:   BB_MOTOR_CONTROLLER.c
 * Author: BB Team
 *
 * Inspired by https://www.embeddedrelated.com/showcode/346.php
 * 
 * Created on April 21, 2017, 2:08 PM
 
 * @file
 * Proportional-integral (PI) control law.
 *
 * This module implements a simple position-type PI controller:
 * <pre>
 *   u = [ kp * e + ki * sum(e) ] >> shift
 * </pre>
 * <tt>shift</tt> is a right bit shift used to scale the output of the
 * controller down from the 32-bit intermediate result.
 *
 * An anti-windup provision is implemented on the PI integrator to prevent
 * deep saturation (aka integrator windup):
 * - The new control output with the latest integrator value is computed.
 * - If the control output exceeds either output limit, <i>and</i> the latest
 *   change in the integrator is in the same direction, then the new integrator
 *   value is not saved for the next call.
 * - Otherwise, the integrator is saved for the next call.
 */

/*******************************************************************************
 * PRIVATE #includes                                                           *
 ******************************************************************************/
#include "BB_MOTOR_CONTROLLER.h"
#include <stdbool.h>
#include "BB_BOARD.h"
#include <plib.h>
#include "BB_Motor.h"
#include "BB_Encoder.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/


/*******************************************************************************
 * Private Support Functions                                                   *
 ******************************************************************************/


/*******************************************************************************
 * Interrupts                                                                  *
 ******************************************************************************/

/*******************************************************************************
  Function:
    Timer4Handler(void)

  Summary:
    Timer4 ISR for motor controller

  Description:
    Called at a frequency defined by TIMER4_FREQ to run the motor controller 
    periodically.

  Precondition:
    Setup in BB_Controller_INIT eventually

  Remarks:
    None
 *****************************************************************************/
void __ISR(_TIMER_4_VECTOR, IPL2SOFT) Timer4Handler(void)
{
    // clear the interrupt flag always
    mT4ClearIntFlag();

    // these are just to check the frequency
    Toggle_LED(BB_LED_4);
    PORTToggleBits(JC03);   // for oscilloscope frequency check

    
    
    // Now to test the controller
    //eCountRadians = GetEncoderRadians(MOTOR_1);
}

/*******************************************************************************
 * Functions                                                                   *
 ******************************************************************************/

/**
 * Proportional-integral (PI) control law.
 *
 * @param[in,out]  p    control parameter and state structure
 * @param[in]      e    error signal
 *
 * @return              control output <code>u</code>
 */
void pid_control(struct PIDControl *p, int e)
{
    /*Compute all the working error variables*/
    p->error = p->referrence - p->input; // Error = r - senor
    p->integralOut += (p->ki * p->error); // Calculate the i-term

    // if ITerm just calculated is greater than the max that it is allowed to be
    //      ITerm = outMax
    // else if Iterm is greater than outmin
    //      ITerm = outMin
    if (p->integralOut > MAX_PWM) {
        p->integralOut = MAX_PWM;
    } else if (p->integralOut < MIN_PWM) {
        p->integralOut = MIN_PWM;
    }
    UINT64 dInput = (p->input - p->lastInput);        // change in the input

    /*Compute PID Output*/
    p->output = p->kp * p->error + p->integralOut - p->kd * dInput;  // sets output
    if (p->output > MAX_PWM) {
        p->output = MAX_PWM;
    } else if (p->output < MIN_PWM) {
        p->output = MIN_PWM;
    }

    /*Remember some variables for next time*/
    p->lastInput = p->input;
}

/**
 * Initializes the PI control.
 *
 * This function resets the PI integrator to zero.
 *
 * @param[in,out]  p  control parameter structure
 */
void pid_control_init(struct PIDControl *p)
{
   p->lastInput = GetEncoderCount();
   ITerm = Output;
   if(ITerm> outMax) ITerm= outMax;
   else if(ITerm< outMin) ITerm= outMin;
}

/**
 * @Function SetTunings(void)
 * @param   *p - pointer to motor controller struct
 *          Kp - proportional constant, range: (1 - 4e6)
 *          Ki - integral constant,     range: (1 - 858e6)
 *          Kd - derivative constant,   range: (5 - 2147)
 * @return none
 * @brief
 * @note 
 * @author  */
void SetTunings(struct PIDControl *p, UINT32 Kp, UINT32 Ki, UINT32 Kd)
{
    UINT32 SampleTimeInSec = (SAMPLE_TIME * 1000);
    p->kp = Kp;
    p->ki = Ki * SampleTimeInSec;
    p->kd = Kd / SampleTimeInSec;
}
