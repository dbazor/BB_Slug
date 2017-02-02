/****************************************************************************
 
  Header file for template Hierarchical Sate Machine
  based on the Gen2 Events and Services Framework

 ****************************************************************************/

#ifndef HSMFANCYROACH_H
#define HSMFANCYROACH_H

// Event Definitions
#include "ES_Configure.h" /* gets us event definitions */

#define RoachMotorsStop() // this would be where we would set both motors to zero
#define RoachMotorsForward() // this would be where both motors set to forward
#define RoachMotorsBackward() // this would be where both motors set to backward
#define RoachMotorTankRight() // this would be where motors are set opposite
#define RoachMotorTankLeft() // this would be where motors are set opposite




// Public Function Prototypes

uint8_t InitFancyRoachHSM ( uint8_t Priority );
uint8_t PostFancyRoachHSM( ES_Event ThisEvent );
ES_Event RunFancyRoachHSM( ES_Event ThisEvent );


#endif /* HSMFANCYROACH_H */

