
/*
 * File:   BB_UART.c
 * Author: mdunne
 *
 * Created on November 10, 2011, 8:42 AM
 */

#include <xc.h>
#include "BB_Bluetooth.h"

#include "BB_BOARD.h"
#include <peripheral/uart.h>
#include <stdint.h>
#include <plib.h>



/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

#define F_PB (BB_BOARD_GetPBClock())
#define QUEUESIZE 1024
#define DESIRED_BAUD 115200

/*******************************************************************************
 * PRIVATE DATATYPES                                                           *
 ******************************************************************************/
typedef struct CircBuffer {
    unsigned char buffer[QUEUESIZE];
    int head;
    int tail;
    unsigned int size;
    unsigned char overflowCount;
} CircBuffer;
typedef struct CircBuffer* CBRef;


/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/
void newBlueToothBuffer(CBRef cB);
void freeBluetoothCircBuffer(CBRef* cB);
unsigned int getBuffLength(CBRef cB);
int readhead(CBRef cB);
int readtail(CBRef cB);
unsigned char Peak(CBRef cB);
unsigned char readfront(CBRef cB);
unsigned char writeback(CBRef cB, unsigned char data);

// Pavlo added:
//void BB_WriteUART1(const char * string);

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
struct CircBuffer outgoingUart;
CBRef transmitBuffer;
struct CircBuffer incomingUart;
CBRef receiveBuffer;
static uint8_t AddingToTransmit = FALSE;
static uint8_t GettingFromReceive = FALSE;
static uint8_t TransmitCollisionOccured = FALSE;
static uint8_t ReceiveCollisionOccured = FALSE;

/*******************************************************************************
 * Public VARIABLES                                                           *
 ******************************************************************************/
//char * string = "Hello World!";

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function BB_Bluetooth_Init(void)
 * @param none
 * @return none
 * @brief  Initializes the UART to work for the hc05 bluetooth module
 * @Michael Gonzalez  */

void BB_Bluetooth_Init(void)
{
    transmitBuffer = (struct CircBuffer*) &outgoingUart; //set up buffer for receive
    newBlueToothBuffer(transmitBuffer);

    receiveBuffer = (struct CircBuffer*) &incomingUart; //set up buffer for transmit
    newBlueToothBuffer(receiveBuffer);


    // Configure UART
    UARTConfigure(UART2, UART_ENABLE_PINS_TX_RX_ONLY); // changed from 0x00
    UARTSetDataRate(UART2, F_PB, 9600);
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_TX | UART_RX));
    //    From PLIB UART: Enabling the UART transmitter may cause an immediate UART TX interrupt
    //    request (if the UART TX interrupt is enabled), unless the transmit buffer
    //    has been pre-loaded with data.

    // Configure UART Interrupts
    U2TXREG = 'A'; // put a bit in the TX FIFO to allow it to stop interrupting since done will happen
    UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    INTSetVectorPriority(INT_UART_2_VECTOR, INT_PRIORITY_LEVEL_4); //set the interrupt priority
    INTEnable(INT_U2RX, INT_ENABLED);
    INTEnable(INT_U2TX, INT_ENABLED);
}



/**
 * @Function GetChar(void)
 * @param None.
 * @return ch - char from the serial port
 * @brief  reads first character from buffer or returns 0 if no chars available
 * */
char GetCommand(void)
{
    char ch;
    if (getBuffLength(receiveBuffer) == 0) {
        ch = 0;
    } else {
        GettingFromReceive = TRUE;
        ch = readfront(receiveBuffer);
        GettingFromReceive = FALSE;
    }
    //re-enter the interrupt if we added a character while transmitting another one
    if (ReceiveCollisionOccured) {
        INTSetFlag(INT_U2RX);
        ReceiveCollisionOccured = FALSE;
    }
    return ch;
}




/**
 * @Function IsReceiveEmpty(void)
 * @param None.
 * @return TRUE or FALSE
 * @brief  returns the state of the receive buffer
 * */
char Is_ReceiveEmpty(void)
{
    if (getBuffLength(receiveBuffer) == 0)
        return TRUE;
    return FALSE;
}

/**
 * @Function IsTransmitEmpty(void)
 * @param None.
 * @return TRUE or FALSE
 * @brief  returns the state of the receive buffer
 * */
char Is_transmitEmpty(void)
{
    if (getBuffLength(transmitBuffer) == 0)
        return TRUE;
    return FALSE;
}

/****************************************************************************
 Function
    IntUart1Handler

 Parameters
    None.

 Returns
    None.

 Description
    Interrupt Handle for the uart. with the PIC32 architecture both send and receive are handled within the same interrupt

 Notes


 Author
 Max Dunne, 2011.11.10
 ****************************************************************************/
