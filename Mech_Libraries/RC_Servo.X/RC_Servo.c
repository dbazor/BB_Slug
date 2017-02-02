/*
 * File:   RC_Servo.c
 * Author: Gabriel Hugh Elkaim
 *
 * Created on August 14, 2013, 9:51 AM
 */

/******************************************************************************
 * Software License Agreement
 *
 * Copyright � 2011 Microchip Technology Inc.  All rights reserved.
 * Microchip licenses to you the right to use, modify, copy and distribute
 * Software only when embedded on a Microchip microcontroller or digital
 * signal controller, which is integrated into your product or third party
 * product (pursuant to the sublicense terms in the accompanying license
 * agreement).
 *
 * You should refer to the license agreement accompanying this Software
 * for additional information regarding your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED ìAS ISî WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY
 * OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION,
 * BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA,
 * COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY
 * CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
 * OR OTHER SIMILAR COSTS.
 *
 *****************************************************************************/

#include <BOARD.h>
#include <xc.h>
#include <plib.h>
#include <peripheral/int.h>

#include "RC_Servo.h"
#include <SERIAL.h>


/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

//#define RC_DEBUG_VERBOSE
#ifdef RC_DEBUG_VERBOSE
#define dbprintf(...) printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif
// uncomment if ok with short pulse when shutting down module
//#define SHUTDOWN_WITH_SHORT_PULSE_OK

#ifdef RC_SERVO_TEST
#define TEST_LED    LATFbits.LATF6
#define TIMER_LED   LATGbits.LATG6
#endif

#define RCPINCOUNT      10
#define SERVOCENTER     1500
#define RCPERIODTIME    (RCPINCOUNT * MAXPULSE)
#define FULLOVERFLOW    0xFFFF

#define RC_TRISX03  TRISFbits.TRISF5
#define RC_LATX03   LATFbits.LATF5
#define RC_TRISX04  TRISBbits.TRISB0
#define RC_LATX04   LATBbits.LATB0

#define RC_TRISY06  TRISDbits.TRISD10
#define RC_LATY06   LATDbits.LATD10
#define RC_TRISY07  TRISEbits.TRISE7
#define RC_LATY07   LATEbits.LATE7

#define RC_TRISZ08  TRISDbits.TRISD8
#define RC_LATZ08   LATDbits.LATD8
#define RC_TRISZ09  TRISEbits.TRISE1
#define RC_LATZ09   LATEbits.LATE1

#define RC_TRISV03  TRISBbits.TRISB2
#define RC_LATV03   LATBbits.LATB2
#define RC_TRISV04  TRISBbits.TRISB3
#define RC_LATV04   LATBbits.LATB3

#define RC_TRISW07  TRISBbits.TRISB15
#define RC_LATW07   LATBbits.LATB15
#define RC_TRISW08  TRISBbits.TRISB14
#define RC_LATW08   LATBbits.LATB14

/* Note that you need to set the prescalar and periferal clock appropriate to
 * the processor board that you are using. In order to calculate the minimal
 * prescalar: Prescalar = (2000*F_PB/(1000000*0xFFFF))+1, round down */

#define F_PB        (BOARD_GetPBClock())
#define F_PB_IN_KHZ (F_PB/1000)
#define PRESCALE    2
#define uSEC        (F_PB_IN_KHZ / (PRESCALE * 1000))
#define ALLRCPINS   0x3FF

/* Code readability Macros, information hiding */
#define RC_SetOutput(pin)   *RC_TRISCLR[pin] = rcBitsMap[pin]
#define RC_SetInput(pin)    *RC_TRISSET[pin] = rcBitsMap[pin]

#define RC_SetPin(pin)      *RC_LATSET[pin] = rcBitsMap[pin]
#define RC_ClearPin(pin)    *RC_LATCLR[pin] = rcBitsMap[pin]
#define RC_SetPeriod(pin)   WritePeriod4(ReadTimer4() + uSEC * RCupTime[pin])
#define RC_GetTime(pin)     RCupTime[pin]

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static volatile unsigned int * const RC_TRISCLR[] = {&TRISFCLR, &TRISBCLR, &TRISDCLR,
    &TRISECLR, &TRISDCLR, &TRISECLR,
    &TRISBCLR, &TRISBCLR, &TRISBCLR,
    &TRISBCLR};

static volatile unsigned int * const RC_TRISSET[] = {&TRISFSET, &TRISBSET, &TRISDSET,
    &TRISESET, &TRISDSET, &TRISESET,
    &TRISBSET, &TRISBSET, &TRISBSET,
    &TRISBSET};

static volatile unsigned int * const RC_LATSET[] = {&LATFSET, &LATBSET, &LATDSET,
    &LATESET, &LATDSET, &LATESET,
    &LATBSET, &LATBSET, &LATBSET,
    &LATBSET};
static volatile unsigned int * const RC_LATCLR[] = {&LATFCLR, &LATBCLR, &LATDCLR,
    &LATECLR, &LATDCLR, &LATECLR,
    &LATBCLR, &LATBCLR, &LATBCLR,
    &LATBCLR};
