/****************************************************************************
 Module
   FancyRoachHSM.c

 Revision
   1.0.2

 Description
   This is a template file for implementing hierarchical state machines under the
   Gen2 Events and Services Framework.

 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 09/13/13 15:17 ghe      added tattletail functionality and recursive calls
 01/15/12 11:12 jec      revisions for Gen2 framework
 11/07/11 11:26 jec      made the queue static
 10/30/11 17:59 jec      fixed references to CurrentEvent in RunTemplateSM()
 10/23/11 18:20 jec      began conversion from SMTemplate.c (02/20/07 rev)
 ****************************************************************************/
/*----------------------------- Include Files -----------------------------*/
/* include header files for this state machine as well as any machines at the
   next lower level in the hierarchy that are sub-machines to this machine
 */
#include "ES_Configure.h"
#include "ES_Framework.h"
#ifdef DEV
#include "ES_TattleTale.h"
#endif
#include "BOARD.h"
#include "FancyRoachHSM.h"
#include "SubStoppedHSM.h"
#include "SubDanceHSM.h"
#include "SubEvadeHSM.h"
#include "serial.h"
#include <stdio.h>

/*----------------------------- Module Defines ----------------------------*/
#define PrintLoc()              printf("%s: %d\r\n",__FILE__,__LINE__); \
            while(!IsTransmitEmpty());
/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this machine.They should be functions
   relevant to the behavior of this state machine
 */

/*---------------------------- Module Variables ---------------------------*/
// everybody needs a state variable, you may need others as well.
typedef enum {
    InitPState,
    Stopped,
    Fleeing,
    Evading,
} FancyRoachState_t;

static const char *StateNames[] = {
	"InitPState",
	"Stopped",
	"Fleeing",
	"Evading",
};


static FancyRoachState_t CurrentState;

// with the introduction of Gen2, we need a module level Priority var as well
static uint8_t MyPriority;





/*------------------------------ Module Code ------------------------------*/

/****************************************************************************
 Function
     InitFancyRoachHSM

 Parameters
     uint8_t : the priorty of this service

 Returns
     bool, FALSE if error in initialization, TRUE otherwise

 Description
     Saves away the priority, sets up the initial transition and does any
     other required initialization for this state machine
 Notes

 Author
     J. Edward Carryer, 10/23/11, 18:55
 ****************************************************************************/
uint8_t InitFancyRoachHSM(uint8_t Priority) {
    MyPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = InitPState;
    // post the initial transition event
    if (ES_PostToService(MyPriority, INIT_EVENT) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/****************************************************************************
 Function
     PostFancyRoachHSM

 Parameters
     EF_Event ThisEvent , the event to post to the queue

 Returns
     boolean False if the Enqueue operation failed, True otherwise

 Description
     Posts an event to this state machine's queue
 Notes

 Author
     J. Edward Carryer, 10/23/11, 19:25
 ****************************************************************************/
uint8_t PostFancyRoachHSM(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

/****************************************************************************
 Function
    RunFancyRoachHSM, rename to relevant state machine in your own files

 Parameters
   ES_Event : the event to process

 Returns
   ES_Event, ES_NO_EVENT if no error ES_ERROR otherwise

 Description
   add your description here
 Notes
   uses nested switch/case to implement the machine.
 Author
   J. Edward Carryer, 01/15/12, 15:23
 ****************************************************************************/
ES_Event RunFancyRoachHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    FancyRoachState_t nextState;
    // printf("current event: %s\r\n",EventNames[ThisEvent.EventType]);
    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitPState: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to ES_Init
            {
                // Initialize all sub-state machines
                //PrintLoc();
                InitSubStoppedHSM();
                //PrintLoc();
                InitSubDanceHSM();
                //PrintLoc();
                InitSubEvadeHSM();
                //PrintLoc();
                nextState = Stopped;
                makeTransition = TRUE;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case Stopped: // Roach is stopped in darkenss
            ThisEvent = RunSubStoppedHSM(ThisEvent); // run sub-state machine for this state
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        RoachMotorsStop();
                        break;

                    case ES_EXIT: // reset lower level state machine
                        InitSubStoppedHSM();
                        break;


                    case WAS_DARK_NOW_LIGHT: // start fleeing
                        nextState = Fleeing;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;


                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break;

        case Fleeing: // Roach is running from light
            ThisEvent = RunSubDanceHSM(ThisEvent); // run dance sub-statemachine
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        RoachMotorsForward();
                        break;

                    case WAS_LIGHT_NOW_DARK: // found darkness, stop
                        nextState = Stopped;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case BUMPED: // ran into something
                        nextState = Evading;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_EXIT: // reset sub state machine
                        InitSubDanceHSM();
                        break;

                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break;
            
        case Evading: // Roach ran into something
            ThisEvent = RunSubEvadeHSM(ThisEvent); // run evade sub-statemachine
            if (ThisEvent.EventType != ES_NO_EVENT) { // An event is active
                switch (ThisEvent.EventType) {
                    case ES_ENTRY:
                        RoachMotorsStop();
                        break;

                    case DONE_EVADING: // evade substate finished
                        nextState = Fleeing;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case WAS_LIGHT_NOW_DARK:
                        nextState = Stopped;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                        break;

                    case ES_EXIT: // nothing to do
                    default: // all unhandled events pass the event back up to the next level
                        break;
                }
            }
            break;
            
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        //PrintLoc();
        // printf("current event at state transition: %s\r\n",EventNames[ThisEvent.EventType]);
        RunFancyRoachHSM(EXIT_EVENT);
        // PrintLoc();
        CurrentState = nextState;
        RunFancyRoachHSM(ENTRY_EVENT);
        // PrintLoc();
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}

/****************************************************************************
 Function
     QueryFancyRoachHSM

 Parameters
     None

 Returns
     TemplateState_t The current state of the Template state machine

 Description
     returns the current state of the Template state machine
 Notes

 Author
     J. Edward Carryer, 10/23/11, 19:21
 ****************************************************************************/
FancyRoachState_t QueryFancyRoachHSM(void) {
    return (CurrentState);
}

/***************************************************************************
 private functions
 ***************************************************************************/

