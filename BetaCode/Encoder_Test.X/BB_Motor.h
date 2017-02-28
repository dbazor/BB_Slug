#ifndef BB_MOTOR_H
#define	BB_MOTOR_H

/*******************************************************************************
 * PRIVATE #INCLUDES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define FORWARD 1
#define REVERSE 0

#define MOTOR_1 1 
#define MOTOR_2 2 
#define MOTOR_3 3

#define PERIOD 1000
#define MAX_PWM PERIOD

// Prototypes
void MotorsInit(void);   // called in boardinit
void SetMotorSpeed(int PWM, int motorNum);
int GetMotorSpeed(int motorNum);
int GetMotorDirection(int motorNum);
void MotorsStop(void);

// 

#endif	/* BB_MOTOR_H */