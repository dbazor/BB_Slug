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
#define TIMER4_FREQ     2000    // this is the frequency the motor controller will run at
#define T4_PERIOD       (SYS_FREQ/PB_DIV/PRESCALE/TIMER4_FREQ)
#define SAMPLE_TIME     (1.0/(float)TIMER4_FREQ)
#define MOTOR1_KP       1   // Kp - proportional constant, range: (1 - 4e6)
#define MOTOR1_KI       2   // Ki - integral constant,     range: (2 - 858e6)
#define MOTOR1_KD       5   // Kd - derivative constant,   range: (5 - 2147)

/*******************************************************************************
 * PUBLIC Variables                                                             *
 ******************************************************************************/

/** PI control data structure.  This structure contains configuration (the
    proportional and integral gain, plus a final divisor), output limits, and
    an integration accumulator (the PI controller's state variable). */
typedef struct PIDControler {
    UINT32 kp; /**< Proportional gain constant */
    UINT32 ki; /**< Integral gain constant */
    UINT32 kd; /**< Derivative gain constant */
    INT64 error;
    INT64 input;   // encoder reading
    INT64 output;  // control effort
    INT64 reference;  // setpoint, must be written to
    INT64 integralOut;
    INT64 lastInput;
    UINT8 motorNum;
    
}PIDControl;

extern volatile PIDControl motor1_pid;
extern volatile PIDControl motor2_pid;
extern volatile PIDControl motor3_pid;

extern volatile BOOL loopFlag;

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @Function int pid_control (struct PIControl *p, int e);
 * @param struct PIControl *p,
 * @return int
 * @brief  
 * @author M*/
void PID_Update(volatile PIDControl *p);
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
void SetTunings(volatile PIDControl *p, UINT32 Kp, UINT32 Ki, UINT32 Kd);

/**
 * @Function int pid_control_init (struct PIControl *p, int e);
 * @param struct PIControl *p,
 * @return int
 * @brief  
 * @author M*/
void PID_Init(volatile PIDControl *p, BOOL firstInit, UINT8 motorNum, UINT32 kp, UINT32 ki, UINT32 kd);

#endif /* _PI_CONTROL_H */



