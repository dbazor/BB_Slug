/****************************************************************************
 Module
   TemplateService.c

 Revision
   1.0.1

 Description
   This is a template file for implementing a simple service under the 
   Gen2 Events and Services Framework.

 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 01/16/12 09:58 jec      began conversion from TemplateFSM.c
 ****************************************************************************/
/*----------------------------- Include Files -----------------------------*/
/* include header files for this state machine as well as any machines at the
   next lower level in the hierarchy that are sub-machines to this machine
 */
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "ES_TimerService.h"
#include <BOARD.h>
#include <ES_TattleTale.h>

#ifdef TIMER_SERVICE_TEST
#include <stdio.h>

static enum {
    init, expired, runstop
} timerServiceTestingState;
#endif

/*----------------------------- Module Defines ----------------------------*/

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this service.They should be functions
   relevant to the behavior of this service
 */

/*---------------------------- Module Variables ---------------------------*/
// with the introduction of Gen2, we need a module level Priority variable
static uint8_t MyPriority;

static ES_EventTyp_t UserTimerStates[TIMERS_USED];


/*------------------------------ Module Code ------------------------------*/

/**
 * @Function InitTimerService(uint8_t Priority)
 * @param uint8_t - the priorty of this service
 * @return uint8_t - FALSE if error in initialization, TRUE otherwise
 * @brief  Saves away the priority, and does any  other required initialization for 
 * this service
 * @author Max Dunne   2013.01.04 */
uint8_t InitTimerService(uint8_t Priority) {
    ES_Event ThisEvent;

    MyPriority = Priority;

    // post the initial transition event
    ThisEvent.EventType = ES_INIT;
    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }

}

/**
 * @Function PostTimerService(ES_Event ThisEvent)
 * @param ThisEvent - the event to post to the queue
 * @return FALSE or TRUE
 * @brief  Posts an event to the timers queue
 * @author Max Dunne   2013.01.04 */
