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
#include "BB_IMU.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define AVERAGE_SIZE 5

/*******************************************************************************
 * Private Support Functions                                                   *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC VARIABLES                                                            *
 ******************************************************************************/
volatile BOOL loopFlag = FALSE;
volatile PIDControl linearX;
volatile PIDControl linearY;
volatile PIDControl thetaX;
volatile PIDControl thetaY;
volatile PIDControl omegaX;
volatile PIDControl omegaY;
volatile PIDControl motorCtlr1;
volatile PIDControl motorCtlr2;
volatile PIDControl motorCtlr3;
volatile PrintData printData;
volatile MotorSpeedsCmd motorSpeedsCmd; // currently in rad/sec
double xAngleOffset;
double yAngleOffset;
volatile BOOL printFlag;
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
void __ISR(_TIMER_4_VECTOR, IPL5SOFT) Timer4Handler(void)
{
    // clear the interrupt flag always
    mT4ClearIntFlag();

    static double averageX[AVERAGE_SIZE], averageY[AVERAGE_SIZE];
    static int index = 0;
    static double sumX = 0, sumY = 0;

    // Set pin high
    PORTSetBits(JC03);

    static int count = 0;
    static Quat q, result;
    static gyroAxis g;
    static encodeVal encoder;

    // 0)
    GetEncoderXYZ(&encoder);
    //PID_Update(&linearX, encoder.x, 0, MAX_ANGLE);
    //PID_Update(&linearY, encoder.y, 0, MAX_ANGLE);

    // 1) Get most recent data from IMU
    IMU_Read_Quat();
    IMU_Get_Quat(&q);
    BB_Quat_Tip_Vector(&q, &result);
    double angleX = BB_Quat_Find_Tip_Angle_X(&result) - xAngleOffset; // in rad
    double angleY = BB_Quat_Find_Tip_Angle_Y(&result) - yAngleOffset; // in rad

    IMU_Read_Gyro();
    IMU_Get_Gyro(&g);    
    // low pass the gyro
    double gyroX = g.x;
    double gyroY = g.y;
    // Rolling average of size AVERAGE_SIZE
    sumX -= averageX[index]; // subtract out oldest value
    sumY -= averageY[index];
    sumX += gyroX; // add in newest value
    sumY += gyroY;
    averageX[index] = gyroX; // replace oldest value with newest value
    averageY[index] = gyroY;
    index++;
    index %= AVERAGE_SIZE;
    double gyroAverageX = sumX / AVERAGE_SIZE;
    double gyroAverageY = sumY / AVERAGE_SIZE;

    // 2) Run middle controller
    //    PID_ThetaUpdate(&thetaX, xAngle, linearX.output, MAX_PWM, gyroY);
    //    PID_ThetaUpdate(&thetaY, yAngle, linearY.output, MAX_PWM, gyroX);
    PID_ThetaUpdate(&thetaX, angleX, 0, MAX_RAD_PER_SEC, gyroAverageY);
    PID_ThetaUpdate(&thetaY, angleY, 0, MAX_RAD_PER_SEC, gyroAverageX);

    // 3) Run inner controller
    PID_OmegaUpdate(&omegaX, gyroAverageY, thetaX.output, MAX_RAD_PER_SEC);
    PID_OmegaUpdate(&omegaY, gyroAverageX, thetaY.output, MAX_RAD_PER_SEC);





    // 4) Set motors    
    MotorSet_XYZ(omegaX.output, omegaY.output, 0); // this now just sets global variables for the three motor controllers to try to achieve in rad/sec.
    //MotorSet_XYZ(omegaX.output, omegaY.output, 0); 

    //MotorSet_XYZ(thetaX.output, thetaY.output, 0);   // for testing only middle controller


    count++;
    //if (count % 50 == 0 && printFlag) {
    if (printFlag) {
        printData.ready2print = TRUE;
        printData.count = count;
        printData.angleX = angleX;
        printData.angleY = angleY;
        printData.thetaOutX = thetaX.output;
        printData.thetaOutY = thetaY.output;
        printData.gyroX = gyroX;
        printData.gyroY = gyroY;
        printData.omegaOutX = omegaX.output;
        printData.omegaOutY = omegaY.output;
        printData.encoderX = encoder.x;
        printData.encoderY = encoder.y;

        //        printf("Encoder1: %d, Encoder2: %d, Encoder3: %d EncoderX: %f, EncoderY: %f, EncoderRot: %f\n", 
        //                GetEncoderCount(MOTOR_1), GetEncoderCount(MOTOR_2), GetEncoderCount(MOTOR_3), 
        //                encoder.x, encoder.y, encoder.rot);
        // print for MatLab 
        //        printf("%d, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n",
        //                count, angleX, angleY, thetaX.output, thetaY.output, gyroX, gyroY,
        //                omegaX.output, omegaY.output, omegaXoutputAverage, omegaYoutputAverage,
        //                encoder.x, encoder.y);
        //        printf("\n\nCount: %d\nencoder.x = %f, encoder.y = %f\n", count, encoder.x, encoder.y);
        //        printf("linaerX.output = %f, linaerY.output = %f\n", linearX.output, linearY.output);
        //        printf("thetaX.uPWM = %f, thetaY.uPWM = %f\n", thetaX.output, thetaY.output);
        //        printf("omegaX.uPWM = %f, omegaY.uPWM = %f\n", omegaX.output, omegaY.output);
        //        printf("\ngyroX: %f, gyroY: %f\n", gyroX, gyroY);
        //        printf("x angle = %f, y angle = %f\n", xAngle, yAngle);
        //    printf("xAngle: %f, gyroY: %f\n", xAngle, gyroY);
    }

    //    PID_Update(&motor1_pid);
    //    SetMotorSpeed(motor1_pid.uPWM, motor1_pid.motorNum);

    // these are just to check the frequency
    //    Turn_On_LED(BB_LED_4);
    //    PORTToggleBits(JC03); // for oscilloscope frequency check
    //    

    // Set pin low
    PORTClearBits(JC03);

    // Now to test the controller
    //eCountRadians = GetEncoderRadians(MOTOR_1);
}

