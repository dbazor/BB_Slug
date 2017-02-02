/****************************************************************************
 Module
     ES_Framework.h
 Description
     header file for use with the top level functions of the ES Event Framework
 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 9/14/14        MaxL    condensing into 3 files
 10/17/06 07:41 jec      started coding
*****************************************************************************/

#ifndef ES_Framework_H
#define ES_Framework_H

#include <inttypes.h>
#include "ES_Events.h"
#include "ES_Timers.h"
#include "ES_CheckEvents.h"
#include "ES_General.h"
#include "ES_KeyboardInput.h"
#include "ES_LookupTables.h"
#include "ES_Port.h"
#include "ES_PostList.h"
#include "ES_PriorTables.h"
#include "ES_Queue.h"
#include "ES_ServiceHeaders.h"
#include "ES_TattleTale.h"
#include "ES_Timers.h"


#define ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))

typedef enum {
              Success = 0,
              FailedPost = 1,
              FailedRun,
              FailedPointer,
              FailedIndex,
              FailedInit
} ES_Return_t;

ES_Return_t ES_Initialize( void );


ES_Return_t ES_Run( void );
uint8_t ES_PostAll( ES_Event ThisEvent );
uint8_t ES_PostToService( uint8_t WhichService, ES_Event ThisEvent);



#endif   // ES_Framework_H
