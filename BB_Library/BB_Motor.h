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

#define PERIOD 750 // reduced from 1000 to preserve motor
#define MAX_PWM PERIOD
#define MIN_PWM (-MAX_PWM)
#define MAX_VOLTAGE 12
// add a min voltage

#define MOTOR_1_DIR IOPORT_E, BIT_1
#define MOTOR_2_DIR IOPORT_E, BIT_2
#define MOTOR_3_DIR IOPORT_E, BIT_3

// For matrix inverseA
#define MOTOR_BRACKET_LENGTH (0.18/3) //needs to be the motor bracket length divded by three
#define ONE_THIRD   (1.0/3.0)
#define TWO_THIRDS  (2.0/3.0)
#define ROOT3OVER3  (0.577350269)
#define HEIGHT      3
#define WIDTH       3

/*******************************************************************************
 * PROTOTYEPS                                                   *
 ******************************************************************************/

/**
 * Function: MotorsInit()
 * @param 
 * @return 
 * @brief   Initialize PWM and direction pins for motors
 * @notes   This function is called in BB_BOARD_Init() from BB_Board.c
 **/
void MotorsInit(void);

/**
 * Function: SetMotorSpeed()
 * @param   PWM         - integer within 1000 to -1000
 *          motorNum    - MOTOR_x, where x is 1, 2, 3
 * @return 
 * @brief   
 * @notes   
 **/
void SetMotorSpeed(int PWM, int motorNum);

/**
 * Function: GetMotorSpeed()
 * @param   motorNum    - MOTOR_x, where x is 1, 2, 3
 * @return  
 * @brief   
 * @notes   
 **/
int GetMotorSpeed(int motorNum);

/**
 * Function: GetMotorDirection()
 * @param   motorNum    - MOTOR_x, where x is 1, 2, 3
 * @return  motor direction
 * @brief   
 * @notes   
 **/
int GetMotorDirection(int motorNum);

/**
 * Function: MotorsStop()
 * @param
 * @return  
 * @brief   
 * @notes   
 **/
void MotorsStop(void);

/**
 * Function: SetMotor_XYZ()
 * @param   x, y, z
 * @return  
 * @brief   Virtual wheel translation for individual motor contribution. 
 *          Parameter z is rotation, which we pass a zero to for now.
 *          This function vector multiplies by inv(A) and then sets each motor.
 **/
void SetMotor_XYZ(double x, double y, double z);

#endif	/* BB_MOTOR_H */