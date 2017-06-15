#ifndef BB_CONTROLLER_H
#define BB_CONTROLLER_H

/* 
 * File:   BB_BOARD.h
 * Author: 
 * 
 * Inspired by https://www.embeddedrelated.com/showcode/346.php
 *
 * Created on February 2, 2017, 12:10 AM
 */

/*******************************************************************************
 * PUBLIC #INCLUDES                                                             *
 ******************************************************************************/
#include "BB_BOARD.h"
#include "BB_Motor.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#define JC03            IOPORT_G, BIT_1
#define PRESCALE        64
#define TIMER4_FREQ     100    // this is the frequency the tipping controller will run at
#define T4_PERIOD       (SYS_FREQ/PB_DIV/PRESCALE/TIMER4_FREQ)
#define TIP_CTL_SAMPLE_TIME     (1.0/(double)TIMER4_FREQ)
#define TIMER5_FREQ     1000    // this is the frequency the motor controller will run at
#define T5_PERIOD       (SYS_FREQ/PB_DIV/PRESCALE/TIMER5_FREQ)
#define MOTOR_CTL_SAMPLE_TIME     (1.0/(double)TIMER5_FREQ)
#define MAX_ANGLE       10  // max angle passed into linear (outer) controller
#define PRINT_DATA_SIZE (3000)

/*******************************************************************************
 * PUBLIC Variables                                                             *
 ******************************************************************************/

/** PI control data structure.  This structure contains configuration (the
    proportional and integral gain, plus a final divisor), output limits, and
    an integration accumulator (the PI controller's state variable). */
typedef struct PIDControler {
    double kp; /**< Proportional gain constant */
    double ki; /**< Integral gain constant */
    double kd; /**< Derivative gain constant */
    double error;
    double lastErr;
    double input; // encoder reading
    double output;
    double reference; // setpoint, must be written to
    double lastRef; // last reference
    double eIntegral;
    double eDerivative;
    double lastInput;
    double uP;
    double uI;
    double uD;

} PIDControl;

typedef struct PrintData {
    BOOL ready2print;
    int count;
    double angleX;
    double angleY;
    double thetaOutX;
    double thetaOutY;
    double gyroX;
    double gyroY;
    double omegaOutX;
    double omegaOutY;
    double encoderX;
    double encoderY;
    double m1Speed[PRINT_DATA_SIZE];
    double m2Speed;
    double m3Speed;
    double m1Cmd[PRINT_DATA_SIZE];
    double m2Cmd;
    double m3Cmd;
    double m1Output[PRINT_DATA_SIZE];
    double m2Output;
    double m3Output;
    double error[PRINT_DATA_SIZE];
    double uP[PRINT_DATA_SIZE];
    double uI[PRINT_DATA_SIZE];
    double uD[PRINT_DATA_SIZE];
} PrintData;

typedef enum {
    reset = 'r',
    balancing = 'b',
    stop = ' '
} Status_t;

typedef enum {
    linear = 'l',
    theta = 't',
    omega = 'w'
} ControllerLevel_t;

typedef enum {
    proportional = 'p',
    integral = 'i',
    derivative = 'd'
} PID_t;

extern volatile PIDControl linearX;
extern volatile PIDControl linearY;
extern volatile PIDControl thetaX;
extern volatile PIDControl thetaY;
extern volatile PIDControl omegaX;
extern volatile PIDControl omegaY;

extern volatile PrintData printData;

extern volatile BOOL printFlag;

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @Function PID_ThetaUpdate
 * @param struct PIControl *p,
 * @return int
 * @brief  
 * @author M*/
void PID_ThetaUpdate(volatile PIDControl *p, double sensorInput, double reference, int maxOut, double eDerivative);

/**
 * @Function PID_OmegaUpdate
 * @param struct PIControl *p,
 * @return int
 * @brief  
 * @author M*/
void PID_OmegaUpdate(volatile PIDControl *p, double sensorInput, double reference, int maxOut);

/**
 * Proportional-integral (PI) control law.
 *
 * @param[in,out]  p    control parameter and state structure
 *
 * @note p->eIntegral is A_k from Gabe's Drawing
 * 
 * @return              control output <code>u</code>
 */
void PID_MotorUpdate(volatile PIDControl *p, double sensorInput, double reference, int maxOut);

/**
 * @Function int PID_SetReference (struct PIControl *p);
 * @param struct PIControl *p,
 * @return int
 * @brief  
 * @author M*/
void PID_SetReference(volatile PIDControl *p, double refDesired);

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
void PID_SetTune(volatile PIDControl *p, double Kp, double Ki, double Kd);

/**
 * @Function int pid_control_init (struct PIControl *p, int e);
 * @param struct PIControl *p,
 * @return int
 * @brief  
 * @author M*/
void PID_Init(volatile PIDControl *p, BOOL firstInit, double sensorInput, double kp, double ki, double kd);

/**
 * @Function PID_SetAngleOffset(void)
 * @param   
 * @return 
 * @brief
 * @note 
 * @author  */
void PID_SetAngleOffset(double xOffset, double yOffset);

/**
 * @Function PID_Print(void)
 * @param   *p - pointer to motor controller struct
 * @return none
 * @brief
 * @note 
 * @author  */
void PID_Print(volatile PIDControl *p);

/**
 * @Function PID_PrintK(void)
 * @param   *p - pointer to motor controller struct
 * @return none
 * @brief
 * @note 
 * @author  */
void PID_PrintK(volatile PIDControl *p);

#endif /* _PI_CONTROL_H */



