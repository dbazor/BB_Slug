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
/* ------------------------------------------------------------ */
/*				Definitions										*/
/* ------------------------------------------------------------ */
#define cntMsDelay 10000

#include "BB_I2C.h"

// KEEP IN MIND THAT THE ADDRESS BYTE IS REALLY 7 BITS, THE ADDRESS IS SHIFTED TO THE LEFT BY 1 BIT
// FOR READ/WRITE
#define SLAVE_ADDR 0x28

/* ------------------------------------------------------------ */
/*				Prototypes										*/
/* ------------------------------------------------------------ */
void DeviceInit();
void DelayInit();
void DelayMs(int cms);
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
    DelayInit();
    DeviceInit();

    __builtin_disable_interrupts();
    //    i2c_slave_setup(SLAVE_ADDR); // init I2C5, which we use as a slave
    //    // (comment out if slave is on another pic)
    i2c_master_setup(); // init I2C2, which we use as a master
    __builtin_enable_interrupts();

    #if 0
    {
        char slave = (SLAVE_ADDR << 1); //
        i2c_master_start();             // Begin the start sequence
        i2c_master_send(slave);         // send the slave address, left shifted by 1,
    }
    #endif

    #if 1
    {
        while(1) {
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

    i2c_master_start();       // Begin the start sequence
    i2c_states state = START; // initialize state

    int startFlag = 0;
    int writeFlag = 0;
    int readFlag = 0;
    int restartFlag = 0;
    int readFlag= 0;
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

/* ------------------------------------------------------------ */

/***	DelayInit
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
 **		Initialized the hardware for use by delay functions. This
 **		initializes Timer 1 to count at 10Mhz.
/* ------------------------------------------------------------ */

void DelayInit()
{
    unsigned int tcfg;

    /* Configure Timer 1. This sets it up to count a 10Mhz with a period of 0xFFFF
     */
    tcfg = T1_ON | T1_IDLE_CON | T1_SOURCE_INT | T1_PS_1_8 | T1_GATE_OFF | T1_SYNC_EXT_OFF;
    OpenTimer1(tcfg, 0xFFFF);

}

/* ------------------------------------------------------------ */

/***	DelayMs
 **
 **	Parameters:
 **		cms			- number of milliseconds to delay
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Delay the requested number of milliseconds. Uses Timer1.
/* ------------------------------------------------------------ */

void DelayMs(int cms)
{
    int ims;

    for (ims = 0; ims < cms; ims++) {
        WriteTimer1(0);
        while (ReadTimer1() < cntMsDelay);
    }
}