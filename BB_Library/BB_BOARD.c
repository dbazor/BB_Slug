/*
 * File:   BB_BOARD.c
 * Author: Daniel
 *
 * Created on February 2, 2017, 12:10 AM
 */

#include "BB_BOARD.h"
#include "BB_Encoder.h"

#include <peripheral/osc.h>
#include <peripheral/lock.h>

#include "BB_UART.h"
#include "BB_LEDS.h"
#include "BB_CONTROLLER.h"
#include "BB_QUAT.h"


#ifdef BOARD_TEST
#undef DISABLE_ADINIT
#define DISABLE_ADINIT
#endif

/* ------------------------------------------------------------ */
/*				Configuration Bits				                */
/* ------------------------------------------------------------ */
// configure the microcontroller for use with the on-board (MX7) licensed debugger circuit
#pragma config ICESEL = ICS_PGx1

// NU32 suggested setup
// Device Configuration Registers

//#pragma config DEBUG = OFF          // Background Debugger disabled
#pragma config FWDTEN = OFF         // WD timer: OFF
#pragma config WDTPS = PS4096       // WD period: 4.096 sec
#pragma config POSCMOD = HS         // Primary Oscillator Mode: High Speed crystal
#pragma config FNOSC = PRIPLL       // Oscillator Selection: Primary oscillator w/ PLL
#pragma config FPLLMUL = MUL_20     // PLL Multiplier: Multiply by 20
#pragma config FPLLIDIV = DIV_2     // PLL Input Divider:  Divide by 2
#pragma config FPLLODIV = DIV_1     // PLL Output Divider: Divide by 1
#pragma config FPBDIV = DIV_1       // Peripheral Bus Clock: Divide by 1
#pragma config UPLLEN = ON          // USB clock uses PLL
#pragma config UPLLIDIV = DIV_2     // Divide 8 MHz input by 2, mult by 12 for 48 MHz
#pragma config FUSBIDIO = ON        // USBID controlled by USB peripheral when it is on
#pragma config FVBUSONIO = ON       // VBUSON controlled by USB peripheral when it is on
#pragma config FSOSCEN = OFF        // Disable second osc to get pins back                          ?
#pragma config BWP = ON             // Boot flash write protect: ON                                   ?
#pragma config FCANIO = OFF         // Use alternate CAN pins
#pragma config FMIIEN = OFF         // Use RMII (not MII) for ethernet
#pragma config FSRSSEL = PRIORITY_6 // Shadow Register Set for interrupt priority 6


/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

#define  PB_CLOCK SYS_FREQ
#define TurnOffAndClearInterrupt(Name) INTEnable(Name,INT_DISABLED); INTClearFlag(Name)// Configure UART1 for output
//#define DESIRED_BAUD 115200

/**
 * Function: BOARD_Init(void)
 * @param None
 * @return None
 * @brief Set the clocks up for the board, initializes the serial port, and turns
 * on the A/D subsystem for battery monitoring
 * @author Max Dunne, 2013.09.15  */
void BB_BOARD_Init()
{
    // disable interrupts
    __builtin_disable_interrupts();

    //disables all A/D pins for a clean start
    AD1PCFG = 0xffff;

    // Come back and double check what all of these commands are doing - Possibly replace some



    // NU32 startup functions
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583); // enable the cache

    // set the prefectch cache wait state to 2, as per the
    // electrical characteristics data sheet
    CHECONbits.PFMWS = 0x2;

    //enable prefetch for cacheable and noncacheable memory
    CHECONbits.PREFEN = 0x3;

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get B10, B11, B12 and B13 back
    DDPCONbits.JTAGEN = 0;

    //    // *NOTE: This is in the peripheral library example code init()
    //    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    Leds_Init();

    BB_UART_Init();

    MotorsInit();
    Encoder_Init();
    BB_I2C_Init();
    IMU_Init();

    // PID Motor Controller Interrupt
    // *NOTE:
    //      Make sure to change both T4_PS_1_64 and the PRESCALE #define
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // STEP 2. configure Timer 1 using internal clock, 1:64 pre-scaler
    OpenTimer4(T4_ON | T4_SOURCE_INT | T4_PS_1_64, T4_PERIOD);
    // set up the timer interrupt with a priority of 2
    ConfigIntTimer4(T4_INT_ON | T4_INT_PRIOR_2);
    // enable multi-vector interrupts
    INTEnableSystemMultiVectoredInt();
     
    __builtin_enable_interrupts();

}

/**
 * Function: BOARD_End(void)
 * @param None
 * @return None
 * @brief shuts down all peripherals except for serial and A/D. Turns all pins
 * into input
 * @author Max Dunne, 2013.09.20  */
void BB_BOARD_End()
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
/* ------------------------------------------------------------ */

/**
 * Function: DelayMs(void)
 * @param t -> number of milliseconds to delay
 * @return none
 * @brief Delay the requested number of milliseconds. Uses Timer1.
 **/
void DelayUs(unsigned t)
{
    OpenTimer1(T1_ON | T1_PS_1_8, 0xFFFF);
    while (t--) {
        WriteTimer1(0);
        while (ReadTimer1() < SYS_FREQ / 8 / 1000000);
    }
    CloseTimer1();
}

/* ------------------------------------------------------------ */

/**
 * Function: DelayUs(void)
 * @param t -> number of microseconds to delay
 * @return none
 * @brief Delay the requested number of microseconds. Uses Timer1.
 **/
void DelayMs(unsigned t)
{
    OpenTimer1(T1_ON | T1_PS_1_64, 0xFFFF);
    while (t--) {
        WriteTimer1(0);
        while (ReadTimer1() < SYS_FREQ / 64 / 1000);
    }
    CloseTimer1();
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