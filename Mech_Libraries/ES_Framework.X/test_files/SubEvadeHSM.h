/****************************************************************************
 
  Header file for template Hierarchical Sate Machine
  based on the Gen2 Events and Services Framework

 ****************************************************************************/

#ifndef SUB_EVADE_HSM_H
#define SUB_EVADE_HSM_H

// Event Definitions
#include "ES_Configure.h" /* gets us event definitions */

#define ROACH_EVADE_REVERSE_TIME 2000 // 5 seconds before dancing
#define ROACH_EVADE_LEFT_TIME    1500 // 1 1/2 seconds of left spin

// Public Function Prototypes
uint8_t InitSubEvadeHSM(void);
ES_Event RunSubEvadeHSM(ES_Event ThisEvent);

#endif /* SUB_EVADE_HSM_H */

