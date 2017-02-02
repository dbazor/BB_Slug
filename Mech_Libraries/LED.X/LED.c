/*
 * File:   LED.c
 * Author: Elkaim
 *
 * Created on December 18, 2011, 7:58 PM
 */
#include "BOARD.h"
#include <xc.h>
#include <plib.h>
#include <serial.h>
#include <LED.h>



/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
//#define LED_TEST
//#define DEBUG_VERBOSE
#ifdef DEBUG_VERBOSE
#include <stdio.h>
#define dbprintf(...) printf(__VA_ARGS__)
#else
#define dbprintf(...)
#endif

#define NUMLEDBANKS 3
#define NUMLEDSPERBANK 4
#define BANK2OFFSET 4
#define BANK3OFFSET 8
#define FULLBANK 0x0F
#define ALL_LED_BANKS (LED_BANK1|LED_BANK2|LED_BANK3)

#define LED_SetPinOutput(i) *LED_TRISCLR[i] = LED_bitsMap[i]
#define LED_SetPinInput(i) *LED_TRISSET[i] = LED_bitsMap[i];
#define LED_On(i) *LED_LATCLR[i] = LED_bitsMap[i];
#define LED_Off(i) *LED_LATSET[i] = LED_bitsMap[i];
#define LED_Toggle(i) *LED_LATINV[i] = LED_bitsMap[i];
#define LED_Get(i) (*LED_LAT[i]&LED_bitsMap[i])

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/

typedef union {

    struct {
        unsigned bit0 : 1;
        unsigned bit1 : 1;
        unsigned bit2 : 1;
        unsigned bit3 : 1;
        unsigned : 4;
    };
    unsigned char c;
} LEDBitBank_t;

static uint8_t LEDActive;
static uint8_t ActiveLEDBanks;

static volatile unsigned int * const LED_TRISCLR[] = {&TRISDCLR, &TRISDCLR, &TRISDCLR, &TRISDCLR,
    &TRISGCLR, &TRISDCLR, &TRISGCLR, &TRISFCLR,
    &TRISGCLR, &TRISFCLR, &TRISFCLR, &TRISBCLR};

static volatile unsigned int * const LED_TRISSET[] = {&TRISDSET, &TRISDSET, &TRISDSET, &TRISDSET,
    &TRISGSET, &TRISDSET, &TRISGSET, &TRISFSET,
    &TRISGSET, &TRISFSET, &TRISFSET, &TRISBSET};

static volatile unsigned int * const LED_LATCLR[] = {&LATDCLR, &LATDCLR, &LATDCLR, &LATDCLR,
    &LATGCLR, &LATDCLR, &LATGCLR, &LATFCLR,
    &LATGCLR, &LATFCLR, &LATFCLR, &LATBCLR};

static volatile unsigned int * const LED_LATSET[] = {&LATDSET, &LATDSET, &LATDSET, &LATDSET,
    &LATGSET, &LATDSET, &LATGSET, &LATFSET,
    &LATGSET, &LATFSET, &LATFSET, &LATBSET};

static volatile unsigned int * const LED_LATINV[] = {&LATDINV, &LATDINV, &LATDINV, &LATDINV,
    &LATGINV, &LATDINV, &LATGINV, &LATFINV,
    &LATGINV, &LATFINV, &LATFINV, &LATBINV};

static volatile unsigned int * const LED_LAT[] = {&LATD, &LATD, &LATD, &LATD,
    &LATG, &LATD, &LATG, &LATF,
    &LATG, &LATF, &LATF, &LATB};

#ifdef JP_SPI_MASTER
static unsigned short int LED_bitsMap[] = {BIT_5, BIT_3, BIT_11, BIT_6,
    BIT_8, BIT_7, BIT_7, BIT_6,
    BIT_6, BIT_4, BIT_5, BIT_0};
#else
static unsigned short int LED_bitsMap[] = {BIT_5, BIT_3, BIT_11, BIT_6,
    BIT_7, BIT_8, BIT_7, BIT_6,
    BIT_6, BIT_4, BIT_5, BIT_0};
#endif

