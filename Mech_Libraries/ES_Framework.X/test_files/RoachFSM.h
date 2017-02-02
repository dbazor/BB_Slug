/****************************************************************************
 
  Header file for template Flat Sate Machine 
  based on the Gen2 Events and Services Framework

 ****************************************************************************/

#ifndef RoachFSM_H
#define RoachFSM_H

// Event Definitions
#include "ES_Configure.h"
#include <inttypes.h>
// typedefs for the states
// State definitions for use with the query function

typedef enum {
    Init,
    Moving,
    Stopped
} RoachState_t;



// Public Function Prototypes


uint8_t InitRoachFSM(uint8_t Priority);
uint8_t PostRoachFSM(ES_Event ThisEvent);
ES_Event RunRoachFSM(ES_Event ThisEvent);
RoachState_t QueryRoachSM(void);


#endif /* FSMTemplate_H */

