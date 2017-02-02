
#ifndef ES_TattleTale_H
#define ES_TattleTale_H

#include <inttypes.h>
#include <ES_Configure.h>


// Public Function Prototypes



#ifdef USE_TATTLETALE

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
void ES_AddTattlePoint(const char * FunctionName, const char * StateName, ES_Event ThisEvent);


/**
 * @Function ES_CheckTail(const char *FunctionName)
 * @param FunctionName - name of the function called, auto generated
 * @return None.
 * @brief checks to see if system is indeed at the end of a trace and calls
 * keyboard dump if so
 * @note  PRIVATE FUNCTION: Do Not Call this function
 * @author Max Dunne, 2013.09.26 */
void ES_CheckTail(const char *FunctionName);



/**
 * @Function ES_Tattle()
 * @param None.
 * @return None.
 * @brief called at the beginning of all state machines
 * @author Max Dunne, 2013.09.26 */
#define ES_Tattle() ES_AddTattlePoint(__FUNCTION__,StateNames[CurrentState],ThisEvent)

/**
 * @Function ES_Tail()
 * @param None.
 * @return None.
 * @brief called at the end of top level state machines
 * @author Max Dunne, 2013.09.26 */
#define ES_Tail() ES_CheckTail(__FUNCTION__)
#else
#define ES_Tattle()
#define ES_Tail()
#endif


#endif 

