#ifndef BB_MOTOR_H
#define	BB_MOTOR_H

// Defines
#define FORWARD 1
#define REVERSE 0

#define MOTOR_1 1 
#define MOTOR_2 2 
#define MOTOR_3 3

#define PERIOD 1000

// Prototypes
void MotorsInit(void);   // called in boardinit
void Motor(int PWM, int direction, int motorNum);
void Motor1(int PWM, int direction);
void Motor2(int PWM, int direction);
void Motor3(int PWM, int direction);
void MotorsStop(void);

// 

#endif	/* BB_MOTOR_H */