/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                *
 ******************************************************************************/


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function LED_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief  Initializes the LED subsystem, sets each pin in the bank as a digital output 
 * and turns the LED on.
 * @author Gabriel Hugh Elkaim, 2011.12.25 01:16 */
char LED_Init(void)
{
    char i;
    dbprintf("\nInitializing LED subsystem");
    if ((LEDActive)) {
        dbprintf("\nError: LED subsystem initialization failed\r\n");
        return ERROR;
    }
    LEDActive = TRUE;
    ActiveLEDBanks = 0;
    dbprintf("\nLED Subsystem Enabled");
    return SUCCESS;
}

/**
 * @Function LED_AddBanks(unsigned char AddBanks)
 * @param banks - an unsigned char with a 1 in each position to set the bank as an LED bank, should be a bitwise OR of the #define'd LED_BANKx values.
 * @return SUCCESS or ERROR
 * @brief  adds selected banks to the system and sets them to be digital outputs
 * @note  J3 SPI Jumpers should be set in MASTER position.
 * @author Gabriel Hugh Elkaim, 2011.12.25 01:16 */
char LED_AddBanks(unsigned char AddBanks)
{
    char i;
    dbprintf("\nInitializing LED subsystem");

    if (!LEDActive) {
        dbprintf("\nError: LED subsystem not initialized\r\n");
        return ERROR;
    }

    if ((AddBanks == 0) || (AddBanks > ALL_LED_BANKS)) {
        dbprintf("%s returning ERROR with Banks outside range: %X\r\n", __FUNCTION__, AddBanks);
        return ERROR;
    }
    if (ActiveLEDBanks & AddBanks) {
        dbprintf("%s Returning ERROR for pins already in state: %X \r\n", __FUNCTION__, AddBanks);
        return ERROR;
    }
    ActiveLEDBanks |= AddBanks;

    if (AddBanks & LED_BANK1) {
        dbprintf("\nLED Bank1 enabled");
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            LED_SetPinOutput(i);
            LED_On(i);
        }
    }
    if (AddBanks & LED_BANK2) {
        dbprintf("\nLED Bank2 enabled");
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            LED_SetPinOutput(i + BANK2OFFSET);
            LED_On(i + BANK2OFFSET);
        }
    }
    if (AddBanks & LED_BANK3) {
        dbprintf("\nLED Bank3 enabled");
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            LED_SetPinOutput(i + BANK3OFFSET);
            LED_On(i + BANK3OFFSET);
        }
    }
    return SUCCESS;
}

/**
 * @Function LED_RemoveBanks(unsigned char RemoveBanks)
 * @param banks - an unsigned char with a 1 in each position to set the bank as an LED
 * @param bank - should be a bitwise OR of the #define'd LED_BANKx values.
 * @return SUCCESS or ERROR
 * @brief  adds selected banks to the system and sets them to be digital outputs
 * @note  J3 SPI Jumpers should be set in MASTER position.
 * @author Gabriel Hugh Elkaim, 2011.12.25 01:16 */
char LED_RemoveBanks(unsigned char RemoveBanks)
{
    char i;
    dbprintf("\nInitializing LED subsystem");

    if (!LEDActive) {
        dbprintf("\nError: LED subsystem not initialized\r\n");
        return ERROR;
    }

    if ((RemoveBanks == 0) || (RemoveBanks > ALL_LED_BANKS)) {
        dbprintf("%s returning ERROR with Banks outside range: %X\r\n", __FUNCTION__, RemoveBanks);
        return ERROR;
    }
    if (!(ActiveLEDBanks & RemoveBanks)) {
        dbprintf("%s Returning ERROR for pins already in state: %X \r\n", __FUNCTION__, RemoveBanks);
        return ERROR;
    }
    ActiveLEDBanks &= (~RemoveBanks);
    if (LED_BANK1 & RemoveBanks) { // shut down bank one
        dbprintf("\nShutting down bank one");
        //        ledStatus.one_on = FALSE;
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            LED_Off(i);
            LED_SetPinInput(i);
        }
    }
    if (LED_BANK2 & RemoveBanks) { // shut down bank two
        dbprintf("\nShutting down bank two");
        //        ledStatus.two_on = FALSE;
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            LED_Off(i + BANK2OFFSET);
            LED_SetPinInput(i + BANK2OFFSET);
        }
    }
    if (LED_BANK2 & RemoveBanks) { // shut down bank one
        dbprintf("\nShutting down bank three");
        //        ledStatus.three_on = FALSE;
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            LED_Off(i + BANK3OFFSET);
            LED_SetPinInput(i + BANK3OFFSET);
        }
    }
    dbprintf("\nLED Subsystem Enabled");

    return SUCCESS;
}

