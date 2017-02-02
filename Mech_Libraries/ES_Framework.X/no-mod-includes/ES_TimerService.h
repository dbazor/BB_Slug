/****************************************************************************
 
  Header file for template service 
  based on the Gen 2 Events and Services Framework

 ****************************************************************************/

#ifndef ServTemplate_H
#define ServTemplate_H

#include <inttypes.h>

#include "ES_Configure.h"
#include "ES_Events.h"


#define TIMERS_USED 2


// Public Function Prototypes
/**
 * @Function InitTimerService(uint8_t Priority)
 * @param uint8_t - the priorty of this service
 * @return uint8_t - FALSE if error in initialization, TRUE otherwise
 * @brief  Saves away the priority, and does any  other required initialization for
 * this service
 * @author Max Dunne   2013.01.04 */
uint8_t InitTimerService ( uint8_t Priority );

/**
 * @Function PostTimerService(ES_Event ThisEvent)
 * @param ThisEvent - the event to post to the queue
 * @return FALSE or TRUE
 * @brief  Posts an event to the timers queue
 * @author Max Dunne   2013.01.04 */
uint8_t PostTimerService( ES_Event ThisEvent );


/**
 * @Function RunTimerService(ES_Event ThisEvent)
 * @param ES_Event - the event to process
 * @return ES_NO_EVENT or ES_ERROR
 * @brief  accepts the timer events and updates the state arrays
 * @author Max Dunne   2013.01.04 */
ES_Event RunTimerService( ES_Event ThisEvent );



/**
 * @Function IsTimerActive(unsigned char Num)
 * @param Num - the number of the timer to check
 * @return ERROR or TRUE or FALSE
 * @brief  used to determine if a timer is currently active.
 * @author Max Dunne   2013.01.04 */
int8_t IsTimerActive(unsigned char Num);

/**
 * @Function IsTimerExpired(unsigned char Num)
 * @param Num - the number of the timer to check
 * @return ERROR or TRUE or FALSE
 * @brief  used to determine if a timer is currently expired.
 * @author Max Dunne   2013.01.04 */
int8_t IsTimerExpired(unsigned char Num);

/**
 * @Function IsTimerStopped(unsigned char Num)
 * @param Num - the number of the timer to check
 * @return ERROR or TRUE or FALSE
 * @brief  used to determine if a timer is currently stopped.
 * @author Max Dunne   2013.01.04 */
int8_t IsTimerStopped(unsigned char Num);

/**
 * @Function GetUserTimerState(unsigned char Num)
 * @param Num - the number of the timer to check
 * @return ES_EventTyp_t current state of timer
 * @brief  used to get the current timer state.
 * @author Max Dunne   2013.01.04 */
ES_EventTyp_t GetUserTimerState(unsigned char Num);


#endif /* ServTemplate_H */