// controller to maintain correct rate on each motor
// since this is a rate controller it may need feed-forward

void __ISR(_TIMER_5_VECTOR, IPL4SOFT) Timer5Handler(void)
{
    static encodeVal encoder;
    static double prevEncoder1 = 0, prevEncoder2 = 0, prevEncoder3 = 0;
    // clear the interrupt flag always
    mT4ClearIntFlag();
    // get newest values for desired motor speeds in rad/sec

    // get current values of the encoders in rad/sec
    GetEncoderRadians(&encoder);
    
    // convert to speed in rad/sec Might need to take an average to filter out noise                need to change to get three global velocities from encoder
    double motorSpeed1 = (encoder.m1 - prevEncoder1) / MOTOR_CTL_SAMPLE_TIME;
    double motorSpeed2 = (encoder.m2 - prevEncoder2) / MOTOR_CTL_SAMPLE_TIME;
    double motorSpeed3 = (encoder.m3 - prevEncoder3) / MOTOR_CTL_SAMPLE_TIME; 
    
    prevEncoder1 = encoder.m1;
    prevEncoder2 = encoder.m2;
    prevEncoder3 = encoder.m3;
    
    // run update on each
    PID_MotorUpdate(&motorCtlr1, motorSpeed1, motorSpeedsCmd.m1, MAX_RAD_PER_SEC);
    PID_MotorUpdate(&motorCtlr2, motorSpeed2, motorSpeedsCmd.m2, MAX_RAD_PER_SEC);
    PID_MotorUpdate(&motorCtlr3, motorSpeed3, motorSpeedsCmd.m3, MAX_RAD_PER_SEC);
    
    // set motors to new PWM
    MotorSetSpeed((int)(RAD_PER_SEC_2_PWM*motorCtlr1.output), MOTOR_1);
    MotorSetSpeed((int)(RAD_PER_SEC_2_PWM*motorCtlr2.output), MOTOR_2);
    MotorSetSpeed((int)(RAD_PER_SEC_2_PWM*motorCtlr3.output), MOTOR_3);
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
void PID_ThetaUpdate(volatile PIDControl *p, double sensorInput, double reference, int maxOut, double eDerivative)
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
    p->eIntegral += (TIP_CTL_SAMPLE_TIME * p->error); // Calculate the i-term
    p->eDerivative = -eDerivative; // was / SAMPLE_TIME; because gyro is already rate
    //printf("Error Calculated\n");

    double uP = p->kp * p->error;
    double uI = p->ki * p->eIntegral; // temp u integral
    double uD = p->kd * p->eDerivative; //
    //printf("U Calculated\n");

    /*Compute PID Output*/
    p->output = uP + uI + uD; // sets output to motor but doesn't set motor

    if ((p->output > maxOut) || (p->output < -maxOut)) {
        p->eIntegral -= (TIP_CTL_SAMPLE_TIME * p->error); // undo integration 
        p->output -= uI; // reset output to motor
    }
    //printf("Output normalized\n");

    /*Remember some variables for next time*/
    p->lastInput = p->input;
    p->lastRef = p->reference;

}

/**
 * Proportional-integral (PI) control law.
 *
 * @param[in,out]  p    control parameter and state structure
 *
 * @note p->eIntegral is A_k from Gabe's Drawing
 * 
 * @return              control output <code>u</code>
 */