/**
 * @Function LED_ActiveBanks(void)
 * @param None
 * @return Listing of all LED Banks that are active.
 * @brief  Returns a variable of all active LED BANKS. An individual BANK can be determined
 * if active by "anding" with the LED_BANKXX Macros
 * @author Max Dunne, 2013.08.15 */
unsigned int LED_ActiveBanks(void)
{
    return ActiveLEDBanks;
}

/**
 * @Function LED_OnBank(unsigned char bank, unsigned char pattern)
 * @param bank - one of the #define'd LED_BANKx values.
 * @param pattern - which LEDs to turn on.
 * @return char - SUCCESS or ERROR
 * @brief  Forces the LEDs in (bank) to ON for each bit that is high in the pattern.
 * @author Gabriel Hugh Elkaim, 2011.12.25 01:16 */
char LED_OnBank(unsigned char bank, unsigned char pattern)
{
    char i;
    pattern &= FULLBANK;
    if (LEDActive == FALSE) {
        dbprintf("OnBank called before Init\r\n");
        return ERROR;
    }
    if (!(bank & ActiveLEDBanks)) {
        dbprintf("Attempting to Access Bank not Active in %s\r\n", __FUNCTION__);
        return ERROR;
    }
    if ((bank & LED_BANK1)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            if (pattern & (1 << i)) LED_On(i);
        }
        return SUCCESS;
    }
    if ((bank & LED_BANK2)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            if (pattern & (1 << i)) LED_On(i + BANK2OFFSET);
        }
        return SUCCESS;
    }
    if ((bank & LED_BANK3)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {

            if (pattern & (1 << i)) LED_On(i + BANK3OFFSET);
        }
        return SUCCESS;
    }
    return ERROR;
}

/**
 * @Function LED_OffBank(unsigned char bank, unsigned char pattern)
 * @param bank - one of the #define'd LED_BANKx values.
 * @param pattern - char, which LEDs to turn off.
 * @return SUCCESS or ERROR
 * @brief  Forces the LEDs in (bank) to off for each bit that is high in the pattern.
 * @author Gabriel Hugh Elkaim, 2011.12.25 01:16 */
char LED_OffBank(unsigned char bank, unsigned char pattern)
{
    char i;
    pattern &= FULLBANK;
    if (LEDActive == FALSE) {
        dbprintf("OffBank called before Init\r\n");
        return ERROR;
    }
    if (!(bank & ActiveLEDBanks)) {
        dbprintf("Attempting to Access Bank not Active in %s\r\n", __FUNCTION__);
        return ERROR;
    }
    if ((bank & LED_BANK1)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            if (pattern & (1 << i)) LED_Off(i);
        }
        return SUCCESS;
    }
    if ((bank & LED_BANK2)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            if (pattern & (1 << i)) LED_Off(i + BANK2OFFSET);
        }
        return SUCCESS;
    }
    if ((bank & LED_BANK3)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {

            if (pattern & (1 << i)) LED_Off(i + BANK3OFFSET);
        }
        return SUCCESS;
    }
    return ERROR;
}

/**
 * @Function LED_InvertBank(unsigned char bank, unsigned char pattern)
 * @param bank - one of the #define'd LED_BANKx values.
 * @param pattern - which LEDs to toggle.
 * @return SUCCESS or ERROR
 * @brief  Forces the LEDs in (bank) to toggle for each bit that is high in the pattern.
 * @author Gabriel Hugh Elkaim, 2011.12.25 01:16 */
