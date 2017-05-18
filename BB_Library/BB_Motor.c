/* BB_Motor.c
 * 
 * Pavlo Vlastos
 * 
 * Created February 22, 2017, 3:07PM
 * 
 */

/*******************************************************************************
 * PRIVATE #INCLUDES                                                           *
 ******************************************************************************/
#include <xc.h>
#include "BB_BOARD.h"

#include <plib.h>
#include "BB_Motor.h"
#include <math.h>

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/
static void Motor1(int PWM, int direction);
static void Motor2(int PWM, int direction);
static void Motor3(int PWM, int direction);

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
volatile static int motor1Speed;
volatile static int motor2Speed;
volatile static int motor3Speed;
volatile static int motor1Direction;
volatile static int motor2Direction;
volatile static int motor3Direction;



const double inverseA[HEIGHT][WIDTH] = {
    {0, TWO_THIRDS, MOTOR_BRACKET_LENGTH},
    {ROOT3OVER3, -ONE_THIRD, MOTOR_BRACKET_LENGTH},
    {-ROOT3OVER3, -ONE_THIRD, MOTOR_BRACKET_LENGTH}
};

#define SPEED(x) (90 + (x))
#define DRIVE(x,y,z)   SetMotorSpeed(90 + (x),MOTOR_1); SetMotorSpeed(90 + (y),MOTOR_2); SetMotorSpeed(90 + (z),MOTOR_3)

#define MAXSPEED 1000

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
    T2CONbits.TCKPS = 2; // Timer2 prescaler N=4 (1:4)
    PR2 = 999; // period = (PR2+1) * N * 12.5 ns = 100 us, 20 kHz
    TMR2 = 0; // initial TMR2 count is 0

    OC1CONbits.OCM = 0b110; // PWM mode without fault pin; other OC1CON bits are defaults
    OC2CONbits.OCM = 0b110; // PWM mode without fault pin; other OC2CON bits are defaults
    OC3CONbits.OCM = 0b110; // PWM mode without fault pin; other OC3CON bits are defaults

    OC1RS = 500; // duty cycle = OC1RS/(PR2+1) = 50%
    OC1R = 500; // initialize before turning OC1 on; afterward it is read-only
    OC2RS = 500;
    OC2R = 500;
    OC3RS = 500;
    OC3R = 500;

    T2CONbits.ON = 1; // turn on Timer2 (all OC will use timer 2)

    OC1CONbits.ON = 1; // turn on OC1
    OC2CONbits.ON = 1; // turn on OC2
    OC3CONbits.ON = 1; // turn on OC3

    // Changing the below values change OCxR, setting the duty cycle
    // Don't change OCxR directly, because it is read-only. Only change
    // OCxRS, not OCxR. Changes take effect at the start of the new 
    // time period of the PWM (or OCx Output, I think).
    OC1RS = 0; // set duty cycle to 0%
    OC2RS = 0; // set duty cycle to 0%
    OC3RS = 0; // set duty cycle to 0%

    // Configure direction pins on Port E, pins 0, 1, and 2 
    // This corresponds to (Port B, PMOD pins 1, 2, and 3 on Mx7)
    PORTSetPinsDigitalOut(IOPORT_E, BIT_0 | BIT_1 | BIT_2 | BIT_3);

    // Set all pins high initially (or forward)
    PORTSetBits(IOPORT_E, BIT_0 | BIT_1 | BIT_2 | BIT_3);
}

/*  SetDrive(int angle, int rotation)
 **
 **	Parameters:
 **		PWM, range: -1000 to 1000
 **     motorNum is either MOTOR_1, MOTOR_2, or MOTOR_3
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		
/* ------------------------------------------------------------ */
void SetDrive(int angle, int rotation)
{
    double x = cos(M_PI * (double) angle / 180);
    double y = sin(M_PI * (double) angle / 180);

    DRIVE((int) (x * MAXSPEED), (int) ((-0.5 * x + 0.866 * y) * MAXSPEED), (int) ((-0.5 * x - 0.866 * y) * MAXSPEED));
}

/*  SetMotorSpeed(int PWM, int motorNum)
 **
 **	Parameters:
 **		PWM, range: -1000 to 1000
 **     motorNum is either MOTOR_1, MOTOR_2, or MOTOR_3
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		
/* ------------------------------------------------------------ */
void SetMotorSpeed(int PWM, int motorNum)
{
    int direction;

    // Setting direction
    if (PWM < 0) {
        direction = REVERSE;
        PWM = -PWM;
    } else {
        direction = FORWARD;
    }

    // Clip output
    if (PWM > MAX_PWM) {
        PWM = MAX_PWM;
    } else if (PWM < MIN_PWM) {
        PWM = MIN_PWM;
    }

    // Select Motor
    switch (motorNum) {
    case MOTOR_1:
        Motor1(PWM, direction);
        break;

    case MOTOR_2:
        Motor2(PWM, direction);
        break;

    case MOTOR_3:
        Motor3(PWM, direction);
        break;

    }
}

