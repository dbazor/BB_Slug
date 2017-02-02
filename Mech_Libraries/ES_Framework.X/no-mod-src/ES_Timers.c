//#define TEST
/****************************************************************************
 Module
     ES_Timers.c

 Description
     This is a module implementing  8 16 bit timers all using the RTI
     timebase

 Notes
     Everything is done in terms of RTI Ticks, which can change from
     application to application.

 History
 When           Who     What/Why
 -------------- ---     --------
 01/16/12 09:42 jec      added some more error checking to start & init
                         funcs to prevent starting a timer with no
                         service attached or with a time of 0
 01/15/12 16:46 jec      convert to Gen2 of Events & Services framework
 10/21/11 18:26 jec      begin conversion to work with the new Event Framework
 09/01/05 13:16 jec      converted the return types and parameters to use the
                         enumerated constants from the new header.
 08/31/05 10:23 jec      converted several return value tests in the test harness
                         to use symbolic values.
 06/15/04 09:56 jec      converted all external prefixes to TMRS12 to be sure
                         that we don't have any conflicts with the old libs
 05/28/04 13:53 jec      converted for 9S12C32 processor
 12/11/02 14:53 dos      converted for ICC11V6, unadorned char needs to be
                         called out as signed char, default is now unsigned
                         for a plain char.
 11/24/99 14:45 rmo		 updated to compile under ICC11v5.
 02/24/97 17:13 jec      added new function TMR_SetTimer. This will allow one
                                     function to set up the time, while another function
                                 actually initiates the timing.
 02/24/97 13:34 jec      Began Coding
 ****************************************************************************/

/*----------------------------- Include Files -----------------------------*/

#include <xc.h>
#include <BOARD.h>
#include <peripheral/timer.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "ES_ServiceHeaders.h"
#include "ES_Events.h"
#include "ES_PostList.h"
#include "ES_LookupTables.h"
#include "ES_Timers.h"
/*--------------------------- External Variables --------------------------*/

/*----------------------------- Module Defines ----------------------------*/
#define BITS_PER_BYTE 8
#define F_CPU 80000000L
#define F_PB F_CPU/2
#define TIMER_FREQUENCY 1000

#define NUM_TIMERS 16
#define TranslatePin(x) (1<<x)
/*------------------------------ Module Types -----------------------------*/



/*---------------------------- Module Functions ---------------------------*/


/*---------------------------- Module Variables ---------------------------*/
static unsigned int TMR_TimerArray[NUM_TIMERS];

// make this one const to get it put into flash, since it will never change


static unsigned int TMR_ActiveFlags;
static uint32_t FreeRunningTimer; /* this is used by the default RTI routine */

static pPostFunc const Timer2PostFunc[NUM_TIMERS] = {TIMER0_RESP_FUNC,
    TIMER1_RESP_FUNC,
    TIMER2_RESP_FUNC,
    TIMER3_RESP_FUNC,
    TIMER4_RESP_FUNC,
    TIMER5_RESP_FUNC,
    TIMER6_RESP_FUNC,
    TIMER7_RESP_FUNC,
    TIMER8_RESP_FUNC,
    TIMER9_RESP_FUNC,
    TIMER10_RESP_FUNC,
    TIMER11_RESP_FUNC,
    TIMER12_RESP_FUNC,
    TIMER13_RESP_FUNC,
    TIMER14_RESP_FUNC,
    TIMER15_RESP_FUNC};




/*------------------------------ Module Code ------------------------------*/

/**
 * @Function ES_Timer_Init(void)
 * @param none
 * @return None.
 * @brief  Initializes the timer module
 * @author Max Dunne, 2011.11.15 */
 void ES_Timer_Init(void) {
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_1, F_PB / TIMER_FREQUENCY);
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_3);

    mT1IntEnable(1);
}

/**
 * @Function ES_Timer_SetTimer(uint8_t Num, uint32_t NewTime)
 * @param Num - the number of the timer to set.
 * @param NewTime -  the number of milliseconds to be counted
 * @return ERROR or SUCCESS
 * @brief  sets the time for a timer, but does not make it active.
 * @author Max Dunne  2011.11.15 */
ES_TimerReturn_t ES_Timer_SetTimer(uint8_t Num, uint32_t NewTime) {
    static ES_Event NewEvent;
    // tried to set a timer that doesn't exist
    if ((Num >= NUM_TIMERS) || (Timer2PostFunc[Num] == TIMER_UNUSED) || (NewTime == 0)) {
        return ES_Timer_ERR;
    }
    TMR_TimerArray[Num] = NewTime;
    return ES_Timer_OK;
}

/**
 * @Function ES_Timer_StartTimer(uint8_t Num)
 * @param Num - the number of the timer to start
 * @return ERROR or SUCCESS
 * @brief  simply sets the active flag in TMR_ActiveFlags to resart a stopped timer.
 * @author Max Dunne, 2011.11.15 */
ES_TimerReturn_t ES_Timer_StartTimer(uint8_t Num) {
    static ES_Event NewEvent;
    // tried to set a timer that doesn't exist
    if ((Num >= NUM_TIMERS) || (TMR_TimerArray[Num] == 0)) {
        return ES_Timer_ERR;
    }
    TMR_ActiveFlags |= TranslatePin(Num); /* set timer as active */
    NewEvent.EventType = ES_TIMERACTIVE;
    NewEvent.EventParam = Num;
    // post the timeout event to the right Service
    Timer2PostFunc[Num](NewEvent);
    return ES_Timer_OK;
}

/**
 * @Function ES_Timer_StopTimer(unsigned char Num)
 * @param Num - the number of the timer to stop.
 * @return ERROR or SUCCESS
 * @brief  simply clears the bit in TimerActiveFlags associated with this timer. This 
 * will cause it to stop counting.
 * @author Max Dunne 2011.11.15 */
