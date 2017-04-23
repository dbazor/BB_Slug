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
  signed int Heading;
  signed int Roll;
  signed int Pitch;
} eAxis;


typedef struct {
   eAxis euler;
   short temp;            
} IMU_Data;

typedef struct {
 int x;
 int y;
 int z;
} CoordAxis;

typedef struct {
   CoordAxis angle;
} ACC_Calibration;

typedef struct {
   CoordAxis angle;
} MAG_Calibration;

typedef struct {
   CoordAxis angle;
} GYR_Calibration;



/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/
BOOL IMU_Init();
BOOL IMU_Get_Calibration();
BOOL IMU_Set_Calibration();
IMU_Data IMU_Get_Euler_Angles();


#endif