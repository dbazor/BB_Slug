/*
 * File:   pwm.c
 * Author: mdunne
 *
 * Created on November 12, 2011, 9:28 AM
 */

#include <xc.h>
#include <BOARD.h>

#include <peripheral/outcompare.h>
#include <peripheral/timer.h>
#include <pwm.h>
#include <stdio.h>






/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

#define F_PB (BOARD_GetPBClock())

//#define PWM_DEBUG_VERBOSE
#ifdef PWM_DEBUG_VERBOSE
#include "serial.h"
#define dbprintf(...) while(!IsTransmitEmpty()); printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif

#define ALLPWMPINS (PWM_PORTZ06|PWM_PORTY12|PWM_PORTY10|PWM_PORTY04|PWM_PORTX11)
#define NUM_PWM_CHANNELS 5



/*******************************************************************************
 * PRIVATE VARIABLES                                                            *
 ******************************************************************************/
static unsigned char PWMActive = FALSE;
//static unsigned int usedChannels;
static volatile unsigned int * const Duty_Registers[] = {&OC1RS, &OC2RS, &OC3RS, &OC4RS, &OC5RS};
static volatile unsigned int * const Reset_Registers[] = {&OC1R, &OC2R, &OC3R, &OC4R, &OC5R};
static volatile unsigned int * const Config_Registers[] = {&OC1CON, &OC2CON, &OC3CON, &OC4CON, &OC5CON};
static unsigned int PWMActivePins;
static unsigned int PWMFrequency;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function PWM_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief  Initializes the timer for the PWM system and set is to the default frequency
 * @note  None.
 * @author Max Dunne, 2011.11.12 */
char PWM_Init(void)
{
    if (PWMActive) {
        dbprintf("%s tried to init when already initialized", __FUNCTION__);
        return ERROR;
    }
    PWMActive = TRUE;
    PWM_SetFrequency(PWM_DEFAULT_FREQUENCY);
    PWMActivePins = 0;
    return SUCCESS;
}

/**
 * @Function PWM_SetFrequency(unsigned int NewFrequency)
 * @param NewFrequency - new frequency to set. best to use #defined from header
 * @return SUCCESS OR ERROR
 * @brief  Changes the frequency of the PWM system.
 * @note  Behavior of PWM channels during Frequency change is undocumented
 * @author Max Dunne, 2013.08.19 */
char PWM_SetFrequency(unsigned int NewFrequency)
{
    if (!PWMActive) {
        dbprintf("%s called before enable\r\n", __FUNCTION__);
        return ERROR;
    }
    if ((NewFrequency < MIN_PWM_FREQ) | (MAX_PWM_FREQ < NewFrequency)) {
        dbprintf("%s called with frequency outside bounds: %d", __FUNCTION__, NewFrequency);
        return ERROR;
    }
    if (NewFrequency != 0) {
        //turn off the timer in preperation to restarting it with the new fequency
        T2CON = 0;
    }
    if (NewFrequency <= 1000) {
        OpenTimer2(T2_ON | T2_PS_1_32, F_PB / 32 / NewFrequency);
        dbprintf("Period less than 1KHz, setting prescaler to 32\r\n");
    } else {
        OpenTimer2(T2_ON | T2_PS_1_1, F_PB / NewFrequency);
        dbprintf("Period greater than 1KHz, setting prescaler to 1\r\n");
    }
    PWMFrequency=NewFrequency;
    return SUCCESS;
}

/**
 * @Function PWM_GetFrequency(void)
 * @return Frequency of system in Hertz
 * @brief  gets the frequency of the PWM system.
 * @author Max Dunne, 2013.08.19 */
unsigned int PWM_GetFrequency(void)
{
    if (!PWMActive) {
        dbprintf("%s called before enable\r\n", __FUNCTION__);
        return ERROR;
    }
    return (PWMFrequency);
}

/**
 * @Function PWM_AddPins(unsigned short int AddPins)
 * @param AddPins - use #defined PWM_PORTxxx OR'd together for each A/D Pin you wish to add
 * @return SUCCESS OR ERROR
 * @brief  Adds new pins to the PWM system.  If any pin is already active it errors 
 * out
 * @author Max Dunne, 2013.08.15 */
