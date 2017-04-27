#ifndef BB_IMU_H
#define BB_IMU_H

#include "BB_BOARD.h"
#include "BNO55_I2C_driver.h"
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
 

/******************************************************************************/ 


/*******************************************************************************
 * PUBLIC FUNCTION Typdefs                                                 *
 ******************************************************************************/ 
typedef struct {
  float Heading;
  float Roll;
  float Pitch;
} eAxis;


typedef struct {
   eAxis GYR;
   eAxis euler;
   short temp;            
} IMU_Data;

typedef struct {
 int x;
 int y;
 int z;
} CoordAxis;


typedef struct {
    int ACC_Cal;
    int GYR_Cal;
    int MAG_Cal;
    int SYS_Cal;
} Calibration_IMU;



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
BOOL IMU_Init();
Calibration_IMU IMU_Get_Calibration();
BOOL IMU_Set_Calibration();
IMU_Data IMU_Get_Euler_Angles();
IMU_Data IMU_Get_GYR_Angles();
int SystemCalibration();


#endif