static unsigned short int rcBitsMap[] = {BIT_5, BIT_0, BIT_10, BIT_7, BIT_8,
    BIT_1, BIT_2, BIT_3, BIT_15, BIT_14};

unsigned char RCenabled = FALSE;

static enum {
    none, pulsing, idling
} RCstate = none;

static short int RCupTime[RCPINCOUNT];
static short int RCpinsActive = 0x0000;
static char numRCPins = 0;
static int uSecondsLeftToGo = RCPERIODTIME;
static volatile unsigned short int pinsToAdd = 0x0000;
static volatile unsigned short int pinsToRemove = 0x0000;

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/

void RC_InstallPins(void);
void RC_DeletePins(void);
void RC_ShutDown(void);

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function RC_Init(void)
 * @param none
 * @return SUCCESS or ERROR
 * @brief Initialize the RC module for use. Uses TIMER4 hardware and initializes
 *        the interrupt to occur every 50msec (20Hz)
 * @note Returns FAILURE if called and the module was already enabled.
 * @author Gabriel Hugh Elkaim, 2011.12.15 16:42 */
char RC_Init(void)
{
    char i;
    if (RCenabled) { // Error, initializing an already active module
        dbprintf("\nRC_Servo: ERROR - initialized already active module");
        return ERROR;
    }
    // Initialize the upTime array to all zeros
    for (i = 0; i < RCPINCOUNT; i++) {
        RCupTime[i] = 0;
    }
    // Set up Timer #4 to overflow at more than 2msec, enable and set interrupt
    OpenTimer4(T4_ON | T4_IDLE_STOP | T4_GATE_OFF | T4_PS_1_2 |
        T4_32BIT_MODE_OFF | T4_SOURCE_INT, SERVOCENTER * uSEC);
    // Initialize the interrupt module
    INTClearFlag(INT_T4);
    INTSetVectorPriority(INT_TIMER_4_VECTOR, 4);
    INTSetVectorSubPriority(INT_TIMER_4_VECTOR, 0);
    INTEnable(INT_T4, INT_ENABLED);

    // Module is initialized
    dbprintf("\nRC_Servo: module initialized");
    RCenabled = TRUE;
    RCstate = none;
    return SUCCESS;
}

/**
 * @Function RC_AddPins(unsigned short int RCpins)
 * @param RCpins, use #defined RC_PORTxxx OR'd together for each RC servo pin
 * @return SUCCESS or ERROR
 * @brief Enables an RC servo pin such that it will start the PWM on the
 *        next rollover of TIMER4 (20msec max). Will return ERROR if
 *        the pin is already enabled, or if RCpins is out of range. Input
 *        is set by bitwise ORing RC_PORTxYY of desired pins with each
 *        other: ie, RC_AddPins(RC_PORTX03 | RC_PORTZ09);
 * @note If no time has been set, servo will center (1500uSec)
 * @author Gabriel Hugh Elkaim, 2013.07.24 12:25 */
char RC_AddPins(unsigned short int RCpins)
{
    dbprintf("\nRC_Servo: adding pins...");
    // Check if inputs are in range, or if not yet initialized
    if ((RCpins == 0x000) || (RCpins > ALLRCPINS) || (!RCenabled)) {
        // error state, either module or pins not active, or out of range
        dbprintf("\nRC_Servo: add pin FAILED, input out of range or module not initialized");
        return ERROR;
    }
    if ((RCpins & RCpinsActive) || (RCpins & pinsToAdd)) {
        // error, adding already enabled pin (2nd call to add with same args)
        dbprintf("\nRC_Servo: add pin FAILED, pin already enabled");
        return ERROR;
    }
    pinsToAdd |= RCpins;
    return SUCCESS;
}

/**
 * @Function RC_RemovePins(unsigned short int RCpins)
 * @param RCpins - use #defined RC_PORTxxx OR'd together for each RC servo pin
 * @return SUCCESS or ERROR
 * @brief Disables a currently active RC servo pin such that it will shut-
 *        down on the next rollover of TIMER4 (20msec max). Function will
 *        return ERROR if the pin is already disabled, if RCpins is out of
 *        range, or if the RC module is not enabled. Input is set by bitwise
 *        ORing RC_PORTxYY of desired pins with each other: ie,
 *        RC_RemovePins(RC_PORTX03 | RC_PORTZ09);
 * @note Function will leave upTime for pin untouched, such that Removing
 *       and re-Adding a pin will not change the servo position.
 * @author Gabriel Hugh Elkaim, 2013.07.24 12:25 */
char RC_RemovePins(unsigned short int RCpins)
{
    dbprintf("\nRC_Servo: removing pins...");
    // Check if inputs are in range, or if not yet initialized
    if ((RCpins == 0x000) || (RCpins > ALLRCPINS) || (!RCenabled)) {
        // error state, either module or pins not active, or out of range
        dbprintf("\nRC_Servo: remove pin FAILED, input out of range or module not initialized");
        return ERROR;
    }
    if (!((RCpins & RCpinsActive) || (RCpins & pinsToRemove))) {
        // error, removing already disabled pin (2nd call to remove with same args)
        dbprintf("\nRC_Servo: remove pin FAILED, pin already disabled");
        return ERROR;
    }
    pinsToRemove |= RCpins;
    return SUCCESS;

}