uint8_t PostTimerService(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

/**
 * @Function RunTimerService(ES_Event ThisEvent)
 * @param ES_Event - the event to process
 * @return ES_NO_EVENT or ES_ERROR 
 * @brief  accepts the timer events and updates the state arrays
 * @author Max Dunne   2013.01.04 */
ES_Event RunTimerService(ES_Event ThisEvent) {
    ES_Event ReturnEvent;
    
    ReturnEvent.EventType = ES_NO_EVENT; // assume no errors

    switch (ThisEvent.EventType) {
        case ES_INIT:
            break;
        case ES_TIMEOUT:
        case ES_TIMERACTIVE:
        case ES_TIMERSTOPPED:
            UserTimerStates[ThisEvent.EventParam] = ThisEvent.EventType;
            break;
        default:
            //ReturnEvent.EventType = ES_ERROR;
            break;
    }

#ifdef TIMER_SERVICE_TEST
    {
        uint8_t i;
        switch (timerServiceTestingState) {
            case init:
                if (ThisEvent.EventType == ES_INIT) {
                    printf("Timer Module INITED succesfully\r\n");
                    break;
                }
                printf("Timer %d had event %d at time %d\r\n", ThisEvent.EventParam, ThisEvent.EventType, ES_Timer_GetTime());
                if ((ThisEvent.EventParam == (TIMERS_USED - 1)) && (ThisEvent.EventType == ES_TIMERACTIVE)) {
                    timerServiceTestingState = expired;
                    printf("Testing timer user functions [expired][stopped][active]{state}\r\n");
                }
                break;

            case expired:
                for (i = 0; i < TIMERS_USED; i++) {
                    printf("(%d):[%d,%d,%d]{%d} ", i, IsTimerExpired(i), IsTimerStopped(i), IsTimerActive(i), GetUserTimerState(i));
                }
                printf("\r\n");
                if ((ThisEvent.EventParam == (TIMERS_USED - 1)) && (ThisEvent.EventType == ES_TIMEOUT)) {
                    timerServiceTestingState = runstop;
                    ES_Timer_InitTimer(0, 500);
                    for (i = 1; i < TIMERS_USED; i++) {
                        ES_Timer_SetTimer(i, 1000);
                        ES_Timer_StartTimer(i);
                    }
                }
                break;

            case runstop:
                printf("Timer %d had event %d at time %d\r\n", ThisEvent.EventParam, ThisEvent.EventType, ES_Timer_GetTime());
                if ((ThisEvent.EventParam == (0)) && (ThisEvent.EventType == ES_TIMEOUT)) {
                    for (i = 1; i < TIMERS_USED; i++) {
                        ES_Timer_StopTimer(i);
                    }
                }
                if ((ThisEvent.EventParam == (TIMERS_USED - 1)) && (ThisEvent.EventType == ES_TIMERSTOPPED)) {
                    printf("Testing of User Timer Functions is complete.\r\n");
                }

                break;
            default:
                ReturnEvent.EventType = ES_ERROR;
                break;
        }

    }
#endif
    //ES_Tail();
    return ReturnEvent;
}

/**
 * @Function IsTimerExpired(unsigned char Num)
 * @param Num - the number of the timer to check
 * @return ERROR or TRUE or FALSE 
 * @brief  used to determine if a timer is currently expired.
 * @author Max Dunne   2013.01.04 */
int8_t IsTimerExpired(unsigned char Num) {
    if (Num >= TIMERS_USED) {
        return ERROR;
    }
    if (UserTimerStates[Num] == ES_TIMEOUT) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function IsTimerActive(unsigned char Num)
 * @param Num - the number of the timer to check
 * @return ERROR or TRUE or FALSE
 * @brief  used to determine if a timer is currently active.
 * @author Max Dunne   2013.01.04 */
int8_t IsTimerActive(unsigned char Num) {
    if (Num >= TIMERS_USED) {
        return ERROR;
    }
    if (UserTimerStates[Num] == ES_TIMERACTIVE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function IsTimerStopped(unsigned char Num)
 * @param Num - the number of the timer to check
 * @return ERROR or TRUE or FALSE
 * @brief  used to determine if a timer is currently stopped.
 * @author Max Dunne   2013.01.04 */
int8_t IsTimerStopped(unsigned char Num) {
    if (Num >= TIMERS_USED) {
        return ERROR;
    }
    if (UserTimerStates[Num] == ES_TIMERSTOPPED) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @Function GetUserTimerState(unsigned char Num)
 * @param Num - the number of the timer to check
 * @return ES_EventTyp_t current state of timer
 * @brief  used to get the current timer state.
 * @author Max Dunne   2013.01.04 */
ES_EventTyp_t GetUserTimerState(unsigned char Num) {
    if (Num >= TIMERS_USED) {
        return ERROR;
    }
    return UserTimerStates[Num];
}

/***************************************************************************
 private functions
 ***************************************************************************/

//#define TIMER_SERVICE_TEST
#ifdef TIMER_SERVICE_TEST
#include <stdio.h>

void main(void) {

    ES_Return_t ErrorType;
    uint8_t i;
    BOARD_Init();
    // When doing testing, it is usefull to annouce just which program
    // is running.

    printf("Starting Timer Service Test Harness \r\n");
    printf("using the 2nd Generation Events & Services Framework\n\r");

    // Your hardware initialization function calls go here

    // now initialize the Events and Services Framework and start it running
    ErrorType = ES_Initialize();

    timerServiceTestingState = init;
    for (i = 0; i <= TIMERS_USED; i++) {
        ES_Timer_InitTimer(i, (i + 1)*1000); //for second scale
    }
    if (ErrorType == Success) {

        ErrorType = ES_Run();

    }
    //if we got to here, there was an error
    switch (ErrorType) {
        case FailedPointer:
            printf("Failed on NULL pointer");
            break;
        case FailedInit:
            printf("Failed Initialization");
            break;
        default:
            printf("Other Failure");
            break;
    }
    for (;;)
        ;

};


#endif
/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/

