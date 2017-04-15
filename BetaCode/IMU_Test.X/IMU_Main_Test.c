/*
 * File:   Motor_Test_Main.c
 * Author: Pavlo Vlastos
 * Based on Talking_Test.c by Daniel Bazor
 * Created on February 18, 2017, 1:17 PM
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
#include <peripheral/i2c.h>
#include "BB_I2C.h"

// Additional Includes (April 7, 2017)
#include "BNO55_Register_Map.h"
#include "BNO55_I2C_driver.h"
// 

/* ------------------------------------------------------------ */
/*				Definitions										*/
/* ------------------------------------------------------------ */
#define cntMsDelay 10000
//#define Nop() asm( "nop" )                 //No-operation; asm stands for assembly, using an assembly command in C.  Cool!


// KEEP IN MIND THAT THE ADDRESS BYTE IS REALLY 7 BITS, THE ADDRESS IS SHIFTED TO THE LEFT BY 1 BIT
// FOR READ/WRITE
#define SLAVE_ADDR 0x28

/* ------------------------------------------------------------ */
/*				Prototypes										*/
/* ------------------------------------------------------------ */
void DeviceInit();
//void DelayInit();
//void DelayMs(int cms);
//function declaration from Northwestern
void SendData(int, unsigned int);
void Delayms(unsigned t);
//extern interrupt void InputCapture2(void);


/* ------------------------------------------------------------ */
/*				Private Variables								*/

/* ------------------------------------------------------------ */


typedef enum i2c_states {
    START,
    WRITE,
    READ,
    ACK,
    RESTART,
    NACK,
    STOP,
    IDLE
} i2c_states;

/* ------------------------------------------------------------ */
/*				Main											*/

/* ------------------------------------------------------------ */

int main()
{
    BB_BOARD_Init();
    //    DelayInit();
    DeviceInit();

    __builtin_disable_interrupts();
    //    i2c_slave_setup(SLAVE_ADDR); // init I2C5, which we use as a slave
    //    // (comment out if slave is on another pic)
    //    i2c_master_setup(); // init I2C2, which we use as a master
    __builtin_enable_interrupts();

    // Running sample code from Northwestern

    // init functions
    I2CConfigure(I2C1, I2C_ENABLE_SLAVE_CLOCK_STRETCHING | I2C_ENABLE_HIGH_SPEED);
    I2CSetFrequency(I2C1, BB_BOARD_GetPBClock(), I2C_CLOCK_FREQ);
    // not setting slave address since the board will only be running in master mode
    I2CEnable(I2C1, TRUE);

    int rcv = 0xFF; //For received data
    int dataLocation = 0; //  UNIT_SEL; // do this later
    unsigned char rcvData = 0x00;
    unsigned char sndData = 0x00;
    unsigned char dat;

    //// Attempt to configure IMU here, to get data instead of zeros from various IMU registers
    //Config_BNO55();

    dataLocation = BNO055_ACC_DATA_X_LSB;
    printf("Starting\n");
    while (1) {
        // TEST: 
        // 1) Read register 
        // 2) Attempt to write to register
        // 3) Read register again
        // 4) Attempt to write to register again
        // 5) Read register for the last time

        MPU_I2C_Read(SLAVE_ADDR, dataLocation, 1, &rcvData);
        printf("1 Received byte: %d\n", rcvData);
        Delayms(500);
        
        sndData = 0x00;
        MPU_I2C_Write(SLAVE_ADDR, dataLocation, 1, sndData);
        printf("2 Transmitted byte: %d\n", sndData);
        Delayms(500);
        
        MPU_I2C_Read(SLAVE_ADDR, dataLocation, 1, &rcvData);
        printf("3 Received byte: %d\n", rcvData);
        Delayms(500);
        
        sndData = 0x01;
        MPU_I2C_Write(SLAVE_ADDR, dataLocation, 1, sndData);
        printf("4 Transmitted byte: %d\n", sndData);
        Delayms(500);
        
        MPU_I2C_Read(SLAVE_ADDR, dataLocation, 1, &rcvData);
        printf("5 Received byte: %d\n", rcvData);
        Delayms(500);
        
        MPU_I2C_Write(BNO55_I2C_ADDR, OPR_MODE, 1, &dat);
        Delayms(50);
        MPU_I2C_Read(BNO55_I2C_ADDR, OPR_MODE, 1, &dat);
        if (dat == NDOF_CON) {
            printf("Configured\n");
        }
        
        Delayms(500);
    }

    while (1) {
        ;
    }

    return 0;
}

