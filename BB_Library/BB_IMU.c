/*
 * File:   BB_IMU.c
 * Author: BB Team
 *
 * Created on April 16, 2017, 12:49 PM
 */

#include "BB_IMU.h"
#include <plib.h>
#include "BNO55_Register_Map.h"
#include "BB_BOARD.h"


/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/



/*******************************************************************************
 * PRIVATE variables                                                             *
 ******************************************************************************/
static IMU_Data returnData;
static Calibration_IMU Calibration_Data;
static int CalibrationStat;


/*******************************************************************************
 * FUNCTION DEFINITIONS                                                        *
 ******************************************************************************/

/**
 * Function: IMU_Init()
 * @param   None 
 * @return  TRUE    - If successful
 *          FALSE   - Otherwise
 * @brief Configure necessary IMU registers
 **/
BOOL IMU_Init()
{
    unsigned char dat;
    int i;

    /*
    #Define    ACCGYRO_CON          0b0000000101
    #define    AMG_CON              0b0000000111
    #define    IMU_CON              0b0000001000
    #define    NDOF_CON             0b0000001100
     */
    /*   */

    // Select BNO055 config mode
    dat = 0x00;
    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_OPR_MODE, &dat)) {
        printf("Error: in Write to OPR MODE \n");
    }

    //MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_OPR_MODE, 1, &dat);
    DelayMs(100);
  
    printf(" OPR MODE \n");

    //    // Select page 1 to configure sensors
    //    dat = 0x01;
    //    BB_I2C_Write(BNO55_I2C_ADDR, BNO055_PAGE_ID, &dat);
    //    // MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_PAGE_ID, 1, &dat);
    //    printf(" Page ID \n");
    //    

    // Select page 0 to read sensors
    //    dat = 0x00;
    //    BB_I2C_Write(BNO55_I2C_ADDR, BNO055_PAGE_ID, &dat);
    //    // MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_PAGE_ID, 1, &dat);
    //    printf(" PAGE ID \n"); 

    // Select BNO055 sensor units (temperature in degrees F, rate in rps, accel in m/s^2)
    dat = 0x16; // 16 radians 12 degrees

    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_UNIT_SEL, &dat)) {
        printf("Error: in Write to OPR MODE \n");
    }
    //MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_UNIT_SEL, 1, &dat);
    printf(" UNIT SEL \n");

    // Select BNO055 gyro temperature source
    dat = 0x01;
    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_TEMP_SOURCE, &dat)) {
        printf("Error: in Write to OPR MODE \n");
    }
    //MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_TEMP_SOURCE, 1, &dat);
    printf(" TEMP SOURCE \n");
    //    
    // Select BNO055 system power mode
    dat = 0x00;
    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_PWR_MODE, &dat)) {
        printf("Error: in Write to OPR MODE \n");
    }
    //MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_PWR_MODE, 1, &dat);
    printf(" PWR MODE \n");

    // Write Configuration to BNO55 Registers
    // EXIT Config mode and switch to selected Operation mode
    for (i = 0; i <= 5; i++) { // S 21
        //       dat = AMG_CON;
        dat = NDOF_CON;
        printf(" in loop %d OPR MODE \n", i);
        while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_OPR_MODE, &dat)) {
            printf("Error: in Write to OPR MODE \n");
        }
        DelayMs(100);
        while (!BB_I2C_Read(BNO55_I2C_ADDR, OPR_MODE, &dat)) {
            printf("Error: in Write to OPR MODE \n");
        }
        if (dat == NDOF_CON) {
            printf("Configured\n");

            while (SystemCalibration() <3) {
                printf(" waiting till calibrated \n");
                if (SystemCalibration() ==1){
                    Turn_On_LED(IOPORT_G,BIT_12);
                }
                else if (SystemCalibration()==2){
                        Turn_Off_LED(IOPORT_G,BIT_12);
                        Turn_On_LED(IOPORT_G,BIT_13);
                }
                else if (SystemCalibration() == 3){
                        Turn_Off_LED(IOPORT_G,BIT_12);
                        Turn_Off_LED(IOPORT_G,BIT_13);
                        Turn_On_LED(IOPORT_G,BIT_14);
                    return TRUE;
                }
            }
        }
    }

    printf("Error: Failed to configure\n");
    return FALSE;
}

 /**
 * Function: IMU_Read_Euler_Angles()
 * @param None
 * @return TRUE    - If successful
 *         FALSE   - Otherwise
 * @brief 
 **/