ES_TimerReturn_t ES_Timer_StopTimer(unsigned char Num) {
    static ES_Event NewEvent;
    if ((Num >= NUM_TIMERS) || (Timer2PostFunc[Num] == TIMER_UNUSED) || !(TMR_ActiveFlags & TranslatePin(Num))) {
        return ES_Timer_ERR; // tried to set a timer that doesn't exist
    }
    TMR_ActiveFlags &= ~(TranslatePin(Num)); // set timer as inactive
    NewEvent.EventType = ES_TIMERSTOPPED;
    NewEvent.EventParam = Num;
    // post the timeout event to the right Service
    Timer2PostFunc[Num](NewEvent);
    return ES_Timer_OK;
}

/**
 * @Function ES_Timer_InitTimer(uint8_t Num, uint32_t NewTime)
 * @param Num -  the number of the timer to start
 * @param NewTime - the number of tick to be counted
 * @return ERROR or SUCCESS
 * @brief  sets the NewTime into the chosen timer and clears any previous event flag 
 * and sets the timer actice to begin counting.
 * @author Max Dunne 2011.11.15 */
ES_TimerReturn_t ES_Timer_InitTimer(uint8_t Num, uint32_t NewTime) {
    static ES_Event NewEvent;
    if ((Num >= NUM_TIMERS) || (Timer2PostFunc[Num] == TIMER_UNUSED) || (NewTime == 0)) {
        return ES_Timer_ERR;
    }
    TMR_TimerArray[Num] = NewTime;
    TMR_ActiveFlags |= TranslatePin(Num); /* set timer as active */
    NewEvent.EventType = ES_TIMERACTIVE;
    NewEvent.EventParam = Num;
    // post the timeout event to the right Service
    Timer2PostFunc[Num](NewEvent);
    return ES_Timer_OK;
}

/**
 * Function: ES_Timer_GetTime(void)
 * @param None
 * @return FreeRunningTimer - the current value of the module variable FreeRunningTimer
 * @remark Provides the ability to grab a snapshot time as an alternative to using
 * the library timers. Can be used to determine how long between 2 events.
 * @author Max Dunne, 2011.11.15  */
uint32_t ES_Timer_GetTime(void) {
    return (FreeRunningTimer);
}

/****************************************************************************
 Function
     ES_Timer_RTI_Resp
 Parameters
     None.
 Returns
     None.
 Description
     This is the new RTI response routine to support the timer module.
     It will increment time, to maintain the functionality of the
     GetTime() timer and it will check through the active timers,
     decrementing each active timers count, if the count goes to 0, it
     will post an event to the corresponding SM and clear the active flag to
     prevent further counting.
 Notes
     None.
 Author
     J. Edward Carryer, 02/24/97 15:06
 ****************************************************************************/
void __ISR(_TIMER_1_VECTOR, ipl3auto) Timer1IntHandler(void) {
    static ES_Event NewEvent;
    uint8_t CurTimer = 0;
    mT1ClearIntFlag();
#ifdef USE_KEYBOARD_INPUT
    return;
#endif
    ++FreeRunningTimer; // keep the GetTime() timer running 
    if (TMR_ActiveFlags != 0) {
        for (CurTimer = 0; CurTimer < NUM_TIMERS; CurTimer++) {
            if ((TMR_ActiveFlags & (TranslatePin(CurTimer))) != 0) {
                if (--TMR_TimerArray[CurTimer] == 0) {
                    NewEvent.EventType = ES_TIMEOUT;
                    NewEvent.EventParam = CurTimer;
                    // post the timeout event to the right Service
                    Timer2PostFunc[CurTimer](NewEvent);
                    // and stop counting
                    TMR_ActiveFlags &= ~(TranslatePin(CurTimer));
                }

            }
        }

    }
}
/*------------------------------- Footnotes -------------------------------*/
#ifdef TEST

#include <termio.h>
#include <stdio.h>
#include <timerS12.h>


#define TIME_OUT_DELAY 1221
signed char Message[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x0d, 0};

void main(void) {
    int p;
    printf("\rStarting, should delay for 10 seconds\r");
    EF_Timer_Init(EF_Timer_RATE_8MS);
    EF_Timer_InitTimer(0, TIME_OUT_DELAY); /* TIME_OUT_DELAY  = 10s w/ 8.19mS interval */
    EF_Timer_InitTimer(1, TIME_OUT_DELAY);
    EF_Timer_InitTimer(2, TIME_OUT_DELAY);
    EF_Timer_InitTimer(3, TIME_OUT_DELAY);
    EF_Timer_InitTimer(4, TIME_OUT_DELAY);
    EF_Timer_InitTimer(5, TIME_OUT_DELAY);
    EF_Timer_InitTimer(6, TIME_OUT_DELAY);
    EF_Timer_InitTimer(7, TIME_OUT_DELAY);
    while (EF_Timer_IsTimerExpired(0) != EF_Timer_EXPIRED)
        ;
    printf("Timed Out\r");

    EF_Timer_InitTimer(7, TIME_OUT_DELAY);
    for (p = 0; p < 10000; p++); /* kill some time */
    EF_Timer_StopTimer(7);
    if (EF_Timer_IsTimerActive(7) != EF_Timer_NOT_ACTIVE)
        printf("Timer Stop Failed\r");
    else
        printf("Timer Stop Succeded, restarting timeout\r");
    EF_Timer_StartTimer(7);
    while (EF_Timer_IsTimerExpired(7) != EF_Timer_EXPIRED)
        ;

    printf("Timed Out Again\r");

    DisableInterrupts;
}
#endif
/*------------------------------ End of file ------------------------------*/

