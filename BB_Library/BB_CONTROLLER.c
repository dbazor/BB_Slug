/*
 * File:   BB_CONTROLLER.c
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
#include "BB_CONTROLLER.h"
#include <stdbool.h>
#include "BB_BOARD.h"
#include <plib.h>
#include "BB_Motor.h"
#include "BB_Encoder.h"
#include "BB_QUAT.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/


/*******************************************************************************
 * Private Support Functions                                                   *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC VARIABLES                                                            *
 ******************************************************************************/
volatile BOOL loopFlag = FALSE;
volatile PIDControl thetaX;
volatile PIDControl thetaY;
volatile PIDControl omegaX;
volatile PIDControl omegaY;

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

    // Set pin high
    PORTSetBits(JC03);

    static Quat q, result;

    // 1) Get most recent data from IMU
    IMU_Read_Quat();
    IMU_Get_Quat(&q);
    BB_Quat_Tip_Vector(&q, &result);
    float xAngle = BB_Quat_Find_Tip_Angle_X(&result); // in degrees
    float yAngle = BB_Quat_Find_Tip_Angle_Y(&result); // in degrees
    printf("x angle = %f, y angle = %f\n", xAngle, yAngle);
    
    // 2) Run outer controller
    PID_Update(&thetaX, xAngle, 0);
    PID_Update(&thetaY, yAngle, 0);

    // 3) Run inner controller
    //    IMU_Read_Gyro();
    //    PID_Update(&omegaX,IMU_Get_Gyro_Y(), thetaX.uPWM);
    //    PID_Update(&omegaY,IMU_Get_Gyro_X(), thetaY.uPWM);

    // 4) Set motors
    //    SetMotor_XYZ(omegaX.uPWM, omegaY.uPWM, 0);
    SetMotor_XYZ(thetaX.uPWM, thetaY.uPWM, 0);
    printf("thetaX.uPWM = %f, thetaY.uPWM = %f", thetaX.uPWM, thetaY.uPWM);

    //    PID_Update(&motor1_pid);
    //    SetMotorSpeed(motor1_pid.uPWM, motor1_pid.motorNum);

    loopFlag = TRUE;

    // these are just to check the frequency
    //    Turn_On_LED(BB_LED_4);
    //    PORTToggleBits(JC03); // for oscilloscope frequency check
    //    

    // Set pin low
    PORTClearBits(JC03);

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
 *
 * @note p->eIntegral is A_k from Gabe's Drawing
 * 
 * @return              control output <code>u</code>
 */
void PID_Update(volatile PIDControl *p, float sensorInput, float reference)
{
    //printf("Starting update\n");
    p->reference = reference;
    // Reset integral state if reference changes sign
    if (((p->reference > 0) && (p->lastRef < 0)) || ((p->reference < 0) && (p->lastRef > 0))) {
        p->eIntegral = 0;
    }

    // Get the current sensor reading
    p->input = sensorInput;
    //printf("input found\n");

    /*Compute all the working error variables*/
    p->error = p->reference - p->input; // Error = r - senor
    p->eIntegral += (SAMPLE_TIME * p->error); // Calculate the i-term
    //printf("Error Calculated\n");

    double uP = p->kp * p->error;
    double uI = (p->ki * p->eIntegral); // temp u integral
    double uD = (p->kd * (p->lastInput - p->input)) / SAMPLE_TIME; //
    //printf("U Calculated\n");

    /*Compute PID Output*/
    p->uPWM = uP + uI + uD; // sets output to motor but doesn't set motor

    if ((p->uPWM > MAX_PWM) || (p->uPWM < MIN_PWM)) {
        p->eIntegral -= (SAMPLE_TIME * p->error); // undo integration 
        p->uPWM = uP + uD; // reset output to motor
    }
    //printf("Output normalized\n");

    /*Remember some variables for next time*/
    p->lastInput = p->input;
    p->lastRef = p->reference;

}

/**
 * @Function int PID_SetReference (struct PIControl *p);
 * @param struct PIControl *p,
 * @return int
 * @brief  
 * @author M*/
void PID_SetReference(volatile PIDControl *p, double refDesired)
{
    p->reference = refDesired;
}

/**
 * @Function SetTunings(void)
 * @param   *p - pointer to motor controller struct
 *          Kp - proportional constant, range: (1 - 4e6)
 *          Ki - integral constant,     range: (2 - 858e6)
 *          Kd - derivative constant,   range: (5 - 2147)
 * @return none
 * @brief
 * @note 
 * @author  */
void PID_SetTune(volatile PIDControl *p, double Kp, double Ki, double Kd)
{
    //    float SampleTimeInSec = (SAMPLE_TIME * 1000.0);
    p->kp = Kp;
    p->ki = Ki;
    p->kd = Kd;
}

/**
 * Initializes the PI control.
 *
 * This function resets the PI integrator to zero.
 *
 * @param[in,out]  p  control parameter structure
 */
void PID_Init(volatile PIDControl *p, BOOL firstInit, float sensorInput, double kp, double ki, double kd)
{
    DisableIntT4;
    if (firstInit) { // first init

        PID_SetTune(p, kp, ki, kd);
        p->error = 0.0;
        p->input = sensorInput;
        p->uPWM = 0; // control effort
        p->reference = 0; // setpoint, must be written to
        p->lastRef = 0;
        p->eIntegral = 0;
        p->lastInput = 0;


    } else { // not first init
        p->lastInput = sensorInput;
        p->eIntegral = p->uPWM;
        // Normalize output to + or - MAX_PWM
        if (p->eIntegral > MAX_PWM) {
            p->eIntegral = MAX_PWM;
        } else if (p->eIntegral < MIN_PWM) {
            p->eIntegral = MIN_PWM;
        }
    }
    EnableIntT4;
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
void PID_Print(volatile PIDControl *p)
{
    printf("\n");
    printf("kp:         %e\n", p->kp);
    printf("ki:         %e\n", p->ki);
    printf("kd:         %e\n", p->kd);
    printf("error:      %e\n", p->error);
    printf("input:      %e\n", p->input);
    printf("output:     %d\n", p->uPWM);
    printf("reference:  %e\n", p->reference);
    printf("integralOut:%e\n", p->eIntegral);
    printf("lastInput:  %e\n", p->lastInput);
}

