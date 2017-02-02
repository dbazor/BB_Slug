/****************************************************************************
 Module
     ES_Framework.c
 Description
     source file for the core functions of the Events & Services framework
 Notes
     
 History
 When           Who     What/Why
 -------------- ---     --------
 * 9/14/14      maxl    condesning into 3 files
 01/30/12 19:31 jec      moved call to ES_InitTimers into the ES_Initialize
                         this rewuired adding a parameter to ES_Initialize.
 01/15/12 12:15 jec      major revision for Gen2
 10/17/11 12:24 jec      started coding
 *****************************************************************************/
/*----------------------------- Include Files -----------------------------*/
#include "ES_Configure.h"
#include "ES_Queue.h"
#include "ES_PostList.h"
#include "ES_CheckEvents.h"
#include "ES_LookupTables.h"
#include "ES_Timers.h"
#include "ES_Framework.h"
#include "ES_KeyboardInput.h"
#include "ES_Events.h"
#include <stdio.h>
#include <BOARD.h>
//#include <termio.h>

// Include the header files for the state machines.
// This gets you the prototypes for the public state machine functions.

#include "ES_ServiceHeaders.h"
#include "serial.h"


/*----------------------------- Module Defines ----------------------------*/
typedef uint8_t InitFunc_t(uint8_t Priority);
typedef ES_Event RunFunc_t(ES_Event ThisEvent);

typedef InitFunc_t * pInitFunc;
typedef RunFunc_t * pRunFunc;

#define NULL_INIT_FUNC ((pInitFunc)0)

typedef struct {
    InitFunc_t *InitFunc; // Service Initialization function
    RunFunc_t *RunFunc; // Service Run function
} ES_ServDesc_t;

typedef struct {
    ES_Event *pMem; // pointer to the memory
    uint8_t Size; // how big is it
} ES_QueueDesc_t;

/*---------------------------- Module Functions ---------------------------*/
static uint8_t CheckSystemEvents(void);

/*---------------------------- Module Variables ---------------------------*/
/****************************************************************************/
// You fill in this array with the names of the service init & run functions
// for each service that you use.
// The order is: InitFunction, RunFunction
// The first enry, at index 0, is the lowest priority, with increasing 
// priority with higher indices

static ES_ServDesc_t const ServDescList[] = {
    {SERV_0_INIT, SERV_0_RUN} /* lowest priority  always present */
#if NUM_SERVICES > 1
    ,
    {SERV_1_INIT, SERV_1_RUN}
#endif
#if NUM_SERVICES > 2
    ,
    {SERV_2_INIT, SERV_2_RUN}
#endif
#if NUM_SERVICES > 3
    ,
    {SERV_3_INIT, SERV_3_RUN}
#endif
#if NUM_SERVICES > 4
    ,
    {SERV_4_INIT, SERV_4_RUN}
#endif
#if NUM_SERVICES > 5
    ,
    {SERV_5_INIT, SERV_5_RUN}
#endif
#if NUM_SERVICES > 6
    ,
    {SERV_6_INIT, SERV_6_RUN}
#endif
#if NUM_SERVICES > 7
    ,
    {SERV_7_INIT, SERV_7_RUN}
#endif

};

/****************************************************************************/
// Initialize this variable with the name of the posting function that you
// want executed when a new keystroke is detected.

static pPostFunc const pPostKeyFunc = POST_KEY_FUNC;

/****************************************************************************/
// The queues for the services

static ES_Event Queue0[SERV_0_QUEUE_SIZE + 1];
#if NUM_SERVICES > 1
static ES_Event Queue1[SERV_1_QUEUE_SIZE + 1];
#endif
#if NUM_SERVICES > 2
static ES_Event Queue2[SERV_2_QUEUE_SIZE + 1];
#endif
#if NUM_SERVICES > 3
static ES_Event Queue3[SERV_3_QUEUE_SIZE + 1];
#endif
#if NUM_SERVICES > 4
static ES_Event Queue4[SERV_4_QUEUE_SIZE + 1];
#endif
#if NUM_SERVICES > 5
static ES_Event Queue5[SERV_5_QUEUE_SIZE + 1];
#endif
#if NUM_SERVICES > 6
static ES_Event Queue6[SERV_6_QUEUE_SIZE + 1];
#endif
#if NUM_SERVICES > 7
static ES_Event Queue7[SERV_7_QUEUE_SIZE + 1];
#endif

/****************************************************************************/
// array of queue descriptors for posting by priority level

static ES_QueueDesc_t const EventQueues[NUM_SERVICES] = {
    { Queue0, ARRAY_SIZE(Queue0)}
#if NUM_SERVICES > 1
    ,
    { Queue1, ARRAY_SIZE(Queue1)}
#endif
#if NUM_SERVICES > 2
    ,
    { Queue2, ARRAY_SIZE(Queue2)}
#endif
#if NUM_SERVICES > 3
    ,
    { Queue3, ARRAY_SIZE(Queue3)}
#endif
#if NUM_SERVICES > 4
    ,
    { Queue4, ARRAY_SIZE(Queue4)}
#endif
#if NUM_SERVICES > 5
    ,
    { Queue5, ARRAY_SIZE(Queue5)}
#endif
#if NUM_SERVICES > 6
    ,
    { Queue6, ARRAY_SIZE(Queue6)}
#endif
#if NUM_SERVICES > 7
    ,
    { Queue7, ARRAY_SIZE(Queue7)}
#endif
};

/****************************************************************************/
// Variable used to keep track of which queues have events in them

uint8_t Ready;

/*------------------------------ Module Code ------------------------------*/

