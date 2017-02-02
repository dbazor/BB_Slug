/****************************************************************************
 Module
   TemplateFSM.c

 Revision
   1.0.1

 Description
   This is a template file for implementing flat state machines under the 
   Gen2 Events and Services Framework.

 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
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
#include "RoachFSM.h"
#include "roach.h"
#include <BOARD.h>
#include "RoachFrameworkEvents.h"
#include "ES_TattleTale.h"
#include <stdio.h>
/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/

#define FullStop() LeftMtrSpeed(0);RightMtrSpeed(0)
#define StraightForward(x) LeftMtrSpeed(x);RightMtrSpeed(x)


#define GetArray(Name)  Name ## StateNames
#define GetArrayWrapper(Name) GetArray(Name)
#define GetStateNames  StateNames
//#define Str(x)   #Str
//GetArray(__FUNCTION__)[0]=3;

#define ROACH_TIMEOUT 3

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this machine.They should be functions
   relevant to the behavior of this state machine
 */

/*---------------------------- Module Variables ---------------------------*/
// everybody needs a state variable, you may need others as well.
// type of state variable should match htat of enum in header file
static RoachState_t CurrentState;

// with the introduction of Gen2, we need a module level Priority var as well
static uint8_t MyPriority;


static const char *StateNames[] ={"Init",
"Moving",
"Stopped"};


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/****************************************************************************
 Function
     InitRoachFSM

 Parameters
     uint8_t : the priorty of this service

 Returns
     uint8_t, FALSE if error in initialization, TRUE otherwise

 Description
     Saves away the priority, sets up the initial transition and does any 
     other required initialization for this state machine
 Notes

 Author
     Max Dunne, 10/23/11, 18:55
 ****************************************************************************/
uint8_t InitRoachFSM(uint8_t Priority) {
    ES_Event ThisEvent;

    MyPriority = Priority;
    // put us into the Initial PseudoState
    CurrentState = Init;
    // post the initial transition event
    ThisEvent.EventType = ES_INIT;
    if (ES_PostToService(MyPriority, ThisEvent) == TRUE) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/****************************************************************************
 Function
     PostTemplateFSM

 Parameters
     EF_Event ThisEvent , the event to post to the queue

 Returns
     uint8_t FALSE if the Enqueue operation failed, TRUE otherwise

 Description
     Posts an event to this state machine's queue
 Notes

 Author
     J. Edward Carryer, 10/23/11, 19:25
 ****************************************************************************/
uint8_t PostRoachFSM(ES_Event ThisEvent) {
    return ES_PostToService(MyPriority, ThisEvent);
}

/****************************************************************************
 Function
    RunTemplateFSM

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
ES_Event RunRoachFSM(ES_Event ThisEvent) {
    ES_Event ReturnEvent;
    ReturnEvent.EventType = ES_NO_EVENT; // assume no errors
//    printf("Roach Received: %d->%X\r\n",ThisEvent.EventType,ThisEvent.EventParam);
//    printf("State Name: %s\r\n",StateNames[CurrentState]);
    ES_Tattle();
   // printf("%s",__FUNCTION__);
            //GetArray(__FUNCTION__)[0]=3;;
    switch (CurrentState) {
        case Init: // If current state is initial Psedudo State
            if (ThisEvent.EventType == ES_INIT)// only respond to EF_Init
            {
                // this is where you would put any actions associated with the
                // transition from the initial pseudo-state into the actual
                // initial state

                // now put the machine into the actual initial state
                CurrentState = Moving;
                StraightForward(5);
                //ES_Timer_InitTimer(ROACH_TIMEOUT,5000);
                printf("Roach initialized, driving forward\r\n");
                break;
            }
        case Moving:
            switch (ThisEvent.EventType) {
                case LIGHTLEVEL:
                    if (ThisEvent.EventParam == DARK) {
                        FullStop();
                        CurrentState = Stopped;
                        printf("Roach Stopping\r\n");
                    }
                    break;
                case ES_TIMEOUT:
                    FullStop();
                    ES_Timer_InitTimer(ROACH_TIMEOUT,5000);
                    CurrentState=Stopped;
                    printf("Roach Stopping\r\n");
                default:
                    break;
            }
            break;
        case Stopped:
            switch (ThisEvent.EventType) {
                case LIGHTLEVEL:
                    if (ThisEvent.EventParam == LIGHT) {
                        StraightForward(5);
                        CurrentState = Moving;
                        printf("Roach Moving\r\n");
                    }
                    break;
                case ES_TIMEOUT:
                    StraightForward(5);
                    ES_Timer_InitTimer(ROACH_TIMEOUT,5000);
                    CurrentState=Moving;
                    printf("Roach Moving\r\n");
                default:
                    break;
            }
            break;
        default:
            ReturnEvent.EventType = ES_ERROR;
    }
    //ES_Tail();
    return ReturnEvent;
}

/****************************************************************************
 Function
     QueryRoachSM

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
RoachState_t QueryRoachFSM(void) {
    return (CurrentState);
}

/***************************************************************************
 private functions
 ***************************************************************************/

