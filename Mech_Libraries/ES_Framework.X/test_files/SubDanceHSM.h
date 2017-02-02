/****************************************************************************
 
  Header file for template Hierarchical Sate Machine
  based on the Gen2 Events and Services Framework

 ****************************************************************************/

#ifndef SUB_DANCE_HSM_H
#define SUB_DANCE_HSM_H

// Event Definitions
#include "ES_Configure.h" /* gets us event definitions */

#define ROACH_DANCE_WAIT_TIME 5000 // 5 seconds before dancing
#define ROACH_RIGHT_TIME      1500 // 1 1/2 seconds of right spin
#define ROACH_LEFT_TIME       2500 // 2 1/2 seconds of left spin


// typedefs for the states
// State definitions for use with the query function


// Public Function Prototypes

uint8_t InitSubDanceHSM(void);
ES_Event RunSubDanceHSM(ES_Event ThisEvent);

#endif /* SUB_DANCE_HSM_H */

