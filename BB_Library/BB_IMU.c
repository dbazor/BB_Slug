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
typedef struct eulerAxis {
    float roll;
    float pitch;
    float yaw; // yaw, but z points up
} eulerAxis;

typedef struct gyroAxis {
    float x;
    float y;
    float z;
} gyroAxis;

typedef struct quatAxis {
    float scalar;
    float x;
    float y;
    float z;
} quatAxis;

typedef struct Calibration_IMU {
    int ACC_Cal;
    int GYR_Cal;
    int MAG_Cal;
    int SYS_Cal;
} Calibration_IMU;

typedef struct IMU_Data {
    gyroAxis gyro;
    eulerAxis euler;
    short temp;
    quatAxis quaternion;
    Calibration_IMU Calibration_Data;
} IMU_Data;

static IMU_Data imuData;

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

    // Select BNO055 sensor units (temperature in degrees F, rate in rps, accel in m/s^2)
    if (IN_RADIANS) {
        dat = 0x16; // radians
    } else {
        dat = 0x10; // degrees
    }
    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_UNIT_SEL, &dat)) {
        printf("Error: in Write to OPR MODE \n");
    }
    printf(" UNIT SEL \n");

    // Select BNO055 gyro temperature source
    dat = 0x01;
    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_TEMP_SOURCE, &dat)) {
        printf("Error: in Write to OPR MODE \n");
    }
    printf(" TEMP SOURCE \n");
    //    
    // Select BNO055 system power mode
    dat = 0x00;
    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_PWR_MODE, &dat)) {
        printf("Error: in Write to OPR MODE \n");
    }
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
        }
        UINT8 OldsysCal=-1;
        do {
                IMU_Read_Calibration(); // must be called before IMU_Get_Sys_Cal())
                UINT8 sysCal = IMU_Get_Sys_Cal();  
                
                if (OldsysCal != sysCal){
                    printf("sysCal: %d\n", sysCal);
                }
                OldsysCal = sysCal;
                switch (sysCal) {
                case 0:
                    Turn_Off_All_LED();
                    Turn_On_LED(BB_LED_1);
                    
                    break;
                case 1:
                    Turn_Off_All_LED();
                    Turn_On_LED(BB_LED_1);
                    Turn_On_LED(BB_LED_2);
                    break;
                case 2:
                    Turn_Off_All_LED();
                    Turn_On_LED(BB_LED_1);
                    Turn_On_LED(BB_LED_2);
                    Turn_On_LED(BB_LED_3);
                    break;
                case 3:
                    Turn_Off_All_LED();
                    Turn_On_LED(BB_LED_1);
                    Turn_On_LED(BB_LED_2);
                    Turn_On_LED(BB_LED_3);
                    Turn_On_LED(BB_LED_4);
                    break;
                }
            } while (IMU_Get_Sys_Cal() < 3);
        return TRUE;
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
BOOL IMU_Read_Euler_Angles()
{
    int eulerData[MEASURE_LENGTH] = {2, 2, 2, 2, 2, 2};
    int i;
    UINT8 dataLocation = BNO055_EUL_HEADING_LSB;
    for (i = 0; i < MEASURE_LENGTH; i++) {
        while (!BB_I2C_Read(BNO55_I2C_ADDR, dataLocation++, &eulerData[i])) {
            printf("Error: in Write to OPR MODE \n");
        }
    }
    printf("%x %x %x\n",eulerData[1], eulerData[0],((eulerData[1] << 8) | eulerData[0]));

    if (IN_RADIANS) {
        // store all euler data in a global struct

        imuData.euler.yaw   = ((float)((eulerData[1] << 8) | eulerData[0]) / RADIANS_FACTOR);
        imuData.euler.roll  = ((float)((eulerData[3] << 8) | eulerData[2]) / RADIANS_FACTOR);
        imuData.euler.pitch = ((float)((eulerData[5] << 8) | eulerData[4]) / RADIANS_FACTOR);
    } else {
        imuData.euler.yaw   = ((float)((eulerData[1] << 8) | eulerData[0]) / DEGREE_FACTOR);
        imuData.euler.roll  = ((float)((eulerData[3] << 8) | eulerData[2]) / DEGREE_FACTOR);
        imuData.euler.pitch = ((float)((eulerData[5] << 8) | eulerData[4]) / DEGREE_FACTOR);
    }
    return TRUE; // Add success check
}

/**
 * Function: IMU_Get_Euler_Roll()
 * @param   
 * @return  Roll
 * @brief 
 **/
float IMU_Get_Euler_Roll()
{
    return imuData.euler.roll;
}

/**
 * Function: IMU_Get_Euler_Pitch()
 * @param   
 * @return  Pitch
 * @brief 
 **/
float IMU_Get_Euler_Pitch()
{
    return imuData.euler.pitch;
}

/**
 * Function: IMU_Get_Euler_Yaw()
 * @param   
 * @return  Yaw
 * @brief 
 **/
float IMU_Get_Euler_Yaw()
{
    return imuData.euler.yaw;
}

/**
 * Function: IMU_Read_Gyro_Angles()
 * @param   None
 * @return  TRUE    - If successful
 *          FALSE   - Otherwise
 * @brief Updates the struct
 **/
