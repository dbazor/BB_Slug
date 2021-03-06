/*
 * File:   BB_I2C.c
 * Author: BB Team
 *
 * 
 */
// I2C Master utilities, 100 kHz, using polling rather than interrupts
// The functions must be callled in the correct order as per the I2C protocol
// Master will use I2C1 SDA1 (D9) and SCL1 (D10)
// Connect these through resistors to Vcc (3.3 V). 2.4k resistors recommended,
// but something close will do.
// Connect SDA1 to the SDA pin on the slave and SCL1 to the SCL pin on a slave

#include "BB_I2C.h"
#include "BB_BOARD.h"
#include <plib.h>
#include "BNO55_Register_Map.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * Private Support Functions                                                   *
 ******************************************************************************/

/**
 * Function: StartTransfer(BOOL restart)
 * @param   restart - If FALSE, send a "Start" condition
 *                  - If TRUE, send a "Restart" condition
 * @return  TRUE    - If successful
 *          FALSE   - If a collision occured during Start signaling
 * @precondition    The I2C module must have been initialized.
 * @brief   Starts (or restarts) a transfer to/from the EEPROM. This is a blocking 
 *      routine that waits for the bus to be idle and the Start (or Restart) signal
 *      to complete.
 * @example <code>
 *              StartTransfer(FALSE);
 *          </code>
 * @author Daniel Bazor */
BOOL StartTransfer(BOOL restart)
{
    I2C_STATUS status;

    // Send the Start (or Restart) signal
    if (restart) {
        I2CRepeatStart(BNO55_I2C_BUS);
    } else {
        // Wait for the bus to be idle, then start the transfer
        while (!I2CBusIsIdle(BNO55_I2C_BUS));

        if (I2CStart(BNO55_I2C_BUS) != I2C_SUCCESS) {
            printf("Error: Bus collision during transfer Start\n");
            return FALSE;
        }
    }

    // Wait for the signal to complete
    do {
        status = I2CGetStatus(BNO55_I2C_BUS);

    } while (!(status & I2C_START));

    return TRUE;
    //    I2C_STATUS status;
    //    //    printf("Start Transfer function\n");
    //    // Send the Start (or Restart) signal
    //    if (restart) {
    //
    //        //            printf("1bus not idle!\n");
    //
    //        I2CRepeatStart(BNO55_I2C_BUS);
    //    } else {
    //        // Wait for the bus to be idle, then start the transfer
    //        while (!I2CBusIsIdle(BNO55_I2C_BUS)) {
    //            //printf("2bus not idle!\n");
    //        }
    //
    //        while (I2CStart(BNO55_I2C_BUS) != I2C_SUCCESS) {
    //            printf("Error: Bus collision during transfer Start\n");
    //            return FALSE;
    //        }
    //    }
    //
    //    // Wait for the signal to complete
    //    do {
    //        status = I2CGetStatus(BNO55_I2C_BUS);
    //        //printf("do while in start \n");
    //    } while (!(status & I2C_START));
    //
    //    //printf("out of do while\n");
    //    return TRUE;
}

/**
 * Function: TransmitOneByte(UINT8 data)
 * @param   data    - Data byte to transmit 
 * @return  TRUE    - Data was sent successfully
 *          FALSE   - A bus collision occurred
 * @brief   This transmits one byte to the EEPROM. This transmits one byte to the 
 *      EEPROM, and reports errors for any bus collisions. This is a blocking 
 *      routine that waits for the transmission to complete.
 * @author Daniel Bazor */
BOOL TransmitOneByte(UINT8 data)
{
    // Wait for the transmitter to be ready
    while (!I2CTransmitterIsReady(BNO55_I2C_BUS));

    // Transmit the byte
    if (I2CSendByte(BNO55_I2C_BUS, data) == I2C_MASTER_BUS_COLLISION) {
        printf("Error: I2C Master Bus Collision\n");
        return FALSE;
    }

    // Wait for the transmission to finish
    while (!I2CTransmissionHasCompleted(BNO55_I2C_BUS));

    return TRUE;
}


/**
 * Function: StopTransfer(void)
 * @param   data    - Data byte to transmit 
 * @return  TRUE    - Data was sent successfully
 *          FALSE   - A bus collision occurred
 * @precondition
 *          The I2C module must have been initialized & a transfer started.
 * @brief   Stops a transfer to/from the EEPROM. This routine Stops a transfer 
 *      to/from the EEPROM, waiting (in a blocking loop) until the Stop condition 
 *      has completed. This is a blocking routine that waits for the Stop signal 
 *      to complete.
 * @author Daniel Bazor */
