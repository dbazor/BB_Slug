#ifndef BB_MOTOR_H
#define	BB_MOTOR_H

/*******************************************************************************
 * PRIVATE #INCLUDES                                                            *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC #DEFINES                                                            *
 ******************************************************************************/
#define FORWARD 1
#define REVERSE 0

#define MOTOR_1 1 
#define MOTOR_2 2 
#define MOTOR_3 3

#define MAX_PWM 1000 
#define MIN_PWM (-MAX_PWM)
#define MAX_VOLTAGE 12
// add a min voltage
#define MOTOR_1_DIR IOPORT_E, BIT_1
#define MOTOR_2_DIR IOPORT_E, BIT_2
#define MOTOR_3_DIR IOPORT_E, BIT_3

// For matrix inverseA
#define MOTOR_BRACKET_LENGTH (0.18) //needs to be the motor bracket length divded by three
#define ONE_THIRD   (1.0/3.0)
#define TWO_THIRDS  (2.0/3.0)
#define ROOT3OVER3  (0.577350269)
#define HEIGHT      3
#define WIDTH       3

#define PWM_2_RAD_PER_SEC  63.66197724  // this is assuming a max speed of 150 RPM which is high
#define RAD_PER_SEC_2_PWM  (1/PWM_2_RAD_PER_SEC)
#define MAX_RAD_PER_SEC (MAX_PWM/PWM_2_RAD_PER_SEC)

typedef struct MotorSpeedsCmd { // holds commanded motorSpeeds in rad/sec
    double m1;
    double m2;
    double m3;
} MotorSpeedsCmd;

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
void MotorSetSpeed(int PWM, int motorNum);

/**
 * Function: GetMotorSpeed()
 * @param   motorNum    - MOTOR_x, where x is 1, 2, 3
 * @return  
 * @brief   
 * @notes   
 **/
int MotorGetSpeed(int motorNum);

/**
 * Function: GetMotorDirection()
 * @param   motorNum    - MOTOR_x, where x is 1, 2, 3
 * @return  motor direction
 * @brief   
 * @notes   
 **/
int MotorGetDirection(int motorNum);

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
void MotorSet_XYZ(double x, double y, double z);

/**
 * Function: MotorsStop()
 * @param
 * @return  
 * @brief   
 * @notes   
 **/
void MotorGetCommand(MotorSpeedsCmd *m);
#endif	/* BB_MOTOR_H */