//
//

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

/*  DeviceInit()
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Set LD1 through LD4 as digital output
/* ------------------------------------------------------------ */

void DeviceInit()
{
    //On MX4ck LED1 is on RG12
    //		   LED2 is on RG13
    //		   LED3 is on RG14
    //		   LED4 is on RG15
    //Set ports for onboard LEDs to outputs
    PORTSetPinsDigitalOut(IOPORT_G, BIT_12 | BIT_13 | BIT_14 | BIT_15);
    PORTSetPinsDigitalOut(IOPORT_E, BIT_1);
    PORTSetPinsDigitalIn(IOPORT_E, BIT_0);
}

void Delayms(unsigned t)
// This uses Timer 1, can be changed to another timer. Assumes FPB = SYS_FREQ
{
    OpenTimer1(T1_ON | T1_PS_1_256, 0xFFFF);
    while (t--) { // t x 1ms loop
        WriteTimer1(0);
        while (ReadTimer1() < SYS_FREQ / 256 / 1000);
    }
    CloseTimer1();
} // Delayms

/*****************************************************
 * RcvData(unsigned int address)		     *
 *					  	     *
 * Gets a byte of data from I2C slave device at      *
 *  ADDRESS.					     *
 *						     *
 * Returns: Received data			     *
 ****************************************************/
int RcvData(unsigned int address)
{
    StartI2C1(); //Send line start condition
    IdleI2C1(); //Wait to complete
    MasterWriteI2C1((address << 1) | 1); //Write out slave address OR 1 (read command)
    IdleI2C1(); //Wait to complete
    int rcv = MasterReadI2C1(); //Read in a value
    StopI2C1(); //Send line stop condition
    IdleI2C1(); //Wait co complete
    return rcv; //Return read value
}

/***************************************************
 * SendData(int data, unsigned int address)        *
 *						    *
 * Sends a byte of data (DATA) over the I2C line   *
 *	to I2C address ADDRESS			    *
 *						    *
 * Returns: nothing				    *
 ***************************************************/
void SendData(int data, unsigned int address)
{
    StartI2C1(); //Send the Start Bit
    IdleI2C1(); //Wait to complete

    MasterWriteI2C1((address << 1) | 0); //Sends the slave address over the I2C line. This must happen first so the 
    //proper slave is selected to receive data.
    IdleI2C1(); //Wait to complete

    MasterWriteI2C1(data); //Sends data byte over I2C line
    IdleI2C1(); //Wait to complete

    StopI2C1(); //Send the Stop condition
    IdleI2C1(); //Wait to complete

} //end function


///* ------------------------------------------------------------ */
//
///***	DelayInit
// **
// **	Parameters:
// **		none
// **
// **	Return Value:
// **		none
// **
// **	Errors:
// **		none
// **
// **	Description:
// **		Initialized the hardware for use by delay functions. This
// **		initializes Timer 1 to count at 10Mhz.
///* ------------------------------------------------------------ */
//
//void DelayInit()
//{
//    unsigned int tcfg;
//
//    /* Configure Timer 1. This sets it up to count a 10Mhz with a period of 0xFFFF
//     */
//    tcfg = T1_ON | T1_IDLE_CON | T1_SOURCE_INT | T1_PS_1_8 | T1_GATE_OFF | T1_SYNC_EXT_OFF;
//    OpenTimer1(tcfg, 0xFFFF);
//
//}
//
///* ------------------------------------------------------------ */
//
///***	DelayMs
// **
// **	Parameters:
// **		cms			- number of milliseconds to delay
// **
// **	Return Value:
// **		none
// **
// **	Errors:
// **		none
// **
// **	Description:
// **		Delay the requested number of milliseconds. Uses Timer1.
///* ------------------------------------------------------------ */
//
//void DelayMs(int cms)
//{
//    int ims;
//
//    for (ims = 0; ims < cms; ims++) {
//        WriteTimer1(0);
//        while (ReadTimer1() < cntMsDelay);
//    }
//}