/**
 * @Function RC_ListPins(void)
 * @param none
 * @return RCpins - the mask of the pins that are currently active
 * @brief Returns a short with a 1 in each bit for each of the RC_PORTxxx that
 *        is enabled. Returns 0x0000 if no pins are enabled.
 * @note To verify RC_PORTxxx is enabled, test (RC_PORTxxx && RcListPins()), if
 *       the result is FALSE, then the pin is currently disabled.
 * @author Gabriel Hugh Elkaim, 2013.07.25 16:05 */
unsigned short int RC_ListPins(void)
{
    return (RCpinsActive);
}

/**
 * @Function RcSetPulseTime(unsigned short int RCpin, unsigned short int pulseTime)
 * @param RCpin - use #defined RC_PORTxxx (only one)
 * @param pulseTime - pulse width in uSeconds from [1000 to 2000]
 * @return SUCCESS or ERROR
 * @brief Sets the up time of the corresponding RC_PORTxxx pin to the value
 *        in pulseTime in uSec [1000 to 2000 nominally]. Checks for valid
 *        inputs for RCpin and ranges for pulseTime. Returns ERROR if the
 *        pin is not currently active.
 * @author Gabriel Hugh Elkaim, 2011.12.15 16:42 */
char RC_SetPulseTime(unsigned short int RCpin, unsigned short int pulseTime)
{
    char i;
    if ((pulseTime < MINPULSE) || (pulseTime > MAXPULSE)) {
        // error state, input out of range
        dbprintf("\nRC_Servo: Set Pulse FAILED, pulse time out of range");
        return ERROR;
    }
    if ((RCpin == 0x000) || (RCpin > ALLRCPINS) || (!RCenabled)) {
        // error state, either module or pins not active, or out of range
        dbprintf("\nRC_Servo: Set Pulse FAILED, out of bounds or module inactive");
        return ERROR;
    }
    // check if pin currently active, either it is already active or pending
    if ((RCpin & RCpinsActive) || (RCpin & pinsToAdd)) {
        i = 0;
        while (RCpin > 1) { // find position of 1 in RCpin
            RCpin >>= 1;
            i++;
        }
        dbprintf("\nRC_Servo: Set Pulse for pin %d at %d uSec", i, pulseTime);
        RCupTime[i] = pulseTime;
        return SUCCESS;
    }
    // error state, pin not active
    dbprintf("\nRC_Servo: Set Pulse FAILED, pin inactive");
    return ERROR;
}

/**
 * @Function RcGetPulseTime(unsigned short int RCpin)
 * @param RCpin - use #defined RC_PORTxxx (only one)
 * @return pulseTime - Pulse width in uSeconds
 * @brief  Returns the current value of the pulse width for the corresponding
 *         RC_PORTxxx pin. Checks for valid input range for RCpin and returns
 *         ERROR if input out of range.
 * @author Gabriel Hugh Elkaim, 2011.12.15 16:42 */
short int RC_GetPulseTime(unsigned short int RCpin)
{
    char i;
    if (!RCenabled) { // fails, module not enabled
        dbprintf("\nRC_Servo: Get Pulse FAILED, module not enabled");
        return ERROR;
    }
    if ((RCpin == 0x000) || (RCpin > ALLRCPINS)) { // fails, pin out of range
        dbprintf("\nRC_Servo: Get Pulse FAILED, pin out of range");
        return ERROR;
    }
    i = 0;
    while (RCpin > 1) { // find position of 1 in RCpin
        RCpin >>= 1;
        i++;
    }
    return (RCupTime[i]);
}

/**
 * @Function RC_ChangePending(void)
 * @param none
 * @return TRUE or FALSE
 * @brief Returns TRUE if pending RC pin changes (Add or Remove) are
 *        still pending, FALSE otherwise.
 * @author Gabriel Hugh Elkaim, 2013.08.18 21:56 */
char RC_ChangePending(void)
{
    // check if either module variable has pending operations
    if ((pinsToAdd == 0) && (pinsToRemove == 0)) {
        return FALSE;
    }
    return TRUE;
}

/**
 * @Function RcEnd(void)
 * @param none
 * @return SUCCESS or ERROR
 * @brief Shuts down the RC_SERVO module. Sets flag so that TIMER4 will
 *        turn off all pins once the idle state of the timer is reached.
 *        All enabled RC_PORTxxx pins that are enabled will be turned into
 *        digital inputs.
 * @author Gabriel Hugh Elkaim, 2011.12.15 16:42 */
char RC_End(void)
{
    if (!RCenabled) {
        // error state, module not active
        dbprintf("\nRC_SERVO: end FAILED, module already shut down");
        return ERROR;
    }
    // note that all shutdown will actually occur in T4 ISR routine on the next
    // T4 rollover unless SHUTDOWN_WITH_SHORT_PULSE is defined, in which case
    // it will happen immediately.
    RCenabled = FALSE;

#ifdef SHUTDOWN_WITH_SHORT_PULSE_OK
    RC_ShutDown();
#endif
    return SUCCESS;
}

