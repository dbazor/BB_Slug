/* BB_Motor.c
 * 
 * Pavlo Vlastos
 * 
 * Created February 22, 2017, 3:07PM
 * 
 */

// Includes
#include <xc.h>
#include "BB_BOARD.h"

#include <plib.h>
#include <peripheral/osc.h>
#include <peripheral/lock.h>
#include "BB_Motor.h"
/*  MotorsInit(void)
 **
 ** Notes:
 **     This function is called in BB_BOARD_Init() from BB_Board.c
 **
 **	Parameters:
 **		
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Initialize PWM and direction pins for motors
/* ------------------------------------------------------------ */
void MotorsInit(void) 
{
    T2CONbits.TCKPS = 2;    // Timer2 prescaler N=4 (1:4)
    PR2 = 999;              // period = (PR2+1) * N * 12.5 ns = 100 us, 20 kHz
    TMR2 = 0;               // initial TMR2 count is 0
    
    OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults
    OC2CONbits.OCM = 0b110; // PWM mode without fault pin; other OC2CON bits are defaults
    OC3CONbits.OCM = 0b110; // PWM mode without fault pin; other OC3CON bits are defaults
    
    OC1RS = 500;            // duty cycle = OC1RS/(PR2+1) = 50%
    OC1R = 500;             // initialize before turning OC1 on; afterward it is read-only
    OC2RS = 500;            
    OC2R = 500;             
    OC3RS = 500;            
    OC3R = 500;             
    
    T2CONbits.ON = 1;       // turn on Timer2 (all OC will use timer 2)
    
    OC1CONbits.ON = 1;      // turn on OC1
    OC2CONbits.ON = 1;      // turn on OC2
    OC3CONbits.ON = 1;      // turn on OC3
    
    // Changing the below values change OCxR, setting the duty cycle
    // Don't change OCxR directly, because it is read-only. Only change
    // OCxRS, not OCxR. Changes take effect at the start of the new 
    // time period of the PWM (or OCx Output, I think).
    OC1RS = 0;              // set duty cycle to 0%
    OC2RS = 0;              // set duty cycle to 0%
    OC3RS = 0;              // set duty cycle to 0%
    
    // Configure direction pins on Port E, pins 0, 1, and 2 
    // This corresponds to (Port B, PMOD pins 1, 2, and 3 on Mx7)
    PORTSetPinsDigitalOut(IOPORT_E, BIT_0 | BIT_1 | BIT_2);
    
    // Set all pins high initially.
    PORTSetBits(IOPORT_E, BIT_0 | BIT_1 | BIT_2);
}

/*  Motor(int PWM, int direction, int motorNum)
 **
 **	Parameters:
 **		dutyCycle is the duty cycle for PWM within period range: 0 to 2000
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Sets PWM pin 0C1 which is port D, pin 2 (JD-02) on Mx7 pro
/* ------------------------------------------------------------ */
void Motor(int PWM, int direction, int motorNum) {
    // Limit PWM value to less than the period. See period define in BB_Motor.h
    PWM = PWM%PERIOD;
    
    // Select Motor
    if (motorNum == MOTOR_1) {
        Motor1(PWM, direction);
        
    } else if (motorNum == MOTOR_2) {
        Motor2(PWM, direction);
    } 
    else if (motorNum == MOTOR_3) {
        Motor3(PWM, direction);
    }
}
/*  Motor(int PWM, int direction)
 **
 **	Parameters:
 **		PWM and direction
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Write PWM to pic32 port D, pin 0 (JD-02 on Mx7)
 **     Set 
/* ------------------------------------------------------------ */
void Motor1(int PWM, int direction) {
    OC1RS = PWM;                        // write PWM to output compare register
    
    if (direction == FORWARD) {
        PORTSetBits(IOPORT_E, BIT_0);
    } else if (direction == REVERSE) {
        PORTClearBits(IOPORT_E, BIT_0);
    }
}
