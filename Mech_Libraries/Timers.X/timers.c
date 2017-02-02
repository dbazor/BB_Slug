/*
 * File:   timers.c
 * Author: mdunne
 *
 * Created on November 15, 2011, 9:53 AM
 */

#include <xc.h>
#include <BOARD.h>
#include <peripheral/timer.h>
#include "timers.h"


/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
 //#define TIMERS_TEST
 
#define F_PB (BOARD_GetPBClock())
#define TIMER_FREQUENCY 1000
//Change to alter number of used timers with a max of 32
#define NUM_TIMERS 16

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static unsigned int Timer_Array[NUM_TIMERS];
static unsigned int TimerActiveFlags;
static unsigned int TimerEventFlags;
static unsigned int FreeRunningTimer;


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function TIMERS_Init(void)
 * @param none
 * @return None.
 * @brief  Initializes the timer module
 * @author Max Dunne, 2011.11.15 */
void TIMERS_Init(void) {
    TimerActiveFlags = 0;
    TimerEventFlags = 0;
    FreeRunningTimer = 0;
    OpenTimer5(T5_ON | T5_SOURCE_INT | T5_PS_1_1, F_PB / TIMER_FREQUENCY);
    INTClearFlag(INT_T5);
    INTSetVectorPriority(INT_TIMER_5_VECTOR,3);
    INTSetVectorSubPriority(INT_TIMER_5_VECTOR,3);
    INTEnable(INT_T5,INT_ENABLED);
}

/**
 * @Function TIMERS_SetTimer(unsigned char Num, unsigned int NewTime)
 * @param Num - the number of the timer to set.
 * @param NewTime -  the number of milliseconds to be counted
 * @return ERROR or SUCCESS
 * @brief  sets the time for a timer, but does not make it active.
 * @author Max Dunne  2011.11.15 */
char TIMERS_SetTimer(unsigned char Num, unsigned int NewTime) {
    if (Num >= NUM_TIMERS)
        return ERROR;
    Timer_Array[Num] = NewTime;
    return SUCCESS;
}

/**
 * @Function TIMERS_StartTimer(unsigned char Num)
 * @param Num - the number of the timer to start
 * @return ERROR or SUCCESS
 * @brief  simply sets the active flag in TMR_ActiveFlags to resart a stopped timer.
 * @author Max Dunne, 2011.11.15 */
char TIMERS_StartTimer(unsigned char Num) {
    if (Num >= NUM_TIMERS)
        return ERROR;
    TimerActiveFlags |= (1 << Num);
    return SUCCESS;
}

/**
 * @Function TIMERS_StopTimer(unsigned char Num)
 * @param Num - the number of the timer to stop.
 * @return ERROR or SUCCESS
 * @brief  simply clears the bit in TimerActiveFlags associated with this timer. This 
 * will cause it to stop counting.
 * @author Max Dunne 2011.11.15 */
char TIMERS_StopTimer(unsigned char Num) {
    if (Num >= NUM_TIMERS)
        return ERROR;
    TimerActiveFlags &= ~(1 << Num);
    return SUCCESS;
}

/**
 * @Function TIMERS_InitTimer(unsigned char Num, unsigned int NewTime)
 * @param Num -  the number of the timer to start
 * @param NewTime - the number of tick to be counted
 * @return ERROR or SUCCESS
 * @brief  sets the NewTime into the chosen timer and clears any previous event flag 
 * and sets the timer actice to begin counting.
 * @author Max Dunne 2011.11.15 */
char TIMERS_InitTimer(unsigned char Num, unsigned int NewTime) {
    if (Num >= NUM_TIMERS)
        return ERROR;
    Timer_Array[Num] = NewTime;
    TimerEventFlags &= ~(1 << Num);
    TimerActiveFlags |= (1 << Num);
    return SUCCESS;
}

/**
 * @Function TIMERS_IsTimerActive(unsigned char Num)
 * @param Num - the number of the timer to check
 * @return ERROR or TIMER_NOT_ACTIVE or TIMER_ACTIVE
 * @brief  used to determine if a timer is currently counting.
 * @author Max Dunne   2011.11.15 */
char TIMERS_IsTimerActive(unsigned char Num) {
    if (Num >= NUM_TIMERS)
        return ERROR;
    if ((TimerActiveFlags & (1 << Num)) != 0) {
        return TIMER_ACTIVE;
    } else {
        return TIMER_NOT_ACTIVE;
    }
}

/**
 * @Function TIMERS_IsTimerExpired(unsigned char Num)
 * @param Num - the number of the timer to check
 * @return ERROR or TIMER_NOT_EXPIRED or TIMER_EXPIRED
 * @brief  used to determine if a timer is currently expired.
 * @author Max Dunne   2011.11.15 */