char PWM_AddPins(unsigned short int AddPins)
{
    if (!PWMActive) {
        dbprintf("%s returning ERROR before enable\r\n", __FUNCTION__);
        return ERROR;
    }
    if ((AddPins == 0) || (AddPins > ALLPWMPINS)) {
        dbprintf("%s returning ERROR with pins outside range: %X\r\n", __FUNCTION__, AddPins);
        return ERROR;
    }

    if (PWMActivePins & AddPins) {
        dbprintf("%s Returning ERROR for pins already in state: %X \r\n", __FUNCTION__, AddPins);
        return ERROR;
    }
    int PinCount = 0;
    //sets new pwm pins in the on state while setting their duty cycles to zero
    for (PinCount = 0; PinCount < ALLPWMPINS; PinCount++) {
        if (AddPins & (1 << PinCount)) {
            *Duty_Registers[PinCount] = 0;
            *Reset_Registers[PinCount] = 0;
            *Config_Registers[PinCount] = (OC_ON | OC_TIMER2_SRC | OC_PWM_FAULT_PIN_DISABLE);
            dbprintf("PWM pin #%d has been added to the system\r\n", PinCount);
        }
    }

    PWMActivePins |= AddPins;
    return SUCCESS;
}

/**
 * @Function PWM_RemovePins(unsigned int PWMPins)
 * @param RemovePins - use #defined AD_PORTxxx OR'd together for each A/D Pin you wish to remove
 * @return SUCCESS OR ERROR
 * @brief  Remove pins from the PWM system.  If any pin is not active it errors out
 * @author Max Dunne, 2013.08.15 */
char PWM_RemovePins(unsigned int PWMPins)
{
    if (!PWMActive) {
        dbprintf("%s returning ERROR before enable\r\n", __FUNCTION__);
        return ERROR;
    }
    if ((PWMPins == 0) || (PWMPins > ALLPWMPINS)) {
        dbprintf("%s returning ERROR with pins outside range: %X\r\n", __FUNCTION__, PWMPins);
        return ERROR;
    }
    if (!(PWMActivePins & PWMPins)) {
        dbprintf("%s Returning ERROR for pins already in state: %X \r\n", __FUNCTION__, PWMPins);
        return ERROR;
    }
    int PinCount = 0;
    for (PinCount = 0; PinCount < ALLPWMPINS; PinCount++) {
        if (PWMPins & (1 << PinCount)) {
            *Duty_Registers[PinCount] = 0;
            *Reset_Registers[PinCount] = 0;
            *Config_Registers[PinCount] &= (~_OC1CON_ON_MASK);
        }
    }
    PWMActivePins &= (~PWMPins);
    return SUCCESS;
}

/**
 * @Function PWM_ListPins(void)
 * @param None
 * @return Listing of all PWM pins that are active.
 * @brief  Returns a variable of all active pwm pins. An individual pin can be determined 
 * if active by "anding" with the pwm_PORTXX Macros
 * @author Max Dunne, 2013.08.19 */
unsigned int PWM_ListPins(void)
{
    return PWMActivePins;
}

/**
 * Function  PWM_SetDutyCycle
 * @param Channels, use #defined PWM_PORTxxx
 * @param Duty, duty cycle for the channel (0-1000)
 * @return SUCCESS or ERROR
 * @remark Sets the Duty Cycle for a Single Channel and returns error if that channel is not enabled
 * @author Max Dunne
 * @date 2011.11.12  */
char PWM_SetDutyCycle(unsigned char Channel, unsigned int Duty)
{
    if (!PWMActive) {
        dbprintf("%s returning ERROR before enable\r\n", __FUNCTION__);
        return ERROR;
    }
    if ((Channel == 0 || Channel > ALLPWMPINS)) {
        dbprintf("%s returning error with pin out of bounds: %X\r\n", __FUNCTION__, Channel);
        return ERROR;
    }
    if (!(Channel & PWMActivePins)) {
        dbprintf("%s returning error with unactivated pin: %X %X\r\n", __FUNCTION__, Channel, PWMActivePins);
        return ERROR;
    }
    if (Duty < 0 || Duty > 1000) {
        dbprintf("%s returning error with duty cycle out of bounds: %d\r\n", __FUNCTION__, Duty);
        return ERROR;
    }

    unsigned int ScaledDuty = 0;
    unsigned int TranslatedChannel = 0;
    ScaledDuty = ((PR2 + 1) * Duty) / MAX_PWM;
    while (Channel > 1) {
        Channel >>= 1;
        TranslatedChannel++;
    }
    dbprintf("Translated Channel is %d and Scaled Duty is %d\r\n", TranslatedChannel, ScaledDuty);
    *Duty_Registers[TranslatedChannel] = ScaledDuty;
    return SUCCESS;

}

