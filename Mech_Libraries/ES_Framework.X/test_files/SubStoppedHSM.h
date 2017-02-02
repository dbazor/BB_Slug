/****************************************************************************
 
  Header file for template Hierarchical Sate Machine
  based on the Gen2 Events and Services Framework

 ****************************************************************************/

#ifndef SUB_STOPPED_HSM_H
#define SUB_STOPPED_HSM_H

// Event Definitions
#include "ES_Configure.h" /* gets us event definitions */

#define FORWARD_BUMPSWITCHES 0x0005 // just a placeholder for forward switches
#define REARWARD_BUMPSWITCHES 0x00A // just a placeholder for rearward swtiches

// Public Function Prototypes
uint8_t InitSubStoppedHSM(void);
ES_Event RunSubStoppedHSM(ES_Event ThisEvent);

#endif /* SUB_STOPPED_HSM_H */