char TIMERS_IsTimerExpired(unsigned char Num) {
    if (Num >= NUM_TIMERS)
        return ERROR;
    if ((TimerEventFlags & (1 << Num)) != 0) {
        return TIMER_EXPIRED;
    } else {
        return TIMER_NOT_EXPIRED;
    }
}

/**
 * @Function TIMERS_ClearTimerExpired(unsigned char Num) 
 * @param Num - the timer whose event flag should be cleared.
 * @return ERROR or SUCCESS
 * @brief  simply clears the appropriate bit in Event Flags to show that the event 
 * has been serviced.
 * @author Max Dunne  211.11.15 */
char TIMERS_ClearTimerExpired(unsigned char Num) {
    if (Num >= NUM_TIMERS)
        return ERROR;
    TimerEventFlags &= ~(1 << Num);
    return SUCCESS;
}

/**
 * Function: TIMERS_GetTime
 * @param None
 * @return FreeRunningTimer, the current value of the module variable FreeRunningTimer
 * @remark Provides the ability to grab a snapshot time as an alternative to using
 * the library timers. Can be used to determine how long between 2 events.
 * @author Max Dunne 2011.11.15  */
unsigned int TIMERS_GetTime(void) {
    return FreeRunningTimer;
}

/**
 * @Function Timer5IntHandler(void)
 * @param None.
 * @return None.
 * @brief  This is the interrupt handler to support the timer module. It will increment 
 * time, to maintain the functionality of the GetTime() timer and it will check 
 * through the active timers, decrementing each active timers count, if the 
 * count goes to 0, it will set the associated event flag and clear the active 
 * flag to prevent further counting.
 * @author Max Dunne 2011.11.15 */

void __ISR(_TIMER_5_VECTOR, ipl3auto) Timer5IntHandler(void) {
    INTClearFlag(INT_T5);
    FreeRunningTimer++;
    char CurTimer = 0;
    if (TimerActiveFlags != 0) {
        for (CurTimer = 0; CurTimer < NUM_TIMERS; CurTimer++) {
            if ((TimerActiveFlags & (1 << CurTimer)) != 0) {
                if (--Timer_Array[CurTimer] == 0) {
                    TimerEventFlags |= (1 << CurTimer);
                    TimerActiveFlags &= ~(1 << CurTimer);
                }
            }
        }
    }
}




#ifdef TIMERS_TEST
    #include "serial.h"
    #include "timers.h"
#include <stdio.h>
    #define TIMERS_IN_TEST NUM_TIMERS
//#include <plib.h>

int main(void) {
    int i = 0;
    BOARD_Init();
    TIMERS_Init();


    printf("\nUno Timers Test Harness\n");
    printf("Setting each timer for one second longer than the last and waiting for all to expire.  There are %d available timers\n", TIMERS_IN_TEST);
    for (i = 0; i <= TIMERS_IN_TEST; i++) {
        TIMERS_InitTimer(i, (i + 1)*1000); //for second scale
    }
    while (TIMERS_IsTimerActive(TIMERS_IN_TEST - 1) == TIMER_ACTIVE) {
        for (i = 0; i <= TIMERS_IN_TEST; i++) {
            if (TIMERS_IsTimerExpired(i) == TIMER_EXPIRED) {
                printf("Timer %d has expired and the free running counter is at %d\n", i, TIMERS_GetTime());
                TIMERS_ClearTimerExpired(i);
            }
        }
    }
    printf("All timers have ended\n");
    printf("Setting and starting 1st timer to 2 seconds using alternative method. \n");
    TIMERS_SetTimer(0, 2000);
    TIMERS_StartTimer(0);
    while (TIMERS_IsTimerExpired(0) != TIMER_EXPIRED);
    printf("2 seconds should have elapsed\n");
    printf("Starting 1st timer for 8 seconds but also starting 2nd timer for 4 second\n");
    TIMERS_InitTimer(0, 8000);
    TIMERS_InitTimer(1, 4000);
    while (TIMERS_IsTimerExpired(1) != TIMER_EXPIRED);
    printf("4 seconds have passed and now stopping 1st timer\n");
    TIMERS_StopTimer(0);
    printf("Waiting 6 seconds to verifiy that 1st timer has indeed stopped\n");
    TIMERS_InitTimer(1, 3000);
    i = 0;
    while (TIMERS_IsTimerActive(1) == TIMER_ACTIVE) {
        if (TIMERS_IsTimerExpired(0) == TIMER_EXPIRED) {
            i++;
            TIMERS_ClearTimerExpired(0);
        }
    }
    if (i == 0) {
        printf("Timer did not expire, module working correctly\n");
    } else {
        printf("Timer did expire, module not working correctly\n");
    }

    return 0;
}

#endif