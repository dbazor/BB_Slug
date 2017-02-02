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

#include <BOARD.h>
#include "ES_Configure.h"
#include "ES_Framework.h"
#include "ES_TattleTale.h"
#include "serial.h"
#include <stdio.h>
#include <peripheral/timer.h>

#ifdef USE_TATTLETALE

/*----------------------------- Module Defines ----------------------------*/


#define TATTLE_POINTS 30

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this service.They should be functions
   relevant to the behavior of this service
 */

void ES_TattleTaleDump(void);


typedef struct {
    //int16_t superState;
    //int16_t subStates[TATTLE_SUBSTATE_DEPTH];
    const char* FunctionName;
    const char *StateName;
    char depth;
    ES_Event Event;


} TattleDataPoint;

static TattleDataPoint TattleData[TATTLE_POINTS];

static uint8_t tattleCount = 0;
static uint8_t tattleDepth = 0;
static uint8_t tattleRecursiveCall = FALSE;
static uint8_t tattleRecursiveCount = 0;
/*------------------------------ Module Code ------------------------------*/





/***************************************************************************
 private functions
 ***************************************************************************/

/**
 * @Function ES_TattleTaleDump(void)
 * @param None.
 * @return None.
 * @brief Dumps all events caught by tattle and tail in one trace.
 * @note  PRIVATE FUNCTION: do not call this function
 * @author Max Dunne, 2013.09.26 */
void ES_TattleTaleDump(void)
{
    uint8_t curDataPoint = 0;
    tattleDepth = 0;
    T1CONCLR = _T1CON_ON_MASK;
    printf("\r\n");
    for (curDataPoint = 0; curDataPoint < tattleCount; curDataPoint++) {
#ifdef SUPPRESS_EXIT_ENTRY_IN_TATTLE
        if ((TattleData[curDataPoint].Event.EventType != ES_ENTRY) || (TattleData[curDataPoint].Event.EventType != ES_EXIT)) {
#endif
            printf("%s(%s[%s,%X])",
                    TattleData[curDataPoint].FunctionName, TattleData[curDataPoint].StateName,          \
                        EventNames[TattleData[curDataPoint].Event.EventType], TattleData[curDataPoint].Event.EventParam);
            if (curDataPoint < (tattleCount - 1)) {
                printf("->");
            } else {
                printf(";");
            }
            while (!IsTransmitEmpty());
#ifdef SUPPRESS_EXIT_ENTRY_IN_TATTLE
        }
#endif
    }
    printf("\n");
    tattleCount = 0;
    T1CONSET = _T1CON_ON_MASK;
}

/**
 * @Function ES_AddTattlePoint(const char * FunctionName, const char * StateName, ES_Event ThisEvent)
 * @param FunctionName - name of the function called, auto generated
 * @param StateName - Current State Name, grabbed from the the StateNames array
 * @param ThisEvent - Event passed to the function
 * @return None.
 * @brief saves pointers for current call and checks to see if a recursive call is
 * occuring
 * @note  PRIVATE FUNCTION: Do Not Call this function
 * @author Max Dunne, 2013.09.26 */
void ES_AddTattlePoint(const char * FunctionName, const char * StateName, ES_Event ThisEvent)
{
    if (tattleCount < TATTLE_POINTS) {
        TattleData[tattleCount].FunctionName = FunctionName;
        TattleData[tattleCount].StateName = StateName;
        TattleData[tattleCount].Event = ThisEvent;
        TattleData[tattleCount].depth = tattleDepth;
        tattleCount++;
    }
    if ((ThisEvent.EventType == ES_EXIT) && (FunctionName == TattleData[0].FunctionName)) {
        tattleRecursiveCall = TRUE;
    }
    tattleDepth++;
}



/**
 * @Function ES_CheckTail(const char *FunctionName)
 * @param FunctionName - name of the function called, auto generated
 * @return None.
 * @brief checks to see if system is indeed at the end of a trace and calls
 * keyboard dump if so
 * @note  PRIVATE FUNCTION: Do Not Call this function
 * @author Max Dunne, 2013.09.26 */
void ES_CheckTail(const char *FunctionName)
{
    tattleDepth--;
    if (TattleData[0].FunctionName == FunctionName) {
        if(TattleData[tattleCount - 1].Event.EventType == ES_EXIT){
        }
        if ((!tattleRecursiveCall) && (TattleData[tattleCount - 1].Event.EventType != ES_EXIT)) {
            ES_TattleTaleDump();
            return;
        }
        if (tattleRecursiveCall) {
            tattleRecursiveCount++;
        }
        if (tattleRecursiveCount > 2) {
            tattleRecursiveCount = 0;
            tattleRecursiveCall = FALSE;
            ES_TattleTaleDump();
        }
    }
}
#endif
/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/

