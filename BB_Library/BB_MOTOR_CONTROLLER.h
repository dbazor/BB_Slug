#ifndef BB_MOTOR_CONTROLLER_H
#define BB_MOTOR_CONTROLLER_H

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
#define TIMER4_FREQ     100    // this is the frequency the motor controller will run at
#define T4_PERIOD       (SYS_FREQ/PB_DIV/PRESCALE/TIMER4_FREQ)
#define SAMPLE_TIME     (1.0/(float)TIMER4_FREQ)
#define MOTOR1_KP       1   // Kp - proportional constant, range: (1 - 4e6)     // fix
#define MOTOR1_KI       0   // Ki - integral constant,     range: (2 - 858e6)   // fix
#define MOTOR1_KD       0   // Kd - derivative constant,   range: (5 - 2147)    // fix

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
    double input; // encoder reading
    INT32 uPWM; // control effort
    double reference; // setpoint, must be written to
    double lastRef; // last reference
    double eIntegral;
    double lastInput;
    UINT8 motorNum;

} PIDControl;

extern volatile PIDControl motor1_pid;
extern volatile PIDControl motor2_pid;
extern volatile PIDControl motor3_pid;

extern volatile BOOL loopFlag;

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @Function int pid_control (struct PIControl *p);
 * @param struct PIControl *p,
 * @return int
 * @brief  
 * @author M*/
void PID_Update(volatile PIDControl *p);

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
void SetTunings(volatile PIDControl *p, double Kp, double Ki, double Kd);

/**
 * @Function int pid_control_init (struct PIControl *p, int e);
 * @param struct PIControl *p,
 * @return int
 * @brief  
 * @author M*/
void PID_Init(volatile PIDControl *p, BOOL firstInit, UINT8 motorNum, double kp, double ki, double kd);

#endif /* _PI_CONTROL_H */