void PID_OmegaUpdate(volatile PIDControl *p, double sensorInput, double reference, int maxOut)
{
    //printf("Starting update\n");
    p->reference = reference;
    // Reset integral state if reference changes sign
    if (((p->reference > 0.0) && (p->lastRef < 0.0)) || ((p->reference < 0.0) && (p->lastRef > 0.0))) {
        p->eIntegral = 0.0;
    }

    // Get the current sensor reading
    p->input = sensorInput;
    //printf("input found\n");

    /*Compute all the working error variables*/
    p->error = p->reference - p->input; // Error = r - sensor
    p->eIntegral += (TIP_CTL_SAMPLE_TIME * p->error); // Calculate the i-term
    p->eDerivative = -(p->input - p->lastInput) / TIP_CTL_SAMPLE_TIME; // the minus here is important
    //printf("Error Calculated\n");

    double uP = p->kp * p->error;
    double uI = p->ki * p->eIntegral; // temp u integral
    double uD = p->kd * p->eDerivative;
    //printf("U Calculated\n");

    /*Compute PID Output*/
    p->output = uP + uI + uD; // sets output to motor but doesn't set motor

    if ((p->output > maxOut) || (p->output < -maxOut)) {
        p->eIntegral -= (TIP_CTL_SAMPLE_TIME * p->error); // undo integration 
        p->output -= uI; // reset output to motor
    }
    //printf("Output normalized\n");

    /*Remember some variables for next time*/
    p->lastInput = p->input;
    p->lastRef = p->reference;

}

/**
 * Proportional-integral (PI) control law.
 *
 * @param[in,out]  p    control parameter and state structure
 *
 * @note p->eIntegral is A_k from Gabe's Drawing
 * 
 * @return              control output <code>u</code>
 */
void PID_MotorUpdate(volatile PIDControl *p, double sensorInput, double reference, int maxOut)
{
    //printf("Starting update\n");
    p->reference = reference;
    // Reset integral state if reference changes sign
    if (((p->reference > 0.0) && (p->lastRef < 0.0)) || ((p->reference < 0.0) && (p->lastRef > 0.0))) {
        p->eIntegral = 0.0;
    }

    // Get the current sensor reading
    p->input = sensorInput;
    //printf("input found\n");

    /*Compute all the working error variables*/
    p->error = p->reference - p->input; // Error = r - sensor
    p->eIntegral += (MOTOR_CTL_SAMPLE_TIME * p->error); // Calculate the i-term
    p->eDerivative = -(p->input - p->lastInput) / MOTOR_CTL_SAMPLE_TIME; // the minus here is important
    //printf("Error Calculated\n");

    double uP = p->kp * p->error;
    double uI = p->ki * p->eIntegral; // temp u integral
    double uD = p->kd * p->eDerivative;
    //printf("U Calculated\n");

    /*Compute PID Output*/
    p->output = uP + uI + uD; // sets output to motor but doesn't set motor

    if ((p->output > maxOut) || (p->output < -maxOut)) {
        p->eIntegral -= (MOTOR_CTL_SAMPLE_TIME * p->error); // undo integration 
        p->output -= uI; // reset output to motor
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
    //    double SampleTimeInSec = (SAMPLE_TIME * 1000.0);
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
void PID_Init(volatile PIDControl *p, BOOL firstInit, double sensorInput, double kp, double ki, double kd)
{
    //DisableIntT4;
    if (firstInit) { // first init

        PID_SetTune(p, kp, ki, kd);
        p->error = 0.0;
        p->input = sensorInput;
        p->output = 0; // control effort
        p->reference = 0; // setpoint, must be written to
        p->lastRef = 0;
        p->eIntegral = 0;
        p->lastInput = 0;


    } else { // not first init
        p->lastInput = sensorInput;
        p->eIntegral = p->output;
        // Normalize output to + or - MAX_PWM
        if (p->eIntegral > MAX_PWM) {
            p->eIntegral = MAX_PWM;
        } else if (p->eIntegral < MIN_PWM) {
            p->eIntegral = MIN_PWM;
        }
    }
    //EnableIntT4;
}

/**
 * @Function PID_SetAngleOffset(void)
 * @param   
 * @return 
 * @brief
 * @note 
 * @author  */
void PID_SetAngleOffset(double xOffset, double yOffset)
{
    xAngleOffset = xOffset;
    yAngleOffset = yOffset;
}

/**
 * @Function PID_Print(void)
 * @param   *p - pointer to motor controller struct
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
    printf("output:     %d\n", p->output);
    printf("reference:  %e\n", p->reference);
    printf("integralOut:%e\n", p->eIntegral);
    printf("lastInput:  %e\n", p->lastInput);
}

/**
 * @Function PID_PrintK(void)
 * @param   *p - pointer to motor controller struct
 * @return none
 * @brief
 * @note 
 * @author  */
void PID_PrintK(volatile PIDControl *p)
{
    printf("kp: %g, ki: %g, kd: %g", p->kp, p->ki, p->kd);
}