char LED_InvertBank(unsigned char bank, unsigned char pattern)
{
    char i;
    pattern &= FULLBANK;
    if (LEDActive == FALSE) {
        dbprintf("InvertBank called before Init\r\n");
        return ERROR;
    }
    if (!(bank & ActiveLEDBanks)) {
        dbprintf("Attempting to Access Bank not Active in %s\r\n", __FUNCTION__);
        return ERROR;
    }
    if ((bank & LED_BANK1)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            if (pattern & (1 << i)) LED_Toggle(i);
        }
        return SUCCESS;
    }
    if ((bank & LED_BANK2)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            if (pattern & (1 << i)) LED_Toggle(i + BANK2OFFSET);
        }
        return SUCCESS;
    }
    if ((bank & LED_BANK3)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {

            if (pattern & (1 << i)) LED_Toggle(i + BANK3OFFSET);
        }
        return SUCCESS;
    }
    return ERROR;
}

/**
 * @Function LED_SetBank(unsigned char bank, unsigned char pattern)
 * @param bank - one of the #define'd LED_BANKx values.
 * @param pattern - sets LEDs on (1) or off (0) as in the pattern.
 * @return SUCCESS or ERROR
 * @brief  Forces the LEDs in (bank) to on (1) or off (0) to match the pattern.
 * @author Gabriel Hugh Elkaim, 2011.12.25 01:16 */
char LED_SetBank(unsigned char bank, unsigned char pattern)
{
    char i;
    pattern &= FULLBANK;
    if (LEDActive == FALSE) {
        dbprintf("SetBank called before Init\r\n");
        return ERROR;
    }
    if (!(bank & ActiveLEDBanks)) {
        dbprintf("Attempting to Access Bank not Active in %s\r\n", __FUNCTION__);
        return ERROR;
    }
    if ((bank & LED_BANK1)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            if (pattern & (1 << i)) {
                LED_On(i);
            } else LED_Off(i);
        }
        return SUCCESS;
    }
    if ((bank & LED_BANK2)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            if (pattern & (1 << i)) {
                LED_On(i + BANK2OFFSET);
            } else LED_Off(i + BANK2OFFSET);
        }
        return SUCCESS;
    }
    if ((bank & LED_BANK3)) {
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            if (pattern & (1 << i)) {
                LED_On(i + BANK3OFFSET);
            } else LED_Off(i + BANK3OFFSET);
        }
        return SUCCESS;
    }
    return ERROR;
}

/**
 * @Function LED_GetBank(unsigned char bank)
 * @param bank - one of the #define'd LED_BANKx values.
 * @return char: ERROR or state of BANK
 * @author Max Dunne, 203.10.21 01:16 */
char LED_GetBank(unsigned char bank)
{

    LEDBitBank_t outBank;
    outBank.c = 0;
    if (LEDActive == FALSE) {
        dbprintf("GetBank called before Init\r\n");
        return ERROR;
    }
    if (!(bank & ActiveLEDBanks)) {
        dbprintf("Attempting to Access Bank not Active in %s\r\n", __FUNCTION__);
        return ERROR;
    }
    if ((bank & LED_BANK1)) {
        outBank.bit0 = ~LED_BANK1_0;
        outBank.bit1 = ~LED_BANK1_1;
        outBank.bit2 = ~LED_BANK1_2;
        outBank.bit3 = ~LED_BANK1_3;

    }
    if ((bank & LED_BANK2)) {
        outBank.bit0 = ~LED_BANK2_0;
        outBank.bit1 = ~LED_BANK2_1;
        outBank.bit2 = ~LED_BANK2_2;
        outBank.bit3 = ~LED_BANK2_3;
    }
    if ((bank & LED_BANK3)) {
        outBank.bit0 = ~LED_BANK3_0;
        outBank.bit1 = ~LED_BANK3_1;
        outBank.bit2 = ~LED_BANK3_2;
        outBank.bit3 = ~LED_BANK3_3;
    }
    return outBank.c;
}



/**
 * @Function LED_End(void)
 * @return SUCCESS or ERROR
 * @brief  Ends the LED subsystem, turns active LED pins into inputs and turns LEDs 
 * off.
 * @author Gabriel Hugh Elkaim, 2011.12.25 01:16 */