/*  GetMotorSpeed(int motorNum)
 **
 **	Parameters:
 **		
 **
 **	Return Value:
 **		
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		
/* ------------------------------------------------------------ */
int GetMotorSpeed(int motorNum)
{
    switch (motorNum) {
    case MOTOR_1:
        return motor1Speed;
        break;

    case MOTOR_2:
        return motor2Speed;
        break;

    case MOTOR_3:
        return motor3Speed;
        break;
    }
}

/*  GetMotorDirection(int motorNum)
 **
 **	Parameters:
 **		
 **
 **	Return Value:
 **		motorXDirection
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		
/* ------------------------------------------------------------ */
int GetMotorDirection(int motorNum)
{
    switch (motorNum) {
    case MOTOR_1:
        return motor1Direction;
        break;

    case MOTOR_2:
        return motor2Direction;
        break;

    case MOTOR_3:
        return motor3Direction;
        break;
    }
}

/*  static void Motor1(int PWM, int direction)
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
static void Motor1(int PWM, int direction)
{
    OC1RS = PWM; // write PWM to output compare register

    motor1Speed = PWM;
    motor1Direction = direction;

    if (direction == FORWARD) {
        PORTSetBits(MOTOR_1_DIR);

    } else if (direction == REVERSE) {
        PORTClearBits(MOTOR_1_DIR);
    }
}

/*  static void Motor2(int PWM, int direction)
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
 **		Write PWM to pic32 port D, pin 1 (JD-07 on Mx7)
 **     Set 
/* ------------------------------------------------------------ */
static void Motor2(int PWM, int direction)
{
    OC2RS = PWM; // write PWM to output compare register

    motor2Speed = PWM;
    motor2Direction = direction;

    if (direction == FORWARD) {
        PORTSetBits(MOTOR_2_DIR);

    } else if (direction == REVERSE) {
        PORTClearBits(MOTOR_2_DIR);
    }
}

/*  static void Motor3(int PWM, int direction)
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
 **		Write PWM to pic32 port D, pin 2 (JD-08 on Mx7)
 **     Set 
/* ------------------------------------------------------------ */
static void Motor3(int PWM, int direction)
{
    OC3RS = PWM; // write PWM to output compare register

    motor3Speed = PWM;
    motor3Direction = direction;

    if (direction == FORWARD) {
        PORTSetBits(MOTOR_3_DIR);

    } else if (direction == REVERSE) {
        PORTClearBits(MOTOR_3_DIR);
    }
}

/*  void MotorStop(int PWM, int direction)
 **
 **	Parameters:
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Stop all motors 
/* ------------------------------------------------------------ */
void MotorsStop(void)
{
    OC1RS = 0;
    OC2RS = 0;
    OC3RS = 0;
    motor1Speed = 0;
    motor2Speed = 0;
    motor3Speed = 0;
    motor1Direction = FORWARD;
    motor2Direction = FORWARD;
    motor3Direction = FORWARD;
}

/**
 * Function: SetMotor_XYZ()
 * @param   x, y, z
 * @return  
 * @brief   Virtual wheel translation for individual motor contribution. 
 *          Parameter z is rotation, which we pass a zero to for now.
 *          This function vector multiplies by inv(A) and then sets each motor.
 * @precon  x, y, and z are to be scaled properly
 **/
void SetMotor_XYZ(double x, double y, double z)
{
    INT32 pwm[HEIGHT] = {0, 0, 0};
    int row;
    int col;

    const double mbl = MOTOR_BRACKET_LENGTH/3; 
    const double oneThird = (1.0/3.0);
    const double twoThird = (2.0/3.0);
    const double root3over3 = (0.577350269);

    //    // Matrix 'inverseA' multiplied by motor frame vector 'y'
    //    for (row = 0; row < HEIGHT; row++) {
    //        for (col = 0; col < WIDTH; col++) {
    //            pwm[row] += v[col] * inverseA[row][col];
    //        }
    //    }

    pwm[0] = 0                  + twoThird * y + mbl * z;
    pwm[1] = root3over3 * x     - oneThird * y + mbl * z;
    pwm[2] = -root3over3 * x    - oneThird * y + mbl * z;
    
    //printf("pwm[0]: %d, pwm[1]: %d, pwm[2]: %d\n", pwm[0], pwm[1], pwm[2]);
    
    SetMotorSpeed(pwm[0], MOTOR_1);
    SetMotorSpeed(pwm[1], MOTOR_2);
    SetMotorSpeed(pwm[2], MOTOR_3);
}