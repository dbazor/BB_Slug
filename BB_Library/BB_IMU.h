#ifndef BB_IMU_H
#define BB_IMU_H

#include "BB_BOARD.h"
#include "BNO55_I2C_driver.h"
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
 
/******************************************************************************/
/**
 * Function: IMU_Init()
 * @param None
 * @return None
 * @brief 
 **/
BOOL IMU_Init(); 

/**
 * Function: IMU_Read_Euler_Angles()
 * @param None
 * @return TRUE    - If successful
 *         FALSE   - Otherwise
 * @brief 
 **/
BOOL IMU_Read_Euler_Angles();

/**
 * Function: IMU_Get_Calibration()
 * @param None
 * @return TRUE    - If successful
 *         FALSE   - Otherwise
 * @brief 
 **/
BOOL IMU_Get_Calibration(); 

/**
 * Function: IMU_Set_Calibration()
 * @param None
 * @return TRUE    - If successful
 *         FALSE   - Otherwise
 * @brief 
 **/
BOOL IMU_Set_Calibration();

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/


#endif