BOOL IMU_Read_Gyro_Angles()
{
    UINT8 GYRData[MEASURE_LENGTH] = {2, 2, 2, 2, 2, 2};
    int i;
    UINT8 dataLocation = BNO055_GYR_DATA_X_LSB;
    for (i = 0; i < MEASURE_LENGTH; i++) {
        while (!BB_I2C_Read(BNO55_I2C_ADDR, dataLocation++, &GYRData[i])) {
            printf("Error: in Write to OPR MODE \n");
        }
    }

    if (IN_RADIANS) {
        // store all Gyroscope data in a global struct
        imuData.gyro.x = (float) (((GYRData[1] << 8) | GYRData[0]) / RADIANS_FACTOR);
        imuData.gyro.y = (float) (((GYRData[3] << 8) | GYRData[2]) / RADIANS_FACTOR);
        imuData.gyro.z = (float) (((GYRData[5] << 8) | GYRData[4]) / RADIANS_FACTOR);
    } else {
        imuData.gyro.x = (float) (((GYRData[1] << 8) | GYRData[0]) / DEGREE_FACTOR);
        imuData.gyro.y = (float) (((GYRData[3] << 8) | GYRData[2]) / DEGREE_FACTOR);
        imuData.gyro.z = (float) (((GYRData[5] << 8) | GYRData[4]) / DEGREE_FACTOR);
    }
    return TRUE; // Add success check
}

/**
 * Function: IMU_Get_Gyro_Roll()
 * @param   
 * @return  x
 * @brief 
 **/
float IMU_Get_Gyro_Roll()
{
    return imuData.gyro.x;
}

/**
 * Function: IMU_Get_Gyro_Pitch()
 * @param   
 * @return  y
 * @brief 
 **/
float IMU_Get_Gyro_Pitch()
{
    return imuData.gyro.y;
}

/**
 * Function: IMU_Get_Gyro_Yaw()
 * @param   
 * @return  z
 * @brief 
 **/
float IMU_Get_Gyro_Yaw()
{
    return imuData.gyro.z;
}

/**
 * Function: IMU_Read_Quaternion()
 * @param None
 * @return TRUE    - If successful
 *         FALSE   - Otherwise
 * @brief 
 **/
BOOL IMU_Read_Quaternion()
{
    UINT8 quatData[8] = {2, 2, 2, 2, 2, 2, 2, 2}; // registers init to zero - checks to see if read
    int i;
    UINT8 dataLocation = BNO055_QUA_DATA_W_LSB;
    for (i = 0; i < 8; i++) {
        while (!BB_I2C_Read(BNO55_I2C_ADDR, dataLocation++, &quatData[i])) {
            printf("Error: in Write to OPR MODE \n");
        }
    }
    // store all euler data in a global struct

    imuData.quaternion.scalar = (float) (((quatData[1] << 8) | quatData[0]) / QUAT_SCAL_FACTOR);
    imuData.quaternion.x = (float) (((quatData[3] << 8) | quatData[2]) / QUAT_SCAL_FACTOR);
    imuData.quaternion.y = (float) (((quatData[5] << 8) | quatData[4]) / QUAT_SCAL_FACTOR);
    imuData.quaternion.z = (float) (((quatData[7] << 8) | quatData[6]) / QUAT_SCAL_FACTOR);

    return TRUE; // Add success check
}

/**
 * Function: IMU_Get_Quat_Scalar()
 * @param   
 * @return  x
 * @brief 
 **/
float IMU_Get_Quat_Scalar()
{
    return imuData.quaternion.scalar;
}

/**
 * Function: IMU_Get_Gyro_Roll()
 * @param   
 * @return  x
 * @brief 
 **/
float IMU_Get_Quat_X()
{
    return imuData.quaternion.x;
}

/**
 * Function: IMU_Get_Gyro_Y()
 * @param   
 * @return  y
 * @brief 
 **/
float IMU_Get_Quat_Y()
{
    return imuData.quaternion.y;
}

/**
 * Function: IMU_Get_Gyro_Z()
 * @param   
 * @return  z
 * @brief 
 **/
float IMU_Get_Quat_Z()
{
    return imuData.quaternion.z;
}

/**
 * Function: IMU_Read_Calibration()
 * @param   
 * @return  TRUE    - obtained offset data
 *          FALSE  - failed to get offset data
 * @brief 
 **/
BOOL IMU_Read_Calibration()
{
    UINT8 calibrationData;
    // fills private variable with all the calibration status data
    while (!BB_I2C_Read(BNO55_I2C_ADDR, BNO055_CALIB_STAT, &calibrationData)) {
        printf(" failed to get offset data \n");
    }

    // bit shifting to get the four different calibration data
    imuData.Calibration_Data.SYS_Cal = (calibrationData >> 6)&(0b11);
    imuData.Calibration_Data.GYR_Cal = (calibrationData >> 4)&(0b11);
    imuData.Calibration_Data.ACC_Cal = (calibrationData >> 2)&(0b11);
    imuData.Calibration_Data.MAG_Cal = (calibrationData & 0b11);

    return TRUE;

}

/**
 * Function: IMU_Get_Sys_Cal()
 * @param   
 * @return  
 * @brief 
 **/
UINT8 IMU_Get_Sys_Cal()
{
    return imuData.Calibration_Data.SYS_Cal;
}

/**
 * Function: IMU_Get_Gyro_Cal()
 * @param   
 * @return  
 * @brief 
 **/
UINT8 IMU_Get_Gyro_Cal()
{
    return imuData.Calibration_Data.GYR_Cal;
}

/**
 * Function: IMU_Get_Acc_Cal()
 * @param   
 * @return  
 * @brief 
 **/
UINT8 IMU_Get_Acc_Cal()
{
    return imuData.Calibration_Data.ACC_Cal;
}

/**
 * Function: IMU_Get_Mag_Cal()
 * @param   
 * @return  
 * @brief 
 **/
UINT8 IMU_Get_Mag_Cal()
{
    return imuData.Calibration_Data.MAG_Cal;
}
/*******************************************************************************
 * Private Support Functions                                                   *
 ******************************************************************************/