void __ISR(_UART2_VECTOR, ipl4auto) IntUart2Handler(void)
{
    if (INTGetFlag(INT_U2RX)) {
        //PORTWrite(IOPORT_G, BIT_12);
        INTClearFlag(INT_U2RX);
        if (!GettingFromReceive) {
            writeback(receiveBuffer, (unsigned char) U2RXREG);
        } else {
            //acknowledge we have a collision and return
            ReceiveCollisionOccured = TRUE;
        }
    }
    if (INTGetFlag(INT_U2TX)) {
        PORTWrite(IOPORT_G, BIT_15);

        if (!(getBuffLength(transmitBuffer) == 0)) {
            if (!AddingToTransmit) {

                U2TXREG = readfront(transmitBuffer);

            } else {
                //acknowledge we have a collision and return
                TransmitCollisionOccured = TRUE;
            }
        } else if (getBuffLength(transmitBuffer) == 0) {
            PORTWrite(IOPORT_G, BIT_14);
            INTEnable(INT_U2TX, INT_DISABLED);
            INTClearFlag(INT_U2TX);
        }
    }
}

/*******************************************************************************
 * PRIVATE FUNCTIONS                                                          *
 ******************************************************************************/

void newBlueToothBuffer(CBRef cB)
{

    // initialize to zero
    int i;
    for (i = 0; i < QUEUESIZE; i++) {
        cB->buffer[i] = 0;
    }

    // initialize the data members
    cB->head = 0;
    cB->tail = 0;
    cB->size = QUEUESIZE;
    cB->overflowCount = 0;

}

// this function frees the Circular Buffer CB Ref

void freeBluetoothCircBuffer(CBRef* cB)
{
    // if it is already null, nothing to free
    if (cB == NULL || *cB == NULL) {
        return;
    }

    // free and nil the pointer
    //free(*cB);
    *cB = NULL;
}




// Accesor Methods
// ===============

// returns the amount of unread bytes in the circular buffer

unsigned int getBuffLength(CBRef cB)
{
    // if the circular buffer is not null
    if (cB != NULL) {
        if (cB->head <= cB->tail) {
            return (cB->tail - cB->head);
        } else {
            return (cB->size + cB->tail - cB->head);
        }
    } else {
        return 0;
    }


}

// returns the actual index of the head

int readhead(CBRef cB)
{
    // if the circular buffer is not null
    if (cB != NULL) {
        return (cB->head);
    } else {
        return 0;
    }

}

// returns the actual index of the tail

int readtail(CBRef cB)
{
    // if the circular buffer is not null
    if (cB != NULL) {
        return (cB->tail);
    } else {
        return 0;
    }

}

// returns the byte (actual value) that the head points to. this
// does not mark the byte as read, so succesive calls to peak will
// always return the same value

unsigned char Peak(CBRef cB)
{
    // if the circular buffer is not null
    if (cB != NULL) {
        // if there are bytes in the buffer
        if (getBuffLength(cB) > 0) {
            return cB->buffer[cB->head];
        }
    }
    return 0;
}


// Manipulation Procedures
// ======================
// returns the front of the circular buffer and marks the byte as read

unsigned char readfront(CBRef cB)
{
    // if the circular buffer is not null
    if (cB != NULL) {
        char retVal;
        // if there are bytes in the buffer
        if (getBuffLength(cB) > 0) {
            retVal = cB->buffer[cB->head];
            cB->head = cB->head < (cB->size - 1) ? cB->head + 1 : 0;
            return retVal;
        }
        return 128;
    }
    return 254;
}

// writes one byte at the end of the circular buffer,
// increments overflow count if overflow occurs

unsigned char writeback(CBRef cB, unsigned char data)
{
    // if the circular buffer is not null
    if (cB != NULL) {
        if (getBuffLength(cB) == (cB->size - 1)) {
            cB->overflowCount++;
            //return 1;
        } else {
            cB->buffer[cB->tail] = data;
            cB->tail = cB->tail < (cB->size - 1) ? cB->tail + 1 : 0;
            //return 0;
        }
        //return 0;
    } else {
        return 1;
    }
    return 0;
}

// empties the circular buffer. It does not change the size. use with caution!!

void makeempty(CBRef cB)
{
    if (cB != NULL) {
        int i;
        for (i = 0; i < cB->size; ++i) {
            cB->buffer[i] = 0;
        }
        cB->head = 0;
        cB->tail = 0;
        cB->overflowCount = 0;
    }
}

// returns the amount of times the CB has overflown;

unsigned char getoverflow(CBRef cB)
{
    if (cB != NULL) {
        return cB->overflowCount;
    }
    return 0;
}



