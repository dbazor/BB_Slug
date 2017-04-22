#ifndef BB_IMU_H
#define BB_IMU_H

#include "BB_BOARD.h"
#include "BNO55_I2C_driver.h"
/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
 
/******************************************************************************/ 




/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/ 
BOOL IMU_Init(); 
BOOL IMU_Read_Euler_Angles(); 
BOOL IMU_Get_Calibration(); 
BOOL IMU_Set_Calibration(); 
BOOL IMU_Get_Euler_Angles();

/*******************************************************************************
 * PUBLIC FUNCTION Typdefs                                                 *
 ******************************************************************************/ 
typedef struct {
  int Heading;
  int Roll;
  int Pitch;
} eAxis;


typedef struct {
   eAxis euler;
   short temp;
   float gdt;            
} IMU_Data;

volatile IMU_Data returnData; // should make a getter() for this, and move it to the C file.

#endif