char LED_End(void)
{
    char i;
    dbprintf("\nShutting down the LED subsystem");
    if (!LEDActive) {
        dbprintf("\nERROR: System not initialized");
        return ERROR;
    }
    if (LED_BANK1 & ActiveLEDBanks) { // shut down bank one
        dbprintf("\nShutting down bank one");
        //        ledStatus.one_on = FALSE;
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            LED_Off(i);
            LED_SetPinInput(i);
        }
    }
    if (LED_BANK2 & ActiveLEDBanks) { // shut down bank two
        dbprintf("\nShutting down bank two");
        //        ledStatus.two_on = FALSE;
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            LED_Off(i + BANK2OFFSET);
            LED_SetPinInput(i + BANK2OFFSET);
        }
    }
    if (LED_BANK2 & ActiveLEDBanks) { // shut down bank one
        dbprintf("\nShutting down bank three");
        //        ledStatus.three_on = FALSE;
        for (i = 0; i < NUMLEDSPERBANK; i++) {
            LED_Off(i + BANK3OFFSET);
            LED_SetPinInput(i + BANK3OFFSET);
        }
    }
    LEDActive = FALSE;
    ActiveLEDBanks = 0;

    return SUCCESS;
}

/*******************************************************************************
 * TEST HARNESS                                                                *
 ******************************************************************************/

#ifdef LED_TEST
#include <stdio.h>

#define NOPCOUNT 220000
#define DELAY() for(i=0; i< NOPCOUNT; i++) __asm("nop")

int main(void)
{
    int i, j, k, count;
    unsigned char pat;
    BOARD_Init();
    AD1PCFG = 0xFF;


    printf("\n\nLED Module Unit Test\n");
    printf("\nInitializing LED module");
    dbprintf("\nVerbose Debugging is ON");
    LED_Init();
    LED_AddBanks(LED_BANK1 | LED_BANK2 | LED_BANK3);
    //LED_RemoveBanks(LED_BANK1);

    for (count = 0; count < 10; count++) {
        printf("\npass %u", count);
        printf("\nTesting LED_OnBank");
        for (j = 0; j < NUMLEDBANKS; j++) {
            LED_OnBank((1 << j), 0x0F);
        }
        DELAY();
        printf("\nTesting LED OffBank");
        for (j = 0; j < NUMLEDBANKS; j++) {
            LED_OffBank((1 << j), 0x0F);
        }
        DELAY();
        LED_OnBank(LED_BANK1, 0x0F);
        DELAY();
        LED_OffBank(LED_BANK1, 0x0F);
        LED_OnBank(LED_BANK2, 0x0F);
        DELAY();
        LED_OffBank(LED_BANK2, 0x0F);
        LED_OnBank(LED_BANK3, 0x0F);
        DELAY();
        LED_OffBank(LED_BANK3, 0x0F);
        LED_OnBank(LED_BANK2, 0x0F);
        DELAY();
        LED_OffBank(LED_BANK2, 0x0F);
        LED_OnBank(LED_BANK1, 0x0F);
        DELAY();
        LED_OffBank(LED_BANK1, 0x0F);
        DELAY();
        LED_OnBank(LED_BANK1, 0x01);
        LED_OnBank(LED_BANK2, 0x01);
        LED_OnBank(LED_BANK3, 0x01);
        DELAY();
        printf("\nTesting LED_InvertBank");
        for (k = 0; k < 3; k++) {
            for (j = 0; j < NUMLEDBANKS; j++) {
                LED_InvertBank((1 << j), (0x03 << k));

            }
            DELAY();
        }
        for (k = 0; k < 3; k++) {
            for (j = 0; j < NUMLEDBANKS; j++) {
                LED_InvertBank((1 << j), (0x0C >> k));
            }
            DELAY();
        }

        printf("\nTesting LED_SetBank");
        pat = 0x05;
        for (k = 0; k < 4; k++) {
            for (j = 0; j < NUMLEDBANKS; j++) {
                LED_SetBank((1 << j), pat);
                printf("Input was %X and Get was %X\r\n", (pat), LED_GetBank((1 << j)));
                pat ^= 0x0F;
            }
            DELAY();
        }
    }

    LED_End();
    printf("\nLED module unit test ended");
    return 0;
}
#endif