/****************************************************************************
 Module
     ES_CheckEvents.c
 Description
     source file for the module to call the User event checking routines
 Notes
     Users should only modify the contents of the ES_EventList array.
 History
 When           Who     What/Why
 -------------- ---     --------
 10/16/11 12:32 jec      started coding
*****************************************************************************/

#include "ES_Configure.h"
#include "ES_Events.h"
#include "ES_General.h"
#include "ES_CheckEvents.h"
#include <BOARD.h>

// Include the header files for the module(s) with your event checkers. 
// This gets you the prototypes for the event checking functions.

#include EVENT_CHECK_HEADER

// Fill in this array with the names of your event checking functions

static CheckFunc * const ES_EventList[]={EVENT_CHECK_LIST };


// Implementation for public functions

/****************************************************************************
 Function
   ES_CheckUserEvents
 Parameters
   None
 Returns
   TRUE if any of the user event checkers returned TRUE, FALSE otherwise
 Description
   loop through the EF_EventList array executing the event checking functions
 Notes
   
 Author
   J. Edward Carryer, 10/25/11, 08:55
****************************************************************************/
uint8_t ES_CheckUserEvents( void ) 
{
  unsigned char i;
  // loop through the array executing the event checking functions
  for ( i=0; i< ARRAY_SIZE(ES_EventList); i++) {
    if ( ES_EventList[i]() == TRUE )
      break; // found a new event, so process it first
  }
  if ( i == ARRAY_SIZE(ES_EventList) ) // if no new events
    return (FALSE);
  else
    return(TRUE);
}
/*------------------------------- Footnotes -------------------------------*/
/*------------------------------ End of file ------------------------------*/