/**
 * Function  PWM_GetDutyCycle
 * @param Channels, use #defined PWM_PORTxxx
 *
 * @return Duty cycle
 * @remark Gets the Duty Cycle for a Single Channel and returns error if that channel is not enabled
 * @author Max Dunne
 * @date 2011.11.12  */
unsigned int PWM_GetDutyCycle(char Channel)
{
    if (!PWMActive) {
        dbprintf("%s returning ERROR before enable\r\n", __FUNCTION__);
        return ERROR;
    }
    if ((Channel == 0 || Channel > ALLPWMPINS)) {
        dbprintf("%s returning error with pin out of bounds: %X\r\n", __FUNCTION__, Channel);
        return ERROR;
    }
    if (!(Channel & PWMActivePins)) {
        dbprintf("%s returning error with unactivated pin: %X %X\r\n", __FUNCTION__, Channel, PWMActivePins);
        return ERROR;
    }

    unsigned int ScaledDuty = 0;
    unsigned int Duty = 0;
    unsigned int TranslatedChannel = 0;

    while (Channel > 1) {
        Channel >>= 1;
        TranslatedChannel++;
    }
    ScaledDuty = *Duty_Registers[TranslatedChannel];
    Duty = (ScaledDuty * MAX_PWM) / (PR2 + 1) + 1;
    if (Duty > MAX_PWM) {
        //one off error due to integer division
        Duty = MAX_PWM;
    }
    dbprintf("Translated Channel is %d and unScaled Duty is %d\r\n", TranslatedChannel, Duty);

    return Duty;

}

/**
 * Function: PWM_End
 * @param None
 * @return SUCCESS or ERROR
 * @remark Disables the PWM sub-system and releases all pins.
 * @author Max Dunne
 * @date 2011.11.12  */
char PWM_End(void)
{
    int Curpin;
    if (!PWMActive) {
        return ERROR;
    }
    for (Curpin = 0; Curpin < NUM_PWM_CHANNELS; Curpin++) {
        *Duty_Registers[Curpin] = 0;
        *Reset_Registers[Curpin] = 0;
    }
    INTEnable(INT_T2, INT_DISABLED);
    INTEnable(INT_OC1, INT_DISABLED);
    INTEnable(INT_OC2, INT_DISABLED);
    INTEnable(INT_OC3, INT_DISABLED);
    INTEnable(INT_OC4, INT_DISABLED);
    INTEnable(INT_OC5, INT_DISABLED);
    OC1CONCLR = _OC1CON_ON_MASK;
    OC2CONCLR = _OC2CON_ON_MASK;
    OC3CONCLR = _OC3CON_ON_MASK;
    OC4CONCLR = _OC4CON_ON_MASK;
    OC5CONCLR = _OC5CON_ON_MASK;

    PWMActive = FALSE;
    PWMFrequency = 0;
    return SUCCESS;
}


/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/
#ifdef PWM_TEST

#include <xc.h>
#include <serial.h>
#include "PWM.h"
#include <stdio.h>


#define DELAY(x)    for (wait = 0; wait <= x; wait++) {asm("nop");}
#define A_BIT       40000
#define A_LOT       80000
#define NUM2SWEEP   10
#define NUM2SPIN    2
#define INC         100
#define PWM_PORT     PWM_PORTX04

#define FIFTY_PERCENT_DUTY 500
//#define GetArray(Name)  Name ## Goober
//#define GetArrayWrapper()
//#define Str(x)   #Str
//GetArray(__FUNCTION__)[0]=3;

