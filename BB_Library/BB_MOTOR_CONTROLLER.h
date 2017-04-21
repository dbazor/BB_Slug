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
#define JC03 IOPORT_G, BIT_1
#define PRESCALE        64
#define TIMER4_FREQ     2000    // this is the frequency the motor controller will run at
#define T4_PERIOD       (SYS_FREQ/PB_DIV/PRESCALE/TIMER4_FREQ)
#define SAMPLE_TIME     (1/TIMER4_FREQ)

/*******************************************************************************
 * PUBLIC Variables                                                             *
 ******************************************************************************/

/** PI control data structure.  This structure contains configuration (the
    proportional and integral gain, plus a final divisor), output limits, and
    an integration accumulator (the PI controller's state variable). */
struct PIDControl {
    UINT32 kp; /**< Proportional gain constant */
    UINT32 ki; /**< Integral gain constant */
    UINT32 kd; /**< Derivative gain constant */
    UINT64 error;
    UINT64 input;   // encoder reading
    UINT64 output;  // control effort
    UINT64 referrence;  // setpoint, must be written to
    UINT64 integralOut;
    UINT64 lastInput;
    
};

volatile struct PIDControl motor1_pid;
volatile struct PIDControl motor2_pid;
volatile struct PIDControl motor3_pid;

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @Function int pid_control (struct PIControl *p, int e);
 * @param struct PIControl *p,
 * @return int
 * @brief  
 * @author M*/
void pid_control(struct PIDControl *p, int e);

/**
 * @Function int pid_control_init (struct PIControl *p, int e);
 * @param struct PIControl *p,
 * @return int
 * @brief  
 * @author M*/
void pid_control_init(struct PIDControl *p);

#endif /* _PI_CONTROL_H */



