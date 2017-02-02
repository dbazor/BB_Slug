/****************************************************************************
 Module
   SubEvadeHSM.c

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
#include "SubEvadeHSM.h"

/*----------------------------- Module Defines ----------------------------*/

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this machine.They should be functions
   relevant to the behavior of this state machine
 */

/*---------------------------- Module Variables ---------------------------*/
// everybody needs a state variable, you may need others as well.
typedef enum {
    InitPSubState,
    Stopping,
    Reversing,
    TurningLeft,
    Resuming,
} SubEvadeState_t;

static const char *StateNames[] = {
	"InitPSubState",
	"Stopping",
	"Reversing",
	"TurningLeft",
	"Resuming",
};


static SubEvadeState_t CurrentState = InitPSubState;

/*------------------------------ Module Code ------------------------------*/

/****************************************************************************
 Function
     InitSubEvadeHSM

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
uint8_t InitSubEvadeHSM(void) {
    ES_Event returnEvent;

    CurrentState = InitPSubState;
    returnEvent = RunSubEvadeHSM(INIT_EVENT);
    if (returnEvent.EventType == ES_NO_EVENT) {
        return TRUE;
    }
    return FALSE;
}

/****************************************************************************
 Function
    RunSubEvadeHSM, rename to relevant state machine in your own files

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
ES_Event RunSubEvadeHSM(ES_Event ThisEvent) {
    uint8_t makeTransition = FALSE; // use to flag transition
    SubEvadeState_t nextState;

    ES_Tattle(); // trace call stack

    switch (CurrentState) {
        case InitPSubState: // Initial Pseudo-State, use to init State Machine
            if (ThisEvent.EventType == ES_INIT) { // Handle ES_INIT only
                CurrentState = Stopping;
                ThisEvent.EventType = ES_NO_EVENT;
            }
            break;

        case Stopping: // Roach is stopped in darkenss, but got bumped
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    RoachMotorsStop();
                    nextState = Reversing;
                    makeTransition = TRUE;
                    ThisEvent.EventType = ES_NO_EVENT;
                    break;

                case ES_EXIT: // do nothing
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Reversing: // Roach is running from light
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    RoachMotorsBackward();
                    ES_Timer_InitTimer(FANCY_ROACH_TIMER, ROACH_EVADE_REVERSE_TIME);
                    break;

                case ES_TIMEOUT: // Done reversing
                    if (ThisEvent.EventParam == FANCY_ROACH_TIMER) {
                        nextState = TurningLeft;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case ES_EXIT: // kill timer
                    ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                    break;

                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case TurningLeft: // Roach is running from light
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    RoachMotorTankLeft();
                    ES_Timer_InitTimer(FANCY_ROACH_TIMER, ROACH_EVADE_LEFT_TIME);
                    break;

                case ES_TIMEOUT: // Done reversing
                    if (ThisEvent.EventParam == FANCY_ROACH_TIMER) {
                        nextState = Resuming;
                        makeTransition = TRUE;
                        ThisEvent.EventType = ES_NO_EVENT;
                    }
                    break;

                case ES_EXIT: // kill timer
                    ES_Timer_StopTimer(FANCY_ROACH_TIMER);
                    break;

                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;

        case Resuming: // Roach is running from light
            switch (ThisEvent.EventType) {
                case ES_ENTRY:
                    RoachMotorsForward();

                    PostFancyRoachHSM((ES_Event) {
                        DONE_EVADING, 0x0000});
                    InitSubEvadeHSM(); // reset machine for next entry
                    break;

                case ES_EXIT: // do nothing
                default: // all unhandled events pass the event back up to the next level
                    break;
            }
            break;
            
    } // end switch on Current State

    if (makeTransition == TRUE) { // making a state transition, send EXIT and ENTRY
        // recursively call the current state with an exit event
        RunSubEvadeHSM(EXIT_EVENT);
        CurrentState = nextState;
        RunSubEvadeHSM(ENTRY_EVENT);
    }

    ES_Tail(); // trace call stack end
    return ThisEvent;
}

/***************************************************************************
 private functions
 ***************************************************************************/

