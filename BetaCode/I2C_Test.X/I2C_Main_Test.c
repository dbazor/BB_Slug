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
#if 1
    {
        // init functions
        I2CConfigure(I2C1, I2C_ENABLE_SLAVE_CLOCK_STRETCHING | I2C_ENABLE_HIGH_SPEED);
        I2CSetFrequency(I2C1, BB_BOARD_GetPBClock(), I2C_CLOCK_FREQ);
        // not setting slave address since the board will only be running in master mode
        I2CEnable(I2C1, TRUE);

        int rcv = 0xFF; //For received data
        int dataLocation = BNO055_CHIP_ID_ADDR;

        while (1) {
            SendData(dataLocation, SLAVE_ADDR); //Sends hex data 0xAA to slave address 0x40
            rcv = RcvData(SLAVE_ADDR); //Receives data from address 0x40
            printf("The output of %x is: %8x\n",dataLocation, rcv);
            Delayms(500);
            dataLocation++;
        }
    }
#endif 

#if 0
    {
        char slave = (SLAVE_ADDR << 1); //
        i2c_master_start(); // Begin the start sequence
        i2c_master_send(slave); // send the slave address, left shifted by 1,
    }
#endif


#if 0
    {
        while (1) {
            UINT16 slave = (SLAVE_ADDR << 1); //
            I2CEnable(I2C1, TRUE);
            I2CConfigure(I2C1, 0);
            I2CSetSlaveAddress(I2C1, slave, 0, I2C_USE_7BIT_ADDRESS);
            I2CSendByte(I2C1, 0x05);
        }
    }
#endif


#if 0
    {
        typedef volatile unsigned char * volatile buffer_t;

        static buffer_t to_write = NULL; // data to write
        static buffer_t to_read = NULL; // data to read - not sure if this should be a char or not

        static volatile unsigned char address = 0; // the 7-bit address to write to / read from
        static volatile unsigned int n_write = 0; // number of data bytes to write
        static volatile unsigned int n_read = 0; // number of data bytes to read

        i2c_master_start(); // Begin the start sequence
        i2c_states state = START; // initialize state

        int startFlag = 0;
        int writeFlag = 0;
        int readFlag = 0;
        int restartFlag = 0;
        int readFlag = 0;
        int ackFlag = 0;
        int nackFlag = 0;
        int stopFlag = 0;

        while (1) {
            //printf("Read byte from I2C1RCV: %8x\n", I2C1RCV);

            static unsigned int write_index = 0, read_index = 0;

            switch (state) {
            case START: // start bit has been sent
                startFlag = 1;
                write_index = 0; // reset indices
                read_index = 0;
                if (n_write > 0) { // there are bytes to write
                    state = WRITE; // transition to write mode
                    I2C1TRN = address << 1; // send the address, with write mode set
                } else {
                    state = ACK; // skip directly to reading
                    I2C1TRN = (address << 1) & 1;
                }
                break;

            case WRITE: // a write has finished
                writeFlag = 1;
                if (I2C1STATbits.ACKSTAT) { // error: didn?t receive an ACK from the slave
                    state = ERROR;
                } else {
                    if (write_index < n_write) { // still more data to write
                        I2C1TRN = to_write[write_index]; // write the data
                        ++write_index;
                    } else { // done writing data, time to read or stop
                        if (n_read > 0) { // we want to read so issue a restart
                            state = RESTART;
                            I2C1CONbits.RSEN = 1; // send the restart to begin the read
                        } else { // no data to read, issue a stop
                            state = STOP;
                            I2C1CONbits.PEN = 1;
                        }
                    }
                }
                break;

            case RESTART: // the restart has completed
                restartFlag = 1;
                // now we want to read, send the read address
                state = ACK; // when interrupted in ACK mode, we will initiate reading a byte
                I2C1TRN = (address << 1) | 1; // the address is sent with the read bit sent
                break;

            case READ:
                readFlag = 1;
                to_read[read_index] = I2C1RCV;
                ++read_index;
                if (read_index == n_read) { // we are done reading, so send a nack
                    state = NACK;
                    I2C1CONbits.ACKDT = 1;
                } else {
                    state = ACK;
                    I2C1CONbits.ACKDT = 0;
                }
                I2C1CONbits.ACKEN = 1;
                break;

            case ACK:
                // just sent an ack meaning we want to read more bytes
                state = READ;
                I2C1CONbits.RCEN = 1;
                break;

            case NACK:
                //issue a stop
                state = STOP;
                I2C1CONbits.PEN = 1;
                break;

            case STOP:
                state = IDLE; // we have returned to idle mode, indicating that the data is ready
                break;

            default:
                // some error has occurred
                state = ERROR;
            }
            IFS0bits.I2C1MIF = 0; //clear the interrupt flag
        }
    }
#endif

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

    MasterWriteI2C1((address << 1) | 0); //Sends the slave address over the I2C line.  This must happen first so the 
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