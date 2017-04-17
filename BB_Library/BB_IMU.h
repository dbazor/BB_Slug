#ifndef BB_IMU_H
#define BB_IMU_H

#include "BB_BOARD.h"
#include "BNO55_I2C_driver.h"
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
 
/******************************************************************************/
BOOL IMU_Init(); 
BOOL IMU_Read_Euler_Angles(); 
BOOL IMU_Get_Calibration(); 
BOOL IMU_Set_Calibration();


/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/


#endif