int main(void)
{
    unsigned int wait = 0;
    unsigned short int duty;
    unsigned short int i, j, k;
    char testPassed = FALSE;
//    int8_t wantedResult = SUCCESS;
    DELAY(A_BIT);

    BOARD_Init();
    printf("\nCMPE118 PWM module test harness.");
    printf("\nInitial Test, ensure all functions return ERROR "
            "before PWM_Init() is run\n");

    /***************************************************************************
     *            TEST PWM MODULE ERROR CHECKING BEFORE ENABLE                  *
     ***************************************************************************/
    //
    // PWM_AddPins() should FAIL before enable
    //
    printf("\nTesting: PWM_AddPins() for each pin before enable: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        if (PWM_AddPins(i) == ERROR) {

        } else {
            testPassed = FALSE;
            printf("\nPWM_AddPins(0x%03x) returned SUCCESS: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");

    //
    // PWM_RemovePins() should FAIL before enable
    //
    printf("\nTesting: PWM_RemovePins() for each pin before enable: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        if (PWM_RemovePins(i) == ERROR) {
        } else {
            testPassed = FALSE;
            printf("\nPWM_RemovePins(0x%03x) returned SUCCESS: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");

    //
    // PWM_SetDutyCycle() should FAIL before enable
    //
    printf("\nTesting: PWM_SetDutyCycle() for each pin before enable: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        if (PWM_SetDutyCycle(i, FIFTY_PERCENT_DUTY) == ERROR) {
            DELAY(A_BIT);
        } else {
            testPassed = FALSE;
            printf("\nPWM_SetDutyCycle(0x%03x) returned SUCCESS "
                    "before enable: FAILED TEST", i);
        }
    }
    if (testPassed) printf("PASSED");

    //
    // PWM_GetPulseTime() should FAIL before enable
    //
    printf("\nTesting: PWM_GetPulseTime() for each pin before enable: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        if (PWM_GetDutyCycle(i) == ERROR) {
            DELAY(A_BIT);
        } else {
            testPassed = FALSE;
            printf("\nPWM_GetPulseTime(0x%03x) returned SUCCESS "
                    "before enable: FAILED TEST", i);
        }
    }
    if (testPassed) printf("PASSED");

    //
    // PWM_ListPins() should return all zeros
    //
    printf("\nTesting: PWM_ListPins() before enable: ");
    if (PWM_ListPins() == 0x000) {
        printf("PASSED");
    } else {
        printf("FAILED");
        printf("\nPWM_ListPins() = 0x%03x, FAILED TEST", PWM_ListPins());
    }

    //
    // PWM_End() should FAIL before enable
    //
    printf("\nTesting: PWM_End() before enable: ");
    if (PWM_End() == ERROR) {
        printf("PASSED");
    } else {
        printf("FAILED TEST");
    }
    printf("\nPWM Module tests before PWM_Init() complete.\n");

    /***************************************************************************
     *            ENABLE PWM MODULE TEST INIT AND END                           *
     ***************************************************************************/
    //
    //Enable module, test error checking on all functions
    //
    DELAY(A_BIT);
    printf("\nTesting: Enable Module and re-test bounds checking");
    printf("\nEnabling PWM module with PWM_Init(): ");
    if (PWM_Init() == SUCCESS) {
        printf("PASSED");
    } else {
        printf("FAILED TEST");
    }
    DELAY(A_BIT);
    printf("\nRe-enabling PWM module with PWM_Init(): ");
    if (PWM_Init() == SUCCESS) {
        printf("FAILED TEST");
    } else {
        printf("PASSED");
    }
    DELAY(A_BIT);
    printf("PWM Frequency: %d\r\n\r\n",PWM_GetFrequency());
    /***************************************************************************
     *            TEST PWM_ADDPIN()                                             *
     ***************************************************************************/
    printf("\n\nTesting PWM_AddPins() functionality");
    printf("\nTesting: PWM_AddPins(0x000) out of bounds: ");
    if (PWM_AddPins(0x000) == SUCCESS) {
        printf("FAILED TEST");
    } else {
        printf("PASSED");
    }
    DELAY(A_BIT);
    printf("\nTesting: PWM_AddPins(0xFFF), out of bounds: ");
    if (PWM_AddPins(0xFFF) == SUCCESS) {
        printf("FAILED TEST");
    } else {
        printf("PASSED");
    }

    DELAY(A_BIT);
    printf("\nTesting: PWM_AddPins() for each pin: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        if (PWM_AddPins(i) == SUCCESS) {

        } else {
            testPassed = FALSE;
            printf("\nPWM_AddPins(0x%03x) returned ERROR: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");
    DELAY(A_BIT);
    printf("\nTesting: PWM_AddPins() for each pin already enabled: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        if (PWM_AddPins(i) == ERROR) {
            DELAY(A_BIT);
        } else {
            testPassed = FALSE;
            printf("\nPWM_AddPins(0x%03x) returned SUCCESS with pin "
                    "already enabled: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");
    printf("\nTesting PWM_ListPins() with all pins enabled: ");
    if (PWM_ListPins() == ALLPWMPINS) {
        printf("PASSED");
    } else {
        printf("FAILED");
        printf("\nPWM_ListPins() = 0x%03x, FAILED TEST", PWM_ListPins());
    }
    PWM_RemovePins(PWM_ListPins()); // get rid of all pins

    DELAY(A_BIT);
    printf("\nTesting: PWM_AddPins() for two pins simultaneously: ");
    testPassed = TRUE;
    for (j = 1; j <= ALLPWMPINS; j <<= 1) {
        for (i = 1; i <= ALLPWMPINS; i <<= 1) {
            if (PWM_AddPins(i | j) == SUCCESS) {
            } else {
                testPassed = FALSE;
                printf("\nPWM_AddPins(0x%03x) returned ERROR: TEST FAILED", i | j);
            }
            PWM_RemovePins(i | j); // remove currently enabled pins

        }
    }
    if (testPassed) printf("PASSED");
    printf("\nPWM_AddPins() Tests complete.");


    /***************************************************************************
     *            TEST PWM_REMOVEPIN()                                          *
     ***************************************************************************/
    PWM_AddPins(ALLPWMPINS); // enable all pins for remove pin test

    printf("\n\nTesting PWM_RemovePins() functionality");
    printf("\nTesting: PWM_RemovePins(0x000) out of bounds: ");
    if (PWM_RemovePins(0x000) == SUCCESS) {
        printf("FAILED TEST");
    } else {
        printf("PASSED");
    }
    DELAY(A_BIT);
    printf("\nTesting: PWM_RemovePins(0xFFF), out of bounds: ");
    if (PWM_RemovePins(0xFFF) == SUCCESS) {
        printf("FAILED TEST");
    } else {
        printf("PASSED");
    }

    DELAY(A_BIT);
    printf("\nTesting: PWM_RemovePins() for each pin: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        if (PWM_RemovePins(i) == SUCCESS) {

        } else {
            testPassed = FALSE;
            printf("\nPWM_RemovePins(0x%03x) returned ERROR: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");

    DELAY(A_BIT);
    printf("\nTesting: PWM_RemovePins() for each pin already disabled: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        if (PWM_RemovePins(i) == ERROR) {
            DELAY(A_BIT);
        } else {
            testPassed = FALSE;
            printf("\nPWM_RemovePins(0x%03x) returned SUCCESS with pin "
                    "already disabled: TEST FAILED", i);
        }
    }
    if (testPassed) printf("PASSED");
    printf("\nTesting: PWM_ListPins() with all pins disabled: ");
    if (PWM_ListPins() == 0x000) {
        printf("PASSED");
    } else {
        printf("FAILED");
        printf("\nPWM_ListPins() = 0x%03x, FAILED TEST", PWM_ListPins());
    }
    printf("\nPWM_RemovePins() Tests complete.");

    /***************************************************************************
     *            TEST PWM_LISTPINS()                                            *
     ***************************************************************************/
    printf("\n\nTesting PWM_ListPins() functionality");
    DELAY(A_BIT);
    printf("\nTesting: PWM_ListPins() for each individual pin: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        PWM_AddPins(i);

        if (PWM_ListPins() != i) {
            testPassed = FALSE;
            printf("\nPWM_ListPins() = 0x%03x does not "
                    "match [0x%03x]: TEST FAILED", PWM_ListPins(), i);
        }
        PWM_RemovePins(i);

    }
    if (testPassed) printf("PASSED");
    // now do it with all pins active
    printf("\nTesting: PWM_ListPins() for aggregate pins: ");
    testPassed = TRUE;
    PWM_RemovePins(PWM_ListPins()); // get rid of all pins

    for (i = 1; i <= ALLPWMPINS; i = (i << 1) + 1) {
        PWM_AddPins(i);

        if (PWM_ListPins() != i) {
            testPassed = FALSE;
            printf("\nPWM_ListPins() = 0x%03x does not "
                    "match [0x%03x]: TEST FAILED", PWM_ListPins(), i);
        }
        PWM_RemovePins(i);

    }
    if (testPassed) printf("PASSED");
    // now test for all enabled but one
    printf("\nTesting: PWM_ListPins() for all pins except one: ");
    testPassed = TRUE;
    PWM_RemovePins(PWM_ListPins()); // get rid of all pins

    PWM_AddPins(ALLPWMPINS);

    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        if (PWM_RemovePins(i) == SUCCESS) {

            if (PWM_ListPins() != (ALLPWMPINS & (~i))) {
                testPassed = FALSE;
                printf("\nPWM_ListPins() = 0x%03x does not "
                        "match [0x%03x]: TEST FAILED", PWM_ListPins(), (ALLPWMPINS & (~i)));
            }
            PWM_AddPins(i);

        }
    }
    if (testPassed) printf("PASSED");
    PWM_End();
    DELAY(A_LOT);

    printf("\nPWM_ListPins() Tests complete.");

    /***************************************************************************
     *            TEST PWM_SETPULSE()                                            *
     ***************************************************************************/
    PWM_Init();
    printf("\n\nTesting PWM_SetDutyCycle() functionality");
    printf("\nALL pinse set to SERVOCENTER, all attached servos should center");
    PWM_AddPins(ALLPWMPINS);

    if (PWM_ListPins() != ALLPWMPINS) {
        printf("ERROR, pins not enabled");
    }
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        PWM_SetDutyCycle(i, FIFTY_PERCENT_DUTY);
    }
    printf("\n ");
    for (k = 0; k < NUM2SWEEP; k++) {
        DELAY(A_LOT);
        printf(". ");
    }

    printf("\nSweeping pulse on ALL pins from MIN_PWM to MAX_PWM (%d times)", NUM2SWEEP);
    printf("\nAny servos attached to pins should move from FULL CCW to FULL CW and back");
    printf("\nOn cycle: ");
    for (k = 0; k < NUM2SWEEP; k++) {
        printf("%d ", k);
        for (duty = MIN_PWM; duty < MAX_PWM; duty += INC) {
            for (i = 1; i <= ALLPWMPINS; i <<= 1) {
                PWM_SetDutyCycle(i, duty);
            }
            DELAY(A_LOT);
        }
        for (duty = MIN_PWM; duty >= MAX_PWM; duty -= INC) {
            for (i = 1; i <= ALLPWMPINS; i <<= 1) {
                PWM_SetDutyCycle(i, duty);
            }
            DELAY(A_LOT);
        }
    }
    printf("\n\nSweeping pulse on EACH pin from MIN_PWM to MAX_PWM (%d times)", NUM2SPIN);
    printf("\nAll servos attached should remain centered, each servo "
            "will turn from FULL CCW to FULL CW in turn");
    printf("\nOn Servo: ");
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        PWM_SetDutyCycle(i, FIFTY_PERCENT_DUTY);
    }
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        printf("[0x%03x] ", i);
        for (k = 0; k < NUM2SPIN; k++) {
            for (duty = MIN_PWM; duty <= MAX_PWM; duty += INC) {
                PWM_SetDutyCycle(i, duty);
                DELAY(A_LOT);
            }
            for (duty = MAX_PWM; duty <= MIN_PWM; duty -= INC) {
                PWM_SetDutyCycle(i, duty);
                DELAY(A_LOT);
            }
        }
        PWM_SetDutyCycle(i, FIFTY_PERCENT_DUTY);
    }
    printf("\nPWM_SetDutyCycle Tests complete");


    /***************************************************************************
     *            TEST PWM_GETPULSE()                                            *
     ***************************************************************************/
    printf("\n\nTesting PWM_GetPulseTime() functionality");
    printf("\nTesting: PWM_GetPulseTime() for each pin set to SERVOCENTER: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        PWM_SetDutyCycle(i, FIFTY_PERCENT_DUTY);
        if (PWM_GetDutyCycle(i) != FIFTY_PERCENT_DUTY) {
            printf("\nPWM_GetPulseTime(0x%03X) FAILED: returned %d "
                    "instead of %d", i,PWM_GetDutyCycle(i),FIFTY_PERCENT_DUTY);
            while (!IsTransmitEmpty());
            testPassed = FALSE;
        }
    }
    if (testPassed) printf("PASSED");

    printf("\nTesting: PWM_GetPulseTime() for each pin set to MIN_PWM: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        PWM_SetDutyCycle(i, MIN_PWM);
        if (PWM_GetDutyCycle(i) != MIN_PWM) {
            printf("\nPWM_GetPulseTime(0x%03X) FAILED: returned %d "
                    "instead of %d", i,PWM_GetDutyCycle(i), MIN_PWM);
            testPassed = FALSE;
        }
    }
    if (testPassed) printf("PASSED");
    while (!IsTransmitEmpty());
    printf("\nTesting: PWM_GetPulseTime() for each pin set to MAX_PWM: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        PWM_SetDutyCycle(i, MAX_PWM);
        if (PWM_GetDutyCycle(i) != MAX_PWM) {
            printf("\nPWM_GetPulseTime(0x%03X) FAILED: returned %d "
                    "instead of %d", i,PWM_GetDutyCycle(i), MAX_PWM);

            testPassed = FALSE;
        }
    }
    if (testPassed) printf("PASSED");
    while (!IsTransmitEmpty());
    printf("\nTesting: PWM_GetDutyCycle() for each pin variable value: ");
    testPassed = TRUE;
    for (i = 1; i <= ALLPWMPINS; i <<= 1) {
        for (duty = MIN_PWM; duty < MAX_PWM; duty += INC) {
            PWM_SetDutyCycle(i, duty);
            if (PWM_GetDutyCycle(i) != duty) {
                printf("\nPWM_GetPulseTime(0x%03X) FAILED: returned %d "
                        "instead of %d", i, PWM_GetDutyCycle(i), duty);
                testPassed = FALSE;
            }
        }
    }
    if (testPassed) printf("PASSED");
    printf("\nPWM_GetPulseTime() Tests complete");



    /***************************************************************************
     *            TEST PWM_END()                                                *
     ***************************************************************************/
    printf("\nTesting: PWM_End() while enabled: ");
    if (PWM_End() == SUCCESS) {
        printf("PASSED");
        printf("\nAll enabled pins should be high impedance (inputs)");
    } else {
        printf("FAILED TEST");
    }
    printf("\nPWM_End() Testing complete.");
    printf("\n\nPWM MODULE Testing complete.");

    while (1) {
        ;
    }
}
#endif

