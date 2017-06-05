/* BB_Encoder.c
 * 
 * Pavlo Vlastos & Mikey Gonzalez & Daniel Bazor
 * 
 * Created February 22, 2017, 3:07PM
 * 
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include "BB_Encoder.h"
#include <xc.h>
#include <plib.h>
#include "BB_Motor.h"
#include <math.h>


/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

/*******************************************************************************
 * Defines                                                                      *
 ******************************************************************************/
//#define tick2Radian  (2*M_PI/2240)
#define TICK_2_M (0.000140249672)
#define TICK_2_RAD (0.002804993)

/*******************************************************************************
 * PRIVATE Encoder Variables                                                   *
 ******************************************************************************/
static volatile int mc1 = 0; // motor 1 encoder count 
static volatile int mc2 = 0; // motor 2 encoder count 
static volatile int mc3 = 0; // motor 3 encoder count

/* ------------------------------------------------------------ */
/*				Interrupt Sub-Routine           */

/* ------------------------------------------------------------ */
void __ISR(_INPUT_CAPTURE_2_VECTOR, IPL3SOFT) InputCapture2()
{
    int ic2time = IC2BUF; // clear fifo

    int encoder1A = PORTReadBits(ENCODER_1A); // input capture for motor 1 encoder signal A
    int encoder1B = PORTReadBits(ENCODER_1B); // digital input for motor 1 encoder signal B

    // if encoder A is high (greater than 0)
    // and if encoder B is also high (greater than 0)
    //      increment encoder count 
    // else if A and B are not equal and are both not greater than zero
    //      decrement encoder count
    if (((encoder1A > 0) && (encoder1B > 0)) || (encoder1A == encoder1B)) {
        mc1++;
        // localm1E1Count++;
        // SetEncoder1Count(localm1E1Count);
    } else if (encoder1A != encoder1B) {
        mc1--;
        // localm1E1Count--;
        // SetEncoder1Count(localm1E1Count);
    }
    IFS0bits.IC2IF = 0; // clear interrupt flag
}

void __ISR(_INPUT_CAPTURE_3_VECTOR, IPL3SOFT) InputCapture3()
{
    int ic3time = IC3BUF; // clear fifo

    int encoder2A = PORTReadBits(ENCODER_2A); // input capture for motor 2 encoder signal A
    int encoder2B = PORTReadBits(ENCODER_2B); // digital input for motor 2 encoder signal B

    // if encoder A is high (greater than 0)
    // and if encoder B is also high (greater than 0)
    //      increment encoder count 
    // else if A and B are not equal and are both not greater than zero
    //      decrement encoder count
    if (((encoder2A > 0) && (encoder2B > 0)) || (encoder2A == encoder2B)) {
        mc2++;
    } else if (encoder2A != encoder2B) {
        mc2--;
    }

    IFS0bits.IC3IF = 0; // clear interrupt flag
}

void __ISR(_INPUT_CAPTURE_5_VECTOR, IPL3SOFT) InputCapture5()
{
    int ic5time = IC5BUF; // clear fifo

    int encoder3A = PORTReadBits(ENCODER_3A); // input capture for motor 3 encoder signal A
    int encoder3B = PORTReadBits(ENCODER_3B); // digital input for motor 3 encoder signal B

    // see previous ISR for details
    if (((encoder3A > 0) && (encoder3B > 0)) || (encoder3A == encoder3B)) {
        mc3++;
    } else if (encoder3A != encoder3B) {
        mc3--;
    }
    IFS0bits.IC5IF = 0; // clear interrupt flag
}

/**
 * Function: Encoder_Init(void)
 * @param None
 * @return None
 * @brief Sets input capture pins and IO pins to read encoder signals
 *      Also sets up IC2, IC3, and IC5 to all use Timer 3. Sets to captuer
 *      on falling and rising edge. See function for more settings initialized
 * @author Pavlo Vlastos */