/*******************************************************************************
 * PRIVATE FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function RC_InstallPins(void) -- PRIVATE FUNCTION
 * @param none
 * @return none
 * @brief Goes through pinToAdd and sets pins as outputs, clears pins, and
 *        sets the servo to centered if not previously used. Inserts pins
 *        into RCpinsActive.
 * @author Gabriel Hugh Elkaim, 2013.08.16, 23:26 */
void RC_InstallPins(void)
{
    char i;
    unsigned short curPin;
    if (pinsToAdd) { // there are new pins to add
        // Go through input and set each RC pin direction and force to low
        for (i = 0; i < RCPINCOUNT; i++) {
            curPin = (1 << i);
            if (pinsToAdd & curPin) {
                numRCPins++;
                if (RCupTime[i] == 0) {
                    RCupTime[i] = SERVOCENTER;
                }
                RC_SetOutput(i); // Sets pin direction to output
                RC_ClearPin(i); // Forces pin to low state
                dbprintf("\nEnabling pin: 0x%X", curPin);
            }
        }
        RCpinsActive |= pinsToAdd;
        pinsToAdd = 0x000;
    }
}

/**
 * @Function RC_DeletePins(void) -- PRIVATE FUNCTION
 * @param none
 * @return none
 * @brief Goes through pinToRemove and sets pin as input, clears pin.
 *        Removes pins from RCpinsActive.
 * @author Gabriel Hugh Elkaim, 2013.08.16, 23:26 */
void RC_DeletePins(void)
{
    char i;
    unsigned short curPin;
    if (pinsToRemove) { // there are new pins to remove
        // Go through input and set each RC pin to input and force to low
        for (i = 0; i < RCPINCOUNT; i++) {
            curPin = (1 << i);
            if (pinsToRemove & curPin) {
                numRCPins--;
                RC_ClearPin(i); // Forces pin to low state
                RC_SetInput(i); // Set pin as input
                dbprintf("\nRemoving pin: 0x%X", curPin);
            }
        }
        RCpinsActive &= ~pinsToRemove;
        pinsToRemove = 0;
    }
}

/**
 * @Function RC_ShutDown(void) -- PRIVATE FUNCTION
 * @param none
 * @return none
 * @brief Removes all pins in RCpinsActive, resets the RCupTime array to 0
 *        clears all previously active pins and sets them as inputs. Shuts
 *        down the TIMER4 interrupt.
 * @author Gabriel Hugh Elkaim, 2013.08.16, 23:26 */
void RC_ShutDown(void)
{
    char i;
    if (RCpinsActive) {
        pinsToRemove = RCpinsActive;
        RC_DeletePins();
    }
    numRCPins = 0;
    for (i = 0; i < RCPINCOUNT; i++) {
        RCupTime[i] = 0;
    }
    INTEnable(INT_T4, INT_DISABLED);
}


/*******************************************************************************
 * INTERRUPT SERVICE ROUTINES                                                  *
 ******************************************************************************/

/**
 * @Function Timer4IntHandler -- ISR ROUTINE
 * @param none
 * @return none
 * @brief Implements the RC_Servo state machine through the TIMER4 ISR.
 *        Raises and Lowers the appropriate RC pins, and implements a
 *        countdown to get to the 20msec repetition rate.
 * @note Adding and Removing pins are handled within the idling state, after
 *       all pins have gone through their Pulse. Shutdown also occurs in this
 *       state. Potential for idle state to not have any time left, if all 10
 *       servos are enabled and at their largest value.
 * @author Gabriel Hugh Elkaim, 2013.08.16, 23:26 */

void __ISR(_TIMER_4_VECTOR, IPL4AUTO) Timer4IntHandler(void)
{
    static char numPin = 0;
    static unsigned short int curPin = 0x001;
    static char indexPin = 0;
    static char prevPin = -1;
    unsigned short int currentTime;

    INTClearFlag(INT_T4);

    // check for shutdown command, and lower all active pins and set to input
    if (!RCenabled) {
        RC_ShutDown();
        return;
    }

    switch (RCstate) {
    case none:
        // no pins active. Reset for 1.5msec, check for pins to add
        WritePeriod4(ReadTimer4() + uSEC * SERVOCENTER);
        if (pinsToAdd) {
            RC_InstallPins(); // new pins active, reset to start pulses
            curPin = 0x001;
            indexPin = 0;
            numPin = 1;
            RCstate = pulsing;
        }
        break;

    case pulsing:
        // cycle through all active pins, one each interrupt, raising the current
        // and lowering the previous ones until you have done them all
        // Find next pin in RCpinsActive
        while (!(curPin & RCpinsActive)) {
            curPin <<= 1;
            indexPin++;
        }
        uSecondsLeftToGo -= RC_GetTime(indexPin);
        RC_SetPin(indexPin);
        RC_SetPeriod(indexPin);
        if (numPin != 1) { // not on first pin, lower previous pin
            RC_ClearPin(prevPin);
        }
        prevPin = indexPin;
        curPin <<= 1;
        indexPin++;
        numPin++;
        if (numPin > numRCPins) { // finished all pins, go to idle state
            RCstate = idling;

        }
        break;


    case idling:
        // lower previous pin and reset pin counters to keep track of position
        RC_ClearPin(prevPin);
        currentTime = ReadTimer4();

        if (uSecondsLeftToGo <= 0) {
            RCstate = pulsing;
            INTSetFlag(INT_T4);
        } else if ((uSecondsLeftToGo * uSEC + currentTime) < FULLOVERFLOW) {
            WritePeriod4(currentTime + uSEC * uSecondsLeftToGo);
            RCstate = pulsing;
        } else { // need to cycle through
            uSecondsLeftToGo -= MAXPULSE;
            WritePeriod4(currentTime + uSEC * MAXPULSE);
        }

        if (RCstate == pulsing) {// switching back from idle to pulsing
            uSecondsLeftToGo = RCPERIODTIME;
            curPin = 0x001;
            indexPin = 0;
            numPin = 1;
        }

        if (pinsToAdd) RC_InstallPins();
        if (pinsToRemove) RC_DeletePins();
        if (numRCPins == 0) {
            uSecondsLeftToGo = RCPERIODTIME;
            RCstate = none;
        }
        break;

    default:
        dbprintf("\nRC_Servo: ERROR, module in undefined state");
        break;
    }
}



