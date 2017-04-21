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
#define MIN_PWM (-MAX_PWM)
#define MAX_VOLTAGE 12
// add a min voltage

#define MOTOR_1_DIR IOPORT_E, BIT_1
#define MOTOR_2_DIR IOPORT_E, BIT_2
#define MOTOR_3_DIR IOPORT_E, BIT_3

// Prototypes
void MotorsInit(void); // called in boardinit
void SetMotorSpeed(int PWM, int motorNum);
int GetMotorSpeed(int motorNum);
int GetMotorDirection(int motorNum);
void MotorsStop(void);

// 

#endif	/* BB_MOTOR_H */