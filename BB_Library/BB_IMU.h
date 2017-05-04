#ifndef BB_IMU_H
#define BB_IMU_H

#include "BB_BOARD.h"
#include "BNO55_I2C_driver.h"
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#define IN_RADIANS          FALSE // True for radians, False for degrees
#define RADIANS_FACTOR      900.0
#define DEGREE_FACTOR       16.0

#if IN_RADIANS
#define SCALE_FACTOR RADIANS_FACTOR       
#else
#define SCALE_FACTOR DEGREE_FACTOR
#endif

#define MEASURE_LENGTH      6
#define QUAT_SCAL_FACTOR    16384.0 
#define QUAT_LENGTH         8

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/
/**
 * Function: IMU_Init()
 * @param None
 * @return None
 * @brief 
 **/
BOOL IMU_Init();

/**
 * Function: IMU_Read_Euler_Angles()
 * @param   None
 * @return  TRUE    - If successful
 *          FALSE   - Otherwise
 * @brief Updates the struct
 **/
BOOL IMU_Read_Euler_Angles();

/**
 * Function: IMU_Get_Euler_Roll()
 * @param   
 * @return  Roll
 * @brief 
 **/
float IMU_Get_Euler_Roll();

/**
 * Function: IMU_Get_Euler_Pitch()
 * @param   
 * @return  Pitch
 * @brief 
 **/
float IMU_Get_Euler_Pitch();

/**
 * Function: IMU_Get_Euler_Yaw()
 * @param   
 * @return  Yaw
 * @brief 
 **/
float IMU_Get_Euler_Yaw();

/**
 * Function: IMU_Read_Gyro_Angles()
 * @param   None
 * @return  TRUE    - If successful
 *          FALSE   - Otherwise
 * @brief Updates the struct
 **/
BOOL IMU_Read_Gyro_Angles();

/**
 * Function: IMU_Get_Gyro_Roll()
 * @param   
 * @return  Roll
 * @brief 
 **/
float IMU_Get_Gyro_Roll();

/**
 * Function: IMU_Get_Gyro_Pitch()
 * @param   
 * @return  Pitch
 * @brief 
 **/
float IMU_Get_Gyro_Pitch();

/**
 * Function: IMU_Get_Gyro_Yaw()
 * @param   
 * @return  Yaw
 * @brief 
 **/
float IMU_Get_Gyro_Yaw();


/**
 * Function: IMU_Read_Quaternion()
 * @param None
 * @return TRUE    - If successful
 *         FALSE   - Otherwise
 * @brief 
 **/
BOOL IMU_Read_Quaternion();

/**
 * Function: IMU_Get_Quat_Scalar()
 * @param   
 * @return  x
 * @brief 
 **/
float IMU_Get_Quat_W();

/**
 * Function: IMU_Get_Quat_x()
 * @param   
 * @return  x
 * @brief 
 **/
float IMU_Get_Quat_X();

/**
 * Function: IMU_Quaat_Gyro_Y()
 * @param   
 * @return  y
 * @brief 
 **/
float IMU_Get_Quat_Y();

/**
 * Function: IMU_Quat_Gyro_Z()
 * @param   
 * @return  z
 * @brief 
 **/
float IMU_Get_Quat_Z();

/**
 * Function: IMU_Read_Calibration()
 * @param   
 * @return  TRUE    - obtained offset data
 *          FALSE  - failed to get offset data
 * @brief 
 **/
BOOL IMU_Read_Calibration();

/**
 * Function: IMU_Get_Sys_Cal()
 * @param   
 * @return  
 * @brief 
 **/
UINT8 IMU_Get_Sys_Cal();

/**
 * Function: IMU_Get_Gyro_Cal()
 * @param   
 * @return  
 * @brief 
 **/
UINT8 IMU_Get_Gyro_Cal();

/**
 * Function: IMU_Get_Acc_Cal()
 * @param   
 * @return  
 * @brief 
 **/
UINT8 IMU_Get_Acc_Cal();

/**
 * Function: IMU_Get_Mag_Cal()
 * @param   
 * @return  
 * @brief 
 **/
UINT8 IMU_Get_Mag_Cal();
#endif