/****************************************************************************
 Function
   ES_Initialize
 Parameters
   None
 Returns
   ES_Return_t : FailedPointer if any of the function pointers are NULL
                 FailedInit if any of the initialization functions failed
 Description
   Initialize all the services and tests for NULL pointers in the array
 Notes

 Author
   J. Edward Carryer, 10/23/11,
 ****************************************************************************/
ES_Return_t ES_Initialize(void) {
    unsigned char i;
    ES_Timer_Init(); // start up the timer subsystem
    // loop through the list testing for NULL pointers and
    for (i = 0; i < ARRAY_SIZE(ServDescList); i++) {
        if ((ServDescList[i].InitFunc == (pInitFunc) 0) ||
                (ServDescList[i].RunFunc == (pRunFunc) 0))
            return FailedPointer; // protect against NULL pointers
        // and initializing the event queues (must happen before running inits)
        ES_InitQueue(EventQueues[i].pMem, EventQueues[i].Size);
        // executing the init functions
        if (ServDescList[i].InitFunc(i) != TRUE)
            return FailedInit; // this is a failed initialization
    }
    return Success;
}

/****************************************************************************
 Function
   ES_Run
 Parameters
   None
 *
 Returns
   ES_Return_t : FailedRun is any of the run functions failed during execution
 Description
   This is the main framework function. It searches through the state
   machines to find one with a non-empty queue and then executes the
   state machine to process the event in its queue.
   while all the queues are empty, it searches for system generated or
   user generated events.
 Notes
   this function only returns in case of an error
 Author
   J. Edward Carryer, 10/23/11,
   M. Dunne, 2013.09.18
 ****************************************************************************/
ES_Return_t ES_Run(void) {
    // make these static to improve speed
    uint8_t HighestPrior;
    static ES_Event ThisEvent;
    uint8_t CurService;
    uint8_t CurServiceMask;

    while (1) { // stay here unless we detect an error condition

        // loop through the list executing the run functions for services
        // with a non-empty queue
        while (Ready != 0) {
            //if (IsTransmitEmpty()) {
                 //printf("handling queues: %X\r\n", Ready);
            //}
            for (CurService = 0; CurService < NUM_SERVICES; CurService++) {
                CurServiceMask = 1 << CurService;
                //printf("handling queue: %X: %X: %X\r\n", CurService,Ready,Ready & CurServiceMask);
                if (Ready & CurServiceMask) {
                    if (ES_DeQueue(EventQueues[CurService].pMem, &ThisEvent) == 0) {
                        Ready &= ~CurServiceMask; // mark queue as now empty
                    }
                    if (ServDescList[CurService].RunFunc(ThisEvent).EventType == ES_ERROR) {
                        return FailedRun;
                    }
                }
            }
        }
        // all the queues are empty, so look for new system or user detected events
        if (CheckSystemEvents() == FALSE)
#ifndef USE_KEYBOARD_INPUT
            ES_CheckUserEvents();
#else
            ;
#endif

    }
}

/****************************************************************************
 Function
   ES_PostAll
 Parameters
   ES_Event : The Event to be posted
 Returns
   uint8_t : FALSE if any of the post functions failed during execution
 Description
   posts to all of the services' queues 
 Notes

 Author
   J. Edward Carryer, 01/15/12,
 ****************************************************************************/
uint8_t ES_PostAll(ES_Event ThisEvent) {

    unsigned char i;
    // loop through the list executing the post functions
    for (i = 0; i < ARRAY_SIZE(EventQueues); i++) {
        if (ES_EnQueueFIFO(EventQueues[i].pMem, ThisEvent) != TRUE) {
            break; // this is a failed post
        } else {
            Ready |= (1 << i); // show queue as non-empty
        }
    }
    if (i == ARRAY_SIZE(EventQueues)) { // if no failures
        return (TRUE);
    } else {
        return (FALSE);
    }
}

/****************************************************************************
 Function
   ES_PostToService
 Parameters
   uint8_t : Which service to post to (index into ServDescList)
   ES_Event : The Event to be posted
 Returns
   uint8_t : FALSE if the post function failed during execution
 Description
   posts to one of the services' queues
 Notes
   used by the timer library to associate a timer with a state machine
 Author
   J. Edward Carryer, 01/16/12,
 ****************************************************************************/
uint8_t ES_PostToService(uint8_t WhichService, ES_Event TheEvent) {
    if ((WhichService < ARRAY_SIZE(EventQueues)) &&
            (ES_EnQueueFIFO(EventQueues[WhichService].pMem, TheEvent) ==
            TRUE)) {
        Ready |= (1 << WhichService); // show queue as non-empty
        return TRUE;
    } else
        return FALSE;
}


//*********************************
// private functions
//*********************************

/****************************************************************************
 Function
   CheckSystemEvents
 Parameters
   None
 Returns
   uint8_t : TRUE if a system event was detected
 Description
   check for system generated events and uses pPostKeyFunc to post to one
   of the state machine's queues
 Notes
   currently only tests for incoming keystrokes
 Author
   J. Edward Carryer, 10/23/11, 
 ****************************************************************************/
static uint8_t CheckSystemEvents(void) {

    //  if ( kbhit() != 0 ) // new key waiting?
    //  {
    //    ES_Event ThisEvent;
    //    ThisEvent.EventType = ES_NEW_KEY;
    //    ThisEvent.EventParam = getchar();
    //    (*pPostKeyFunc)( ThisEvent );
    //    return TRUE;
    //  }
#ifdef USE_KEYBOARD_INPUT
    if (!IsReceiveEmpty()) {
        ES_Event ThisEvent;
        ThisEvent.EventType = ES_KEYINPUT;
        ThisEvent.EventParam = GetChar();
        PostKeyboardInput(ThisEvent);
        return TRUE;
    }
#endif
    return FALSE;
}

/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/