void EncoderInit(void)
{
    // Set up pins to read encoder A signals
    PORTSetPinsDigitalIn(IOPORT_D, BIT_9 | BIT_10 | BIT_12);
    PORTSetPinsDigitalIn(IOPORT_E, BIT_4 | BIT_5 | BIT_6);

    // set up IC3 to use Timer3. 
    T3CONbits.TCKPS = 0x3; // Timer3 1:8 prescaler; ticks at 10 MHz (each tick is 100ns)
    PR3 = TMR3_ROLLOVER;

    // rollover value is also used in ISR to handle timer rollovers.
    TMR3 = 0;

    IC2CONbits.ICTMR = 0; // IC2 uses Timer3
    IC3CONbits.ICTMR = 0; // IC3 uses Timer3
    IC5CONbits.ICTMR = 0; // IC5 uses Timer3

    IC2CONbits.ICM = 0b110; // falling and rising for IC2
    IC3CONbits.ICM = 0b110; // falling and rising for IC3
    IC5CONbits.ICM = 0b110; // falling and rising for IC5

    IC2CONbits.FEDGE = 0; // capture rising edge first for IC2
    IC3CONbits.FEDGE = 0; // capture rising edge first for IC3
    IC5CONbits.FEDGE = 0; // capture rising edge first for IC5

    IC2CONbits.ICI = 0; // interrupt every 1st edge for IC2
    IC3CONbits.ICI = 0; // interrupt every 1st edge for IC3
    IC5CONbits.ICI = 0; // interrupt every 1st edge for IC5

    IFS0bits.IC2IF = 0; // clear interrupt flag for IC2
    IFS0bits.IC3IF = 0; // clear interrupt flag for IC3
    IFS0bits.IC5IF = 0; // clear interrupt flag for IC5

    IPC2bits.IC2IP = 1; // interrupt priority 1 for IC2
    IPC3bits.IC3IP = 2; // interrupt priority 2 for IC3
    IPC5bits.IC5IP = 3; // interrupt priority 3 for IC5

    IEC0bits.IC2IE = 1; // enable IC2 interrupt
    IEC0bits.IC3IE = 1; // enable IC3 interrupt
    IEC0bits.IC5IE = 1; // enable IC5 interrupt

    T3CONbits.ON = 1; // turn on Timer3

    IC2CONbits.ON = 1; // turn on IC2
    IC3CONbits.ON = 1; // turn on IC3
    IC5CONbits.ON = 1; // turn on IC5
}

/**
 * Function: GetEncoderCount(UINT8 motorNum)
 * @param UINT8 motorNum
 * @return Integer representing Encoder Count
 * @brief Get the encoder count of a specified motor
 **/
int EncoderGetCount(UINT8 motorNum)
{
    switch (motorNum) {
    case MOTOR_1:
        return mc1;
        break;
    case MOTOR_2:
        return mc2;
        break;
    case MOTOR_3:
        return mc3;
        break;
    }
}

/**
 * Function: EncoderGetXYZmeters
 * @param UINT8 motorNum
 * @return Float representing Encoder Count in radians
 * @brief Get the encoder count in radians of a specified motor
 **/
float EncoderGetXYZmeters(encodeVal *e)
{
    EncoderGetXYZ(e);
    e->x = e->x * TICK_2_M;
    e->y = e->y * TICK_2_M;
    e->rot = e->rot * TICK_2_M; // This one is most likely not right. Rotation in mm?
}

/**
 * Function: EncoderGetVirtualWheels
 * @param UINT8 motorNum
 * @return Float representing Encoder Count in radians
 * @brief Get the encoder count in radians of a specified motor
 **/
float EncoderGetVirtualWheels(encodeVal *e)
{
    EncoderGetXYZ(e);
    e->x = e->x * TICK_2_RAD;
    e->y = e->y * TICK_2_RAD;
    e->rot = e->rot * TICK_2_RAD; // This one is most likely not right. Rotation in mm?
}

/**
 * Function: EncoderGetXYZ
 * @param 
 * @return 
 * @brief
 **/
void EncoderGetXYZ(encodeVal *e)
{
    const double root3over2 = (0.8660254038);
    const double mblInv = 1 / MOTOR_BRACKET_LENGTH;
    e->x = (root3over2 * mc2) - (root3over2 * mc3);
    e->y = (double) mc1 - (0.5 * mc2) - (0.5 * mc3);
    e->rot = (mblInv * mc1) + (mblInv * mc2) + (mblInv * mc3);
    // printf("m1: %d, m2: %d, m3: %d\n\n", mc1, mc2, mc2);
    // printf("e.x = %f, e.y = %f, e.rot = %f\n", e->x, e->y, e->rot);
}

/**
 * Function: SetEncoderCount(UINT8 motorNum, UINT8 value)
 * @param UINT8 motorNum, UINT8 value
 * @return None
 * @brief Set the encoder count of a specified motor, to a desired value
 **/
void EncoderSetCount(UINT8 motorNum, UINT8 value)
{
    switch (motorNum) {
    case MOTOR_1:
        mc1 = value;
        break;
    case MOTOR_2:
        mc2 = value;
        break;
    case MOTOR_3:
        mc3 = value;
        break;
    }
}

/**
 * Function: SetEncoderCounts
 * @param 
 * @return None
 * @brief Set the encoder count of all motors to desired values
 **/
void SetEncoderCounts(int count1, int count2, int count3)
{
    mc1 = count1;
    mc2 = count2;
    mc3 = count3;
}