static BOOL IMU_Read_Euler_Angles() {
    UINT8 eulerData[6] = {2, 2, 2, 2, 2, 2};
    int i;
    UINT8 dataLocation = BNO055_EUL_HEADING_LSB;
    for (i = 0; i < 6; i++) {
        if (!BB_I2C_Read(BNO55_I2C_ADDR, dataLocation++, &eulerData[i])) {
            printf("Error: in Write to OPR MODE \n");
            return FALSE;
        }
    }
    // store all euler data in a global struct
    returnData.euler.Heading = (float) ((eulerData[1] << 8) | eulerData[0]) / 16.0;
    returnData.euler.Roll = (float) ((eulerData[3] << 8) | eulerData[2]) / 16.0;
    returnData.euler.Pitch = (float) ((eulerData[5] << 8) | eulerData[4]) / 16.0;

    return TRUE; // Add success check
}

/*******************************************************************************
  Function:
    BOOL IMU_Read_GYR_Angles()

  Summary:
  This Function populates a local strut with the Magnotometer angle coordinates
  Roll, Pitch, and Heading

  Description:


  Precondition:

  Parameters:


  Returns:
    TRUE    - If successful
    FALSE   - Otherwise

  Example:
    <code>

    </code>

  Remarks:
 *****************************************************************************/

static BOOL IMU_Read_GYR_Angles() {
    UINT8 GYRData[6] = {2, 2, 2, 2, 2, 2};
    int i;
    UINT8 dataLocation = BNO055_GYR_DATA_X_LSB;
    for (i = 0; i < 6; i++) {
        if (!BB_I2C_Read(BNO55_I2C_ADDR, dataLocation++, &GYRData[i])) {
            printf("Error: in Write to OPR MODE \n");
            return FALSE;
        }
    }
    // store all Gyroscope data in a global struct
    returnData.GYR.Heading = (float) ((GYRData[1] << 8) | GYRData[0]) / 16.0;
    returnData.GYR.Roll = (float) ((GYRData[3] << 8) | GYRData[2]) / 16.0;
    returnData.GYR.Pitch = (float) ((GYRData[5] << 8) | GYRData[4]) / 16.0;

    return TRUE; // Add success check
}

/*******************************************************************************
  Function:
    BOOL IMU_Get_GYR_Angles()

  Summary:
  This Function returns a struct of type IMU data with the Gyroscope values

  Description:


  Precondition:

  Parameters:


  Returns:
 A struct of tupe IMU data with the Gyroscope angles

  Example:
    <code>

    </code>

  Remarks:
 *****************************************************************************/


IMU_Data IMU_Get_GYR_Angles() {
    if (IMU_Read_GYR_Angles()) {
        return returnData;
    }
    //printf(" could not read GYR data \n");
}

/*******************************************************************************
  Function:
    BOOL IMU_Get_Euler_Angles()

  Summary:
  This Function returns a struct of type IMU data with the euler angle values

  Description:


  Precondition:

  Parameters:


  Returns:
 A struct of tupe IMU data with the euler angles

  Example:
    <code>

    </code>

  Remarks:
 *****************************************************************************/

IMU_Data IMU_Get_Euler_Angles() {
    if (IMU_Read_Euler_Angles()) {
        return returnData;
    }
}

static BOOL IMU_Read_Calibration() {

    UINT8 dataLocation = BNO055_CALIB_STAT;
    // fills private variable with all the calibration status data
    if (!BB_I2C_Read(BNO55_I2C_ADDR, dataLocation, &CalibrationStat)) {
        printf(" failed to get offset data \n");
        return FALSE;
    }
    return TRUE;

}

Calibration_IMU IMU_Get_Calibration() {
    if (IMU_Read_Calibration()) {
        // parse out each calibration status
        Calibration_Data.SYS_Cal = (CalibrationStat >> 6)&(0b11);
        Calibration_Data.GYR_Cal = (CalibrationStat >> 4)&(0b11);
        Calibration_Data.ACC_Cal = (CalibrationStat >> 2)&(0b11);
        Calibration_Data.MAG_Cal = (CalibrationStat & 0b11);
        return (Calibration_Data);
    }
}
int SystemCalibration(){
        if (IMU_Read_Calibration()) {
        return ((CalibrationStat >> 6)&(0b11));
    }
}

BOOL IMU_Set_Calibration();

/*******************************************************************************
 * Private Support Functions                                                   *
 ******************************************************************************/