/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/
#ifdef RC_SERVO_TEST

#include <xc.h>
#include <SERIAL.h>
#include "RC_Servo.h"
#include <stdio.h>


#define DELAY(x)    for (wait = 0; wait <= x; wait++) {asm("nop");}
#define FlushPrintBuffer() //while(!IsTransmitEmpty())
#define A_BIT       18300
#define A_LOT       183000
#define NUM2SWEEP   10
#define NUM2SPIN    2
#define INC         100
#define RC_PORT     RC_PORTX04

int main(void)
{
    unsigned int wait = 0;
    unsigned short int pulse;
    short int returnPulse;
    unsigned short int i, j, k;
    char testPassed = FALSE;
    int8_t wantedResult = SUCCESS;
    DELAY(A_BIT);
    BOARD_Init();
    DELAY(A_LOT);
    printf("\nCMPE118 RC_SERVO module test harness.");
    printf("\nTiming information: PBClock %d (%3.2f Mhz), Counts per uSec: %d",
        F_PB, F_PB_IN_KHZ / 1000.0, uSEC);
    printf("\nInitial Test, ensure all functions return ERROR "
        "before RC_Init() is run\n");


    /***************************************************************************
     *            TEST RC MODULE ERROR CHECKING BEFORE ENABLE                  *
     ***************************************************************************/
    //
    // RC_AddPins() should FAIL before enable
    //
    printf("\nTesting: RC_AddPins() for each pin before enable: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        if (RC_AddPins(i) == ERROR) {
            while (RC_ChangePending()) { // wait until change occured
                DELAY(A_BIT);
            }
        } else {
            testPassed = FALSE;
            printf("\nRC_AddPins(0x%03x) returned SUCCESS: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");
    FlushPrintBuffer();

    //
    // RC_RemovePins() should FAIL before enable
    //
    printf("\nTesting: RC_RemovePins() for each pin before enable: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        if (RC_RemovePins(i) == ERROR) {

            while (RC_ChangePending()) { // wait until change occured
                DELAY(A_BIT);
            }
        } else {
            testPassed = FALSE;
            printf("\nRC_RemovePins(0x%03x) returned SUCCESS: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");
    FlushPrintBuffer();

    //
    // RC_SetPulseTime() should FAIL before enable
    //
    printf("\nTesting: RC_SetPulseTime() for each pin before enable: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        if (RC_SetPulseTime(i, SERVOCENTER) == ERROR) {
            DELAY(A_BIT);
        } else {
            testPassed = FALSE;
            printf("\nRC_SetPulseTime(0x%03x) returned SUCCESS "
                "before enable: FAILED TEST", i);
        }
    }
    if (testPassed) printf("PASSED");

    DELAY(A_BIT);
    FlushPrintBuffer();

    //
    // RC_GetPulseTime() should FAIL before enable
    //
    printf("\nTesting: RC_GetPulseTime() for each pin before enable: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        if (RC_GetPulseTime(i) == ERROR) {
            DELAY(A_BIT);
        } else {
            testPassed = FALSE;
            printf("\nRC_GetPulseTime(0x%03x) returned SUCCESS "
                "before enable: FAILED TEST", i);
        }
    }
    if (testPassed) printf("PASSED");
    FlushPrintBuffer();

    //
    // RC_ListPins() should return all zeros
    //
    printf("\nTesting: RC_ListPins() before enable: ");
    if (RC_ListPins() == 0x000) {
        printf("PASSED");
    } else {
        printf("FAILED");
        printf("\nRC_ListPins() = 0x%03x, FAILED TEST", RC_ListPins());
    }
    FlushPrintBuffer();

    //
    // RC_End() should FAIL before enable
    //
    printf("\nTesting: RC_End() before enable: ");
    if (RC_End() == ERROR) {
        printf("PASSED");
    } else {
        printf("FAILED TEST");
    }
    printf("\nRC Module tests before RC_Init() complete.\n");
    FlushPrintBuffer();

    /***************************************************************************
     *            ENABLE RC MODULE TEST INIT AND END                           *
     ***************************************************************************/
    //
    //Enable module, test error checking on all functions
    //
    DELAY(A_BIT);
    printf("\nTesting: Enable Module and re-test bounds checking");
    printf("\nEnabling RC module with RC_Init(): ");
    if (RC_Init() == SUCCESS) {
        printf("PASSED");
    } else {
        printf("FAILED TEST");
    }
    DELAY(A_BIT);
    printf("\nRe-enabling RC module with RC_Init(): ");
    if (RC_Init() == SUCCESS) {
        printf("FAILED TEST");
    } else {
        printf("PASSED");
    }
    DELAY(A_BIT);
    FlushPrintBuffer();

    /***************************************************************************
     *            TEST RC_ADDPIN()                                             *
     ***************************************************************************/
    printf("\n\nTesting RC_AddPins() functionality");
    printf("\nTesting: RC_AddPins(0x000) out of bounds: ");
    if (RC_AddPins(0x000) == SUCCESS) {
        printf("FAILED TEST");
    } else {
        printf("PASSED");
    }
    DELAY(A_BIT);
    printf("\nTesting: RC_AddPins(0xFFF), out of bounds: ");
    if (RC_AddPins(0xFFF) == SUCCESS) {
        printf("FAILED TEST");
    } else {
        printf("PASSED");
    }

    DELAY(A_BIT);
    printf("\nTesting: RC_AddPins() for each pin: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        if (RC_AddPins(i) == SUCCESS) {
            while (RC_ChangePending()) { // wait until change occured
                DELAY(A_BIT);
            }
        } else {
            testPassed = FALSE;
            printf("\nRC_AddPins(0x%03x) returned ERROR: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");
    DELAY(A_BIT);
    printf("\nTesting: RC_AddPins() for each pin already enabled: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        if (RC_AddPins(i) == ERROR) {
            DELAY(A_BIT);
        } else {
            testPassed = FALSE;
            printf("\nRC_AddPins(0x%03x) returned SUCCESS with pin "
                "already enabled: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");
    printf("\nTesting RC_ListPins() with all pins enabled: ");
    if (RC_ListPins() == ALLRCPINS) {
        printf("PASSED");
    } else {
        printf("FAILED");
        printf("\nRC_ListPins() = 0x%03x, FAILED TEST", RC_ListPins());
    }
    RC_RemovePins(RC_ListPins()); // get rid of all pins
    while (RC_ChangePending()) { // wait until change occured
        DELAY(A_BIT);
    }
    DELAY(A_BIT);
    printf("\nTesting: RC_AddPins() for two pins simultaneously: ");
    testPassed = TRUE;
    for (j = 1; j <= ALLRCPINS; j <<= 1) {
        for (i = 1; i <= ALLRCPINS; i <<= 1) {
            if (RC_AddPins(i | j) == SUCCESS) {
                while (RC_ChangePending()) { // wait until change occured
                    DELAY(A_BIT);
                }
            } else {
                testPassed = FALSE;
                printf("\nRC_AddPins(0x%03x) returned ERROR: TEST FAILED", i | j);
            }
            RC_RemovePins(i | j); // remove currently enabled pins
            while (RC_ChangePending()) { // wait until change occured
                DELAY(A_BIT);
            }
        }
    }
    if (testPassed) printf("PASSED");
    printf("\nRC_AddPins() Tests complete.");
    FlushPrintBuffer();

    /***************************************************************************
     *            TEST RC_REMOVEPIN()                                          *
     ***************************************************************************/
    RC_AddPins(ALLRCPINS); // enable all pins for remove pin test
    while (RC_ChangePending()) { // wait until change occured
        DELAY(A_BIT);
    }
    printf("\n\nTesting RC_RemovePins() functionality");
    printf("\nTesting: RC_RemovePins(0x000) out of bounds: ");
    if (RC_RemovePins(0x000) == SUCCESS) {
        printf("FAILED TEST");
    } else {
        printf("PASSED");
    }
    DELAY(A_BIT);
    printf("\nTesting: RC_RemovePins(0xFFF), out of bounds: ");
    if (RC_RemovePins(0xFFF) == SUCCESS) {
        printf("FAILED TEST");
    } else {
        printf("PASSED");
    }

    DELAY(A_BIT);
    printf("\nTesting: RC_RemovePins() for each pin: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        if (RC_RemovePins(i) == SUCCESS) {
            while (RC_ChangePending()) { // wait until change occured
                DELAY(A_BIT);
            }
        } else {
            testPassed = FALSE;
            printf("\nRC_RemovePins(0x%03x) returned ERROR: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");
    FlushPrintBuffer();

    DELAY(A_BIT);
    printf("\nTesting: RC_RemovePins() for each pin already disabled: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        if (RC_RemovePins(i) == ERROR) {
            DELAY(A_BIT);
        } else {
            testPassed = FALSE;
            printf("\nRC_RemovePins(0x%03x) returned SUCCESS with pin "
                "already disabled: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");
    printf("\nTesting: RC_ListPins() with all pins disabled: ");
    if (RC_ListPins() == 0x000) {
        printf("PASSED");
    } else {
        printf("FAILED");
        printf("\nRC_ListPins() = 0x%03x, FAILED TEST", RC_ListPins());
    }
    printf("\nRC_RemovePins() Tests complete.");
    FlushPrintBuffer();

    /***************************************************************************
     *            TEST RC_LISTPINS()                                            *
     ***************************************************************************/
    printf("\n\nTesting RC_ListPins() functionality");
    DELAY(A_BIT);
    printf("\nTesting: RC_ListPins() for each individual pin: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        RC_AddPins(i);
        while (RC_ChangePending()) { // wait until change occured
            DELAY(A_BIT);
        }
        if (RC_ListPins() != i) {
            testPassed = FALSE;
            printf("\nRC_ListPins() = 0x%03x does not "
                "match [0x%03x]: TEST FAILED", RC_ListPins(), i);
        }
        RC_RemovePins(i);
        while (RC_ChangePending()) { // wait until change occured
            DELAY(A_BIT);
        }
    }
    if (testPassed) printf("PASSED");
    // now do it with all pins active
    printf("\nTesting: RC_ListPins() for aggregate pins: ");
    testPassed = TRUE;
    RC_RemovePins(RC_ListPins()); // get rid of all pins
    while (RC_ChangePending()) { // wait until change occured
        DELAY(A_BIT);
    }
    for (i = 1; i <= ALLRCPINS; i = (i << 1) + 1) {
        RC_AddPins(i);
        while (RC_ChangePending()) { // wait until change occured
            DELAY(A_BIT);
        }
        if (RC_ListPins() != i) {
            testPassed = FALSE;
            printf("\nRC_ListPins() = 0x%03x does not "
                "match [0x%03x]: TEST FAILED", RC_ListPins(), i);
        }
        RC_RemovePins(i);
        while (RC_ChangePending()) { // wait until change occured
            DELAY(A_BIT);
        }
    }
    if (testPassed) printf("PASSED");
    // now test for all enabled but one
    printf("\nTesting: RC_ListPins() for all pins except one: ");
    testPassed = TRUE;
    RC_RemovePins(RC_ListPins()); // get rid of all pins
    while (RC_ChangePending()) { // wait until change occured
        DELAY(A_BIT);
    }
    RC_AddPins(ALLRCPINS);
    while (RC_ChangePending()) { // wait until change occured
        DELAY(A_BIT);
    }
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        if (RC_RemovePins(i) == SUCCESS) {
            while (RC_ChangePending()) { // wait until change occured
                DELAY(A_BIT);
            }
            if (RC_ListPins() != (ALLRCPINS & (~i))) {
                testPassed = FALSE;
                printf("\nRC_ListPins() = 0x%03x does not "
                    "match [0x%03x]: TEST FAILED", RC_ListPins(), (ALLRCPINS & (~i)));
            }
            RC_AddPins(i);
            while (RC_ChangePending()) { // wait until change occured
                DELAY(A_BIT);
            }
        }
    }
    if (testPassed) printf("PASSED");
    RC_End();
    DELAY(A_LOT);

    printf("\nRC_ListPins() Tests complete.");
    FlushPrintBuffer();

    /***************************************************************************
     *            TEST RC_SETPULSE()                                            *
     ***************************************************************************/
    RC_Init();
    printf("\n\nTesting RC_SetPulseTime() functionality");
    printf("\nALL pinse set to SERVOCENTER, all attached servos should center");
    RC_AddPins(ALLRCPINS);
    while (RC_ChangePending()) { // wait until change occured
        DELAY(A_BIT);
    }
    if (RC_ListPins() != ALLRCPINS) {
        printf("ERROR, pins not enabled");
    }
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        RC_SetPulseTime(i, SERVOCENTER);
    }
    printf("\n ");
    for (k = 0; k < NUM2SWEEP; k++) {
        DELAY(A_LOT);
        printf(". ");
    }

    printf("\nSweeping pulse on ALL pins from MINPULSE to MAXPULSE (%d times)", NUM2SWEEP);
    printf("\nAny servos attached to pins should move from FULL CCW to FULL CW and back");
    printf("\nOn cycle: ");
    for (k = 0; k < NUM2SWEEP; k++) {
        printf("%d ", k);
        for (pulse = MINPULSE; pulse < MAXPULSE; pulse += INC) {
            for (i = 1; i <= ALLRCPINS; i <<= 1) {
                RC_SetPulseTime(i, pulse);
            }
            DELAY(A_LOT);
        }
        for (pulse = MAXPULSE; pulse >= MINPULSE; pulse -= INC) {
            for (i = 1; i <= ALLRCPINS; i <<= 1) {
                RC_SetPulseTime(i, pulse);
            }
            DELAY(A_LOT);
        }
    }
    printf("\n\nSweeping pulse on EACH pin from MINPULSE to MAXPULSE (%d times)", NUM2SPIN);
    printf("\nAll servos attached should remain centered, each servo "
        "will turn from FULL CCW to FULL CW in turn");
    printf("\nOn Servo: ");
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        RC_SetPulseTime(i, SERVOCENTER);
    }
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        printf("[0x%03x] ", i);
        for (k = 0; k < NUM2SPIN; k++) {
            for (pulse = MINPULSE; pulse <= MAXPULSE; pulse += INC) {
                RC_SetPulseTime(i, pulse);
                DELAY(A_LOT);
            }
            for (pulse = MAXPULSE; pulse <= MINPULSE; pulse -= INC) {
                RC_SetPulseTime(i, pulse);
                DELAY(A_LOT);
            }
        }
        RC_SetPulseTime(i, SERVOCENTER);
    }
    printf("\nRC_SetPulseTime Tests complete");
    FlushPrintBuffer();


    /***************************************************************************
     *            TEST RC_GETPULSE()                                            *
     ***************************************************************************/

    printf("\n\nTesting RC_GetPulseTime() functionality");
    printf("\nTesting: RC_GetPulseTime() for each pin set to SERVOCENTER: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        RC_SetPulseTime(i, SERVOCENTER);
        DELAY(A_BIT);
        pulse = RC_GetPulseTime(i);
        if (pulse != SERVOCENTER) {
            printf("\nRC_GetPulseTime(0x%03X) FAILED: returned %d "
                "instead of %d", i, pulse, SERVOCENTER);
            testPassed = FALSE;
        }
    }
    if (testPassed) printf("PASSED");
    FlushPrintBuffer();

    printf("\nTesting: RC_GetPulseTime() for each pin set to MINPULSE: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        RC_SetPulseTime(i, MINPULSE);
        pulse = RC_GetPulseTime(i);
        if (pulse != MINPULSE) {
            printf("\nRC_GetPulseTime(0x%03X) FAILED: returned %d "
                "instead of %d", i, pulse, MINPULSE);
            testPassed = FALSE;
        }
    }
    if (testPassed) printf("PASSED");
    FlushPrintBuffer();

    printf("\nTesting: RC_GetPulseTime() for each pin set to MAXPULSE: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        RC_SetPulseTime(i, MAXPULSE);
        pulse = RC_GetPulseTime(i);
        if (pulse != MAXPULSE) {
            printf("\nRC_GetPulseTime(0x%03X) FAILED: returned %d "
                "instead of %d", i, pulse, MAXPULSE);
            testPassed = FALSE;
        }
    }
    if (testPassed) printf("PASSED");
    FlushPrintBuffer();

    printf("\nTesting: RC_GetPulseTime() for each pin variable value: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        for (pulse = MINPULSE; pulse < MAXPULSE; pulse += INC) {
            RC_SetPulseTime(i, pulse);
            returnPulse = RC_GetPulseTime(i);
            if (returnPulse != pulse) {
                printf("\nRC_GetPulseTime(0x%03X) FAILED: returned %d "
                    "instead of %d", i, returnPulse, pulse);
                testPassed = FALSE;
            }
        }
    }
    if (testPassed) printf("PASSED");
    printf("\nRC_GetPulseTime() Tests complete");
    FlushPrintBuffer();

    /***************************************************************************
     *            TEST RC_CHANGEPENDING()                                      *
     ***************************************************************************/
    printf("\n\nTesting RC_ChangePending() functionality");
    printf("\nTesting: RC_ChangePending() with no change actually pending: ");
    if (RC_ChangePending() == TRUE) printf("FAILED");
    else printf("PASSED");
    printf("\nTesting: RC_ChangePending() from pin deletion: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        INTEnable(INT_T4, INT_DISABLED);
        RC_RemovePins(i);
        if (RC_ChangePending() == FALSE) {
            printf("\nRC_ChangePending() FAILED on pin [0x%03X], returned "
                "FALSE when should be TRUE", i);
            testPassed = FALSE;
        } else {
            INTEnable(INT_T4, INT_ENABLED);
            ;
            while (RC_ChangePending()) {
                DELAY(A_BIT);
            }
        }
    }
    if (testPassed) printf("PASSED");

    printf("\nTesting: RC_ChangePending() from pin addition: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLRCPINS; i <<= 1) {
        INTEnable(INT_T4, INT_DISABLED);
        RC_AddPins(i);
        if (RC_ChangePending() == FALSE) {
            printf("\nRC_ChangePending() FAILED on pin [0x%03X], returned "
                "FALSE when should be TRUE", i);
            testPassed = FALSE;
        } else {
            INTEnable(INT_T4, INT_ENABLED);
            while (RC_ChangePending()) {
                DELAY(A_BIT);
            }
        }
    }
    if (testPassed) printf("PASSED");
    printf("\nRC_ChangePending() Testing complete.");
    FlushPrintBuffer();

    /***************************************************************************
     *            TEST RC_END()                                                *
     ***************************************************************************/
    printf("\nTesting: RC_End() while enabled: ");
    if (RC_End() == SUCCESS) {
        printf("PASSED");
        printf("\nAll enabled pins should be high impedance (inputs)");
    } else {
        printf("FAILED TEST");
    }
    printf("\nRC_End() Testing complete.");
    printf("\n\nRC_SERVO MODULE Testing complete.");
    FlushPrintBuffer();

    while (1) {
        ;
    }
}
#endif