// #ifdef PWM_TEST

// #include <serial.h>
// #include <BOARD.h>

// #define MIN_RAMP 0
// #define MAX_RAMP 1000
// #define FIRST_PERIOD PWM_20KHZ
// #define SECOND_PERIOD PWM_500HZ
// #define DELAY() for(i=0;i < NOPCOUNT; i++) __asm("nop")
// #define NOPCOUNT 520000

// int main(void) {
// unsigned int i, j;
// BOARD_Init();
// printf("Uno PWM Test Harness\r\n");
// PWM_Init();
// PWM_AddPins(PWM_PORTX11 | PWM_PORTY04 | PWM_PORTY10 | PWM_PORTY12 | PWM_PORTZ06);
// PWM_SetFrequency(FIRST_PERIOD);


// printf("Ramping PWM from %d-%d%% in 10%% steps at %d with a delay between\r\n", MIN_RAMP, MAX_RAMP, FIRST_PERIOD);
// unsigned char ch = 0;
// for (j = MIN_RAMP; j <= MAX_RAMP; j += 100) {
// PWM_SetDutyCycle(PWM_PORTX11, j);
// PWM_SetDutyCycle(PWM_PORTY04, j + 20);
// PWM_SetDutyCycle(PWM_PORTY10, j + 40);
// PWM_SetDutyCycle(PWM_PORTY12, j + 60);
// PWM_SetDutyCycle(PWM_PORTZ06, j + 80);
// printf("Outputting %d%% Duty Cycle\r\n", j / 10);
// DELAY();
// }
// printf("Setting Period to %d and repeating ramp\r\n", SECOND_PERIOD);
// PWM_End();
// PWM_Init();
// PWM_AddPins(PWM_PORTX11 | PWM_PORTY04 | PWM_PORTY10 | PWM_PORTY12 | PWM_PORTZ06);
// PWM_SetFrequency(FIRST_PERIOD);
// for (j = MIN_RAMP; j <= MAX_RAMP; j += 100) {
// PWM_SetDutyCycle(PWM_PORTX11, j);
// PWM_SetDutyCycle(PWM_PORTY04, j + 20);
// PWM_SetDutyCycle(PWM_PORTY10, j + 40);
// PWM_SetDutyCycle(PWM_PORTY12, j + 60);
// PWM_SetDutyCycle(PWM_PORTZ06, j + 80);
// printf("Outputting %d%% Duty Cycle\r\n", j / 10);
// DELAY();
// }
// PWM_End();
// return 0;
// while (1);
// }

// #endif