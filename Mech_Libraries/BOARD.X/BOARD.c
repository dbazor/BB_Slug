/*
 * File:   BOARD.c
 * Author: Max
 *
 * Created on December 19, 2012, 2:08 PM
 */


#include <BOARD.h>
#include <xc.h>
#include <AD.h>
#ifndef BOARD_TEST
#include <serial.h>
#endif

#define _SUPPRESS_PLIB_WARNING
#include <plib.h>
#include <peripheral/osc.h>
#include <peripheral/lock.h>



#ifdef BOARD_TEST
#undef DISABLE_ADINIT
#define DISABLE_ADINIT
#endif


/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

#define SYSTEM_CLOCK 80000000L
#define  PB_CLOCK SYSTEM_CLOCK/2
#define TurnOffAndClearInterrupt(Name) INTEnable(Name,INT_DISABLED); INTClearFlag(Name)

/**
 * Function: BOARD_Init(void)
 * @param None
 * @return None
 * @brief Set the clocks up for the board, initializes the serial port, and turns
 * on the A/D subsystem for battery monitoring
 * @author Max Dunne, 2013.09.15  */
void BOARD_Init()
{
    //sets the system clock to the optimal frequency given the system clock
    //SYSTEMConfig(SYSTEM_CLOCK, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    //sets the divisor to 2 to ensure 40Mhz peripheral bus
    //OSCSetPBDIV(OSC_PB_DIV_2);


    //disables all A/D pins for a clean start
    AD1PCFG = 0xffff;

    //enables the interrupt system in the new style
    INTConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
    INTEnableInterrupts();
#ifndef BOARD_TEST
    //initializes the uart to give printf capabilities
    SERIAL_Init();
#endif
#ifndef DISABLE_ADINIT
    //starts the A/D system to ensure battery voltage monitoring
    AD_Init();
#endif
    //printf("CMPE118 IO stack is now initialized\r\n");

}

/**
 * Function: BOARD_End(void)
 * @param None
 * @return None
 * @brief shuts down all peripherals except for serial and A/D. Turns all pins
 * into input
 * @author Max Dunne, 2013.09.20  */
void BOARD_End()
{

    // disable timer interrupts, clear flags and turn off module
    TurnOffAndClearInterrupt(INT_T1);
    TurnOffAndClearInterrupt(INT_T1);
    TurnOffAndClearInterrupt(INT_T2);
    TurnOffAndClearInterrupt(INT_T3);
    TurnOffAndClearInterrupt(INT_T4);
    TurnOffAndClearInterrupt(INT_T5);
    T1CON = 0;
    T2CON = 0;
    T3CON = 0;
    T4CON = 0;
    T5CON = 0;


    // disable input capture interrupts, clear flags and turn off module
    TurnOffAndClearInterrupt(INT_IC1);
    TurnOffAndClearInterrupt(INT_IC2);
    TurnOffAndClearInterrupt(INT_IC3);
    TurnOffAndClearInterrupt(INT_IC4);
    TurnOffAndClearInterrupt(INT_IC5);
    IC1CONCLR = _IC1CON_ICM_MASK;
    IC2CONCLR = _IC2CON_ICM_MASK;
    IC3CONCLR = _IC3CON_ICM_MASK;
    IC4CONCLR = _IC4CON_ICM_MASK;
    IC5CONCLR = _IC5CON_ICM_MASK;

    // disable output compare interrupts, clear flags and turn off module
    TurnOffAndClearInterrupt(INT_OC1);
    TurnOffAndClearInterrupt(INT_OC2);
    TurnOffAndClearInterrupt(INT_OC3);
    TurnOffAndClearInterrupt(INT_OC4);
    TurnOffAndClearInterrupt(INT_OC5);
    TurnOffAndClearInterrupt(INT_AD1);
    OC1CONCLR = _OC1CON_ON_MASK;
    OC2CONCLR = _OC2CON_ON_MASK;
    OC3CONCLR = _OC3CON_ON_MASK;
    OC4CONCLR = _OC4CON_ON_MASK;
    OC5CONCLR = _OC5CON_ON_MASK;

    // disable I2C interrupts, clear flags and turn off module
    TurnOffAndClearInterrupt(INT_I2C1);
    TurnOffAndClearInterrupt(INT_I2C1B);
    TurnOffAndClearInterrupt(INT_I2C1M);
    TurnOffAndClearInterrupt(INT_I2C1S);
    TurnOffAndClearInterrupt(INT_I2C2);
    TurnOffAndClearInterrupt(INT_I2C2B);
    TurnOffAndClearInterrupt(INT_I2C2M);
    TurnOffAndClearInterrupt(INT_I2C2S);
    I2C1CONCLR = _I2C1CON_ON_MASK;
    I2C2CONCLR = _I2C2CON_ON_MASK;

    //disable spi interrupts, clear flags and turn off module
    TurnOffAndClearInterrupt(INT_SPI1);
    TurnOffAndClearInterrupt(INT_SPI1E);
    TurnOffAndClearInterrupt(INT_SPI1RX);
    TurnOffAndClearInterrupt(INT_SPI1TX);
    TurnOffAndClearInterrupt(INT_SPI2);
    TurnOffAndClearInterrupt(INT_SPI2E);
    TurnOffAndClearInterrupt(INT_SPI2RX);
    TurnOffAndClearInterrupt(INT_SPI2TX);
    SPI1CONCLR = _SPI1CON_ON_MASK;
    SPI2CONCLR = _SPI2CON_ON_MASK;

    // disable external interrupts, clear flags and turn off module
    TurnOffAndClearInterrupt(INT_INT0);
    TurnOffAndClearInterrupt(INT_INT1);
    TurnOffAndClearInterrupt(INT_INT2);
    TurnOffAndClearInterrupt(INT_INT3);
    TurnOffAndClearInterrupt(INT_INT4);

    // set all ports to be digital inputs
    PORTSetPinsDigitalIn(IOPORT_B, 0xff);
    PORTSetPinsDigitalIn(IOPORT_C, 0xff);
    PORTSetPinsDigitalIn(IOPORT_D, 0xff);
    PORTSetPinsDigitalIn(IOPORT_E, 0xff);
    PORTSetPinsDigitalIn(IOPORT_F, 0xff);
    PORTSetPinsDigitalIn(IOPORT_G, 0xff);

    //Serial and A/D are left on for output and battery monitoring respectively
}

/**
 * Function: BOARD_GetPBClock(void)
 * @param None
 * @return PB_CLOCK - speed the peripheral clock is running in hertz
 * @brief returns the speed of the peripheral clock.  Nominally at 40Mhz
 * @author Max Dunne, 2013.09.01  */
unsigned int BOARD_GetPBClock()
{
    return PB_CLOCK;
}


#ifdef BOARD_TEST


#define MAXPOWTWO 20

int main(void)
{
    BOARD_Init();

//    int curPow2 = 12;
//    int i;
    TRISDbits.TRISD4 = 0;
    LATDbits.LATD4 = 0;
    //will do a pulse of each power of two
    //using scope can determine the length of timing for nops in test harnesses
    //    for(curPow2=0;curPow2<=MAXPOWTWO;curPow2++)
    //    {
//    while (1) {
//        LATDbits.LATD4 ^= 1;
//        for (i = 0; i < 1830000; i++) {
//            asm("nop");
//        }
//        //LATDbits.LATD4 = 0;
//    }
    //will need a scope to test this module, the led should blink at the maximum rate
    while (1) {
        LATDbits.LATD4 ^= 1;
    }
}


#endif