void StopTransfer(void)
{
    I2C_STATUS status;
    // Send the Stop signal
    I2CStop(BNO55_I2C_BUS);

    // Wait for the signal to complete
    do {
        status = I2CGetStatus(BNO55_I2C_BUS);

    } while (!(status & I2C_STOP));
}

/*******************************************************************************
 * Public Functions                                                           *
 ******************************************************************************/


/**
 * Function: BB_I2C_Init()
 * @param None
 * @return None
 * @brief Init function for I2C1. Sets clock and enables I2C1
 * @author Daniel Bazor */
void BB_I2C_Init()
{
    // Configure I2C1
    I2CConfigure(I2C1, I2C_ENABLE_SLAVE_CLOCK_STRETCHING | I2C_ENABLE_HIGH_SPEED);

    // Set the I2C baudrate

    UINT32 actualClock = I2CSetFrequency(BNO55_I2C_BUS, GetPeripheralClock(), I2C_CLOCK_FREQ);
  
    if (abs(actualClock - I2C_CLOCK_FREQ) > I2C_CLOCK_FREQ / 10) {

        printf("Error: I2C1 clock frequency (%u) error exceeds 10%%.\n", (unsigned) actualClock);
    }

    // Enable the I2C bus
    I2CEnable(BNO55_I2C_BUS, TRUE);

}

/**
 * Function: BB_I2C_Write()
 * @param   s_addr  - Slave address
 *          r_addr  - Data address
 * @return  TRUE    - Data was sent successfully
 *          FALSE   - There was a problem with the transfer
 * @brief   Write one Byte to the I2C1 Bus. Writes to the slave at s_addr at 
 *      the location r_addr the data at *dat
 * @example <code>
 *              success = BB_I2C_Write(UINT8 s_addr, UINT8 r_addr, UINT8 &dat);
 *          </code>
 * @author Daniel Bazor */
BOOL BB_I2C_Write(UINT8 s_addr, UINT8 r_addr, UINT8 *dat)
{
    // Initialize the data buffer
    int Index;
    int DataSz;
    UINT8 i2cData[2];
    I2C_7_BIT_ADDRESS SlaveAddress;
    printf(" read address is %x \n", r_addr);
    I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, s_addr, I2C_WRITE);

    i2cData[0] = SlaveAddress.byte;
    i2cData[1] = r_addr; // location to be read from
    i2cData[2] = *dat; // data to be sent
    DataSz = 3;
    printf("Starting Write\n");

    // Start the transfer to read the EEPROM.
    while (!StartTransfer(FALSE)) {
        printf("Error: StartTransfer failed!\n");
        StopTransfer();
        //return FALSE;
    }
    //printf("StartTransfer successful.\n");
    // Transmit all data
    Index = 0;
    while (Index < DataSz) {
        // Transmit a byte
        if (TransmitOneByte(i2cData[Index])) {
            // Advance to the next byte

            //printf("byte transmitted.\n");
            Index++;

            // Verify that the byte was acknowledged
            //printf("Waiting for ACK\n");
            if (!I2CByteWasAcknowledged(BNO55_I2C_BUS)) {
                printf("Error: Sent byte was not acknowledged\n");
                I2CStop(BNO55_I2C_BUS);
                return FALSE;
            }

        } else {
            printf("Error: Transmit one byte failed!\n");
            I2CStop(BNO55_I2C_BUS);
            return FALSE;
        }
    }
    // End the transfer (stop here if an error occured)
    StopTransfer();
}


/**
 * Function: BB_I2C_Read(UINT8 s_addr, UINT8 r_addr, UINT8 *dat)
 * @param   s_addr  - Slave address
 *          r_addr  - Data address
 *          *dat    - where to read data to
 * @return  TRUE    - Data was sent successfully
 *          FALSE   - There was a problem with the transfer
 * @brief   Read one Byte from the I2C1 Bus. Reads from the slave at s_addr 
 *          at the location r_addr the data at *dat
 * @example <code>
 *              success = BB_I2C_Read(UINT8 s_addr, UINT8 r_addr, UINT8 &dat);
 *          </code>
 * @author Daniel Bazor */
BOOL BB_I2C_Read(UINT8 s_addr, UINT8 r_addr, UINT8 * dat)
{
    // Initialize the data buffer
    int Index;
    int DataSz;
    UINT8 i2cData[2];
    I2C_7_BIT_ADDRESS SlaveAddress;

    I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, s_addr, I2C_WRITE);

    i2cData[0] = SlaveAddress.byte;
    i2cData[1] = r_addr; // location to be read from
    DataSz = 2;
    printf(" read address is %x \n", r_addr);

    // Start the transfer to read the EEPROM.
    if (!StartTransfer(FALSE)) {
        printf("Error: StartTransfer failed!\n");
        I2CStop(BNO55_I2C_BUS);
        return FALSE;
    }

    // Address desired Slave
    Index = 0;
    while (Index < DataSz) {
        // Transmit a byte
        if (TransmitOneByte(i2cData[Index])) {
            // Advance to the next byte
            Index++;

            if (!I2CByteWasAcknowledged(BNO55_I2C_BUS)) {
                printf("Error: Sent byte was not acknowledged\n");
                I2CStop(BNO55_I2C_BUS);
                return FALSE;
            }
        } else {
            printf("Error: Transmit one byte failed1\n");
            I2CStop(BNO55_I2C_BUS);
            return FALSE;
        }
    }

    // Restart and send the EEPROM's internal address to switch to a read transfer

    // Send a Repeated Started condition
    if (!StartTransfer(TRUE)) {
        printf("Error: ReStartTransfer failed!\n");
        I2CStop(BNO55_I2C_BUS);
        return FALSE;
    }

    // Transmit the address with the READ bit set
    I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, s_addr, I2C_READ);
    if (TransmitOneByte(SlaveAddress.byte)) {
        // Verify that the byte was acknowledged
        if (!I2CByteWasAcknowledged(BNO55_I2C_BUS)) {
            printf("Error: Sent byte was not acknowledged\n");
            I2CStop(BNO55_I2C_BUS);
            return FALSE;
        }

    } else {
        printf("Error: Change to read failed! \n");
        I2CStop(BNO55_I2C_BUS);
        return FALSE;
    }


    // Read the data from the desired address

    if (I2CReceiverEnable(BNO55_I2C_BUS, TRUE) == I2C_RECEIVE_OVERFLOW) {
        printf("Error: I2C Receive Overflow\n");
        I2CStop(BNO55_I2C_BUS);
        return FALSE;
    } else {
        // read in one byte
        while (!I2CReceivedDataIsAvailable(BNO55_I2C_BUS));
        *dat = I2CGetByte(I2C1); // Read the data 
        I2CAcknowledgeByte(I2C1, FALSE); // (NO acknowledge)
    }

    // End the transfer (stop here if an error occured)

    while (!I2CAcknowledgeHasCompleted(BNO55_I2C_BUS));
    StopTransfer();
}


/**
 * Function: BB_I2C_Read_Multi(UINT8 s_addr, UINT8 r_addr, UINT8 *dat)
 * @param   s_addr  - Slave address
 *          r_addr  - Data address
 *          *dat    - where to read data to
 *          len     - length of data to read
 * @return  TRUE    - Data was sent successfully
 *          FALSE   - There was a problem with the transfer
 * @brief   Read multiple Byte from the I2C1 Bus Reads from the slave at s_addr 
 *          at the location r_addr the data at *dat
 * @example <code>
 *              success = BB_I2C_Read_Multi(UINT8 s_addr, UINT8 r_addr, len, UINT8 &dat);
 *          </code>
 * @author Daniel Bazor */
BOOL BB_I2C_Read_Multi(UINT8 s_addr, UINT8 r_addr, UINT8 len, UINT8 * dat)
{
    // Initialize the data buffer
    int Index;
    int DataSz;
    UINT8 i2cData[2];
    I2C_7_BIT_ADDRESS SlaveAddress;

    I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, s_addr, I2C_WRITE);

    i2cData[0] = SlaveAddress.byte;
    i2cData[1] = r_addr; // location to be read from
    DataSz = 2;
    printf(" read address is %x \n", r_addr);

    // Start the transfer to read the EEPROM.
    if (!StartTransfer(FALSE)) {
        printf("Error: StartTransfer failed!\n");
        I2CStop(BNO55_I2C_BUS);
        return FALSE;
    }

    // Address desired Slave
    Index = 0;
    while (Index < DataSz) {
        // Transmit a byte
        if (TransmitOneByte(i2cData[Index])) {
            // Advance to the next byte
            Index++;

            if (!I2CByteWasAcknowledged(BNO55_I2C_BUS)) {
                printf("Error: Sent byte was not acknowledged\n");
                I2CStop(BNO55_I2C_BUS);
                return FALSE;
            }
        } else {
            printf("Error: Transmit one byte failed1\n");
            I2CStop(BNO55_I2C_BUS);
            return FALSE;
        }
    }

    // Restart and send the EEPROM's internal address to switch to a read transfer

    // Send a Repeated Started condition
    if (!StartTransfer(TRUE)) {
        printf("Error: ReStartTransfer failed!\n");
        I2CStop(BNO55_I2C_BUS);
        return FALSE;
    }


    // Transmit the address with the READ bit set
    I2C_FORMAT_7_BIT_ADDRESS(SlaveAddress, s_addr, I2C_READ);
    if (TransmitOneByte(SlaveAddress.byte)) {
        // Verify that the byte was acknowledged
        if (!I2CByteWasAcknowledged(BNO55_I2C_BUS)) {
            printf("Error: Sent byte was not acknowledged\n");
            I2CStop(BNO55_I2C_BUS);
            return FALSE;
        }

    } else {
        printf("Error: Change to read failed! \n");
        I2CStop(BNO55_I2C_BUS);
        return FALSE;
    }

    // Read the data from the desired address

    //    if (I2CReceiverEnable(BNO55_I2C_BUS, TRUE) == I2C_RECEIVE_OVERFLOW) {
    //        printf("Error: I2C Receive Overflow\n");
    //        I2CStop(BNO55_I2C_BUS);
    //        return FALSE;
    //    } else {
    //        // read in one byte
    //        while (!I2CReceivedDataIsAvailable(BNO55_I2C_BUS));
    //        *dat++ = I2CGetByte(I2C1); // Read the data 
    //        I2CAcknowledgeByte(I2C1, TRUE); // (NO acknowledge)
    //    }
    //    
    //        // Read the data from the desired address
    //
    //    if (I2CReceiverEnable(BNO55_I2C_BUS, TRUE) == I2C_RECEIVE_OVERFLOW) {
    //        printf("Error: I2C Receive Overflow\n");
    //        I2CStop(BNO55_I2C_BUS);
    //        return FALSE;
    //    } else {
    //        // read in one byte
    //        while (!I2CReceivedDataIsAvailable(BNO55_I2C_BUS));
    //        *dat = I2CGetByte(I2C1); // Read the data 
    //        I2CAcknowledgeByte(I2C1, FALSE); // (NO acknowledge)
    //    }
    //
    //    // End the transfer (stop here if an error occured)
    //
    //    while (!I2CAcknowledgeHasCompleted(BNO55_I2C_BUS));
    //    StopTransfer();

    // Read the data from the desired address

    if (I2CReceiverEnable(BNO55_I2C_BUS, TRUE) == I2C_RECEIVE_OVERFLOW) {
        printf("Error: I2C Receive Overflow\n");
        I2CStop(BNO55_I2C_BUS);
        return FALSE;
    } else {
        // read in all of the date to up to len-1
        for (Index = 0; Index < (len - 1); Index++) {
            //printf("top of for\n");

            while (!I2CReceivedDataIsAvailable(BNO55_I2C_BUS)) {
                printf("waiting for Data\n");
            }

            *dat++ = I2CGetByte(I2C1); // Read the data (acknowledge)
            I2CAcknowledgeByte(I2C1, TRUE);

            while (!I2CAcknowledgeHasCompleted(BNO55_I2C_BUS)) {
                printf("waiting for ack\n");
            }
        }
        while (!I2CReceivedDataIsAvailable(BNO55_I2C_BUS)) {
            printf("waiting for last Data\n");
        };
        *dat = I2CGetByte(I2C1); // Read the data 
        I2CAcknowledgeByte(I2C1, FALSE); // (NO acknowledge)
    }

    // End the transfer (stop here if an error occured)
    while (!I2CAcknowledgeHasCompleted(BNO55_I2C_BUS));
    StopTransfer();
}