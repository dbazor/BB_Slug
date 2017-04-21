/*
 * File:   MotorControllerMain.c
 * Author: BB Slug Team
 * 
 * PID control based off of material found in Embedded Computing and Mechatronics with the PIC32
 * Microcontroller Kevin M. Lynch, Nicholas Marchuk, Matthew L. Elwin
 * 
 * Created on April 12, 2017, 5:12 PM
 */
/************************************************************************/
/*  File Description:                                                   */
/************************************************************************/
#include "BB_BOARD.h"
#include <plib.h>
#include <stdio.h>
#include "BB_Motor.h"
#include "BB_Encoder.h"
#include "BB_LEDS.h"
#include "BB_UART.h"
#include <math.h>
#include "BB_MOTOR_CONTROLLER.h"

/* ------------------------------------------------------------ */
/*				Definitions			*/
/* ------------------------------------------------------------ */
#define cntMsDelay      5000
#define thsndDelay      1000




/* ------------------------------------------------------------ */
/*				Prototypes			*/
/* ------------------------------------------------------------ */
void DeviceInit();
void DelayInit();
void DelayMs(unsigned t);
void DelayUs(unsigned t);


/* ------------------------------------------------------------ */
/*				Global Variables		*/
/* ------------------------------------------------------------ */
volatile float eCountRadians = 0;



/* ------------------------------------------------------------ */
/*                            Main                              */

/* ------------------------------------------------------------ */
int main()
{
    BB_BOARD_Init();

    PORTSetPinsDigitalOut(JC03); // PMOD Pin JC 03
    PORTClearBits(JC03);
    // Interrupt
    // *NOTE:
    //      Make sure to change both T4_PS_1_64 and the PRESCALE #define
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // STEP 2. configure Timer 1 using internal clock, 1:64 pre-scaler
    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_64, T4_PERIOD);
    // set up the timer interrupt with a priority of 2
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_2);
    // enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();

    //    printf("T4_PS_1_256:     %x\n", T4_PS_1_256);
    //    printf("T4_PS_1_64:      %x\n", T4_PS_1_64);
    //    printf("T4_PS_1_32       %x\n", T4_PS_1_32);
    //    printf("T4_ON            %x\n", T4_ON);
    float oldECR = 0;
    while (1) {
        if (eCountRadians != oldECR) {
            printf("eCountRadians: %f \n", eCountRadians);
            oldECR = eCountRadians;
        }
    }

    //    eprev = 0;      // initial "previous error" is zero
    //    eint = 0;       // initial error integral is zero
    //    now = 0;        // "now" tracks the elapsed time
    //    every dt seconds do {
    //    s = readSensor();             // read sensor value
    //    r = referenceValue(now);      // get reference signal for time "now"
    //    e = r - s;                    // calculate the error
    //    edot = e - eprev;             // error difference
    //    eint = eint + e;              // error sum
    //    u = Kp*e + Ki*eint + Kd*edot; // calculate the control signal
    //    sendControl(u);               // send control signal to the plant
    //    eprev = e;                    // current error is now prev error for next iteration
    //    now = now + dt;               // update the "now" time
    //    }    
    // dt is Ts in code below

    // for motor 1
    //    float reference = 0; // the target angle value
    //    float e1prev = 0; // initial "previous error" is zero
    //    float e1int = 0; // initial error integral is zero
    //    float e1dot = 0; // 
    //    float now1 = 0; // "now" tracks the elapsed time
    //    float error1 = 0; // the error or difference between the reference and the actual angle
    //    float frequency = 2000;
    //    double u1 = 0;
    //    float Ts1 = 1 / frequency;
    //    float Kp1 = 0.129;
    //    float Ki1 = 0.0735 * Ts1;
    //    float Kd1 = 0.0565 / Ts1;
    //
    //    double encoder1value = 0;
    //    printf("STARTING\n");
    //
    //    //    SetMotorSpeed(250, MOTOR_1);
    //    //    DelayUs();  // Wait for 5 seconds, to allow for turning the wheel
    //    //                    // then the motor should try to move the wheel back to 
    //    //                    // 'zero'
    //    //    MotorsStop();
    //    //    printf("Motor Stopped\n");
    //    //    
    //    int j = 0;
    //    while (1) {
    //        DelayUs(1000);
    //        printf("%d\n", j++);
    //    }
    //
    //    int i = 10000;
    //    while (i > 0) {
    //        encoder1value = GetEncoderRadians(MOTOR_1); //= tick2Radian*GetEncoderCount(MOTOR_1);   // read sensor convert to rad
    //
    //        //
    //        // *IMPORTANT! - READ THE FOLLOWING NOTE!
    //        // *NOTE: reference will be updated in future to be whatever value is needed at a given time
    //        reference = 0; // get reference signal for time "now"
    //
    //        error1 = reference - encoder1value; // calculate the error
    //        e1dot = error1 - e1prev; // error difference
    //        e1int = e1int + error1; // error sum
    //        u1 = Kp1 * error1 + Ki1 * e1int + Kd1*e1dot; // calculate the control signal
    //
    //        // u1 needs to be converted to PWM to voltage
    //        SetMotorSpeed(u1*VOLT_2_PWM, MOTOR_1); // send control signal to the plant to rotate so many degrees
    //        e1prev = error1; // current error is now previous error for next iteration
    //        now1 = now1 + Ts1; // update the "now" time
    //
    //        DelayUs(500); // half a millisecond -> 2kHz
    //
    //        i--;
    //    }
    //
    //
    //    printf("encoder1value: %e \n", encoder1value);
    //    printf("error1: %f \n", error1);
    //    printf("e1dot: %f \n", e1dot);
    //    printf("e1int: %f\n", e1int);
    //    printf("u1: %e \n", u1);
    //    printf("ENDING\n \n");

    while (1); // sit and spin

    return 0;
}
