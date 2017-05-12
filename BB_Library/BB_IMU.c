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
#include "BB_QUAT.h"

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

typedef struct Calibration_IMU {
    int ACC_Cal;
    int GYR_Cal;
    int MAG_Cal;
    int SYS_Cal;
} Calibration_IMU;

typedef struct IMU_Data {
    gyroAxis gyro;
    eulerAxis euler;
    int temp;
    Quat quaternion;
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
    dat = CONFIGMODE_CON;
    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_OPR_MODE, &dat)) {
        printf("Error: in Write to OPR MODE \n");
    }
    DelayMs(100);
    // Reset registers
    dat = 0x20;
    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_SYS_TRIGGER, &dat)) {
        printf("Error: in reset \n");
    }

    DelayMs(100);

    printf(" OPR MODE \n");

    // Select BNO055 sensor units (temperature in degrees F, rate in rps, accel in m/s^2)
    if (IN_RADIANS) {
        dat = 0x16; // radians
    } else {
        dat = 0x10; // degrees
    }
    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_UNIT_SEL, &dat)) {
        printf("Error: write sensor units\n");
    }
    printf(" UNIT SEL \n");

    // Select BNO055 gyro temperature source
    dat = 0x01;
    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_TEMP_SOURCE, &dat)) {
        printf("Error: write temp source \n");
    }
    printf(" TEMP SOURCE \n");
    //    
    // Select BNO055 system power mode
    dat = 0x00;
    while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_PWR_MODE, &dat)) {
        printf("Error: write power mode \n");
    }
    printf(" PWR MODE \n");

    // Write Configuration to BNO55 Registers
    // EXIT Config mode and switch to selected Operation mode
    for (i = 0; i <= 5; i++) { // S 21
        //       dat = AMG_CON;
        dat = NDOF_CON; // changed form NDOF_CON
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

            DelayMs(100);
            // Set to use external crystal
            dat = 0x80;
            while (!BB_I2C_Write(BNO55_I2C_ADDR, BNO055_SYS_TRIGGER, &dat)) {
                printf("Error: write external clock \n");
            }
            // stay in loop till calibrated
            UINT8 oldSysCal = -1;
            UINT8 oldGyroCal = -1;
            UINT8 oldAccCal = -1;
            UINT8 oldMagCal = -1;
            do {
                IMU_Read_Calibration(); // must be called before IMU_Get_Sys_Cal())
                UINT8 sysCal = IMU_Get_Sys_Cal();
                UINT8 gyroCal = IMU_Get_Gyro_Cal();
                UINT8 accCal = IMU_Get_Acc_Cal();
                UINT8 magCal = IMU_Get_Mag_Cal();
                if (oldSysCal != sysCal || oldGyroCal != gyroCal || oldAccCal != accCal || oldMagCal != magCal) {
                    printf("Sys_Cal :  %d Gyro_Cal:  %d Acc_Cal :  %d Mag_Cal :  %d \n", sysCal, gyroCal, accCal, magCal);
                }
                oldSysCal = sysCal;
                oldGyroCal = gyroCal;
                oldAccCal = accCal;
                oldMagCal = magCal;

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
    BYTE eulerData[MEASURE_LENGTH] = {2, 2, 2, 2, 2, 2};
    UINT8 i;
    UINT8 dataLocation = BNO055_EUL_HEADING_LSB;
    const float scale = SCALE_FACTOR;
    while (!BB_I2C_Read_Multi(BNO55_I2C_ADDR, dataLocation, 6, &eulerData[0])) {
        printf("Error: in Read Euler \n");
    }

    // store all euler data in a global struct  
    SHORT tempx = ((((WORD) eulerData[1]) << 8) | ((WORD) eulerData[0]));
    SHORT tempy = ((((WORD) eulerData[3]) << 8) | ((WORD) eulerData[2]));
    SHORT tempz = ((((WORD) eulerData[5]) << 8) | ((WORD) eulerData[4]));

    imuData.euler.yaw = ((float) tempx) / scale;
    imuData.euler.roll = ((float) tempy) / scale;
    imuData.euler.pitch = ((float) tempz) / scale;

    //    printf("E1: %d E0: %d  yaw: %f\n", eulerData[1], eulerData[0], imuData.euler.yaw);
    //    printf("E3: %d E2: %d  roll: %f\n", eulerData[3], eulerData[2], imuData.euler.roll);
    //    printf("E5: %d E4: %d  pitch: %f\n", eulerData[5], eulerData[4], imuData.euler.pitch);

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
BOOL IMU_Read_Gyro()
{
    BYTE GYRData[MEASURE_LENGTH] = {2, 2, 2, 2, 2, 2};
    UINT8 i;
    UINT8 dataLocation = BNO055_GYR_DATA_X_LSB;
    const double scale = SCALE_FACTOR;
    while (!BB_I2C_Read_Multi(BNO55_I2C_ADDR, dataLocation, 6, &GYRData[0])) {
        printf("Error: in Read Gryo \n");
    }

    // store all Gyroscope data in a global struct
    SHORT tempx = ((((WORD) GYRData[1]) << 8) | ((WORD) GYRData[0]));
    SHORT tempy = ((((WORD) GYRData[3]) << 8) | ((WORD) GYRData[2]));
    SHORT tempz = ((((WORD) GYRData[5]) << 8) | ((WORD) GYRData[4]));

    imuData.gyro.x = ((float) tempx) / scale;
    imuData.gyro.y = ((float) tempy) / scale;
    imuData.gyro.z = ((float) tempz) / scale;

    //    printf("E1: %d E0: %d  x: %f\n", GYRData[1], GYRData[0], imuData.gyro.x);
    //    printf("E3: %d E2: %d  y: %f\n", GYRData[3], GYRData[2], imuData.gyro.y);
    //    printf("E5: %d E4: %d  z: %f\n", GYRData[5], GYRData[4], imuData.gyro.z);

    return TRUE; // Add success check
}

/**
 * Function: IMU_Get_Gyro_Roll()
 * @param   
 * @return  x
 * @brief 
 **/
float IMU_Get_Gyro_X()
{
    return imuData.gyro.x;
}

/**
 * Function: IMU_Get_Gyro_Pitch()
 * @param   
 * @return  y
 * @brief 
 **/
float IMU_Get_Gyro_Y()
{
    return imuData.gyro.y;
}

/**
 * Function: IMU_Get_Gyro_Yaw()
 * @param   
 * @return  z
 * @brief 
 **/
float IMU_Get_Gyro_Z()
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
BOOL IMU_Read_Quat()
{
    BYTE quatData[8] = {2, 2, 2, 2, 2, 2, 2, 2}; // registers init to zero - checks to see if read
    int i;
    const double scale = (1.0 / (1 << 14));
    SHORT x, y, z, w;
    x = y = z = w = 0;

    // TODO replace this with multiread
    UINT8 dataLocation = BNO055_QUA_DATA_W_LSB;
    while (!BB_I2C_Read_Multi(BNO55_I2C_ADDR, dataLocation, 8, &quatData[0])) {
        printf("Error: in Write to OPR MODE \n");
    }

    w = (((WORD) quatData[1]) << 8) | ((WORD) quatData[0]);
    x = (((WORD) quatData[3]) << 8) | ((WORD) quatData[2]);
    y = (((WORD) quatData[5]) << 8) | ((WORD) quatData[4]);
    z = (((WORD) quatData[7]) << 8) | ((WORD) quatData[6]);

    // store scaled all euler data in a global struct
    imuData.quaternion.w = w * scale;
    imuData.quaternion.x = x * scale;
    imuData.quaternion.y = y * scale;
    imuData.quaternion.z = z * scale;

    printf("Q1: %d Q0: %d temp w: %d w: %f\n", quatData[1], quatData[0], w, imuData.quaternion.w);
    printf("Q3: %d Q2: %d temp x: %d x: %f\n", quatData[3], quatData[2], x, imuData.quaternion.x);
    printf("Q5: %d Q4: %d temp y: %d y: %f\n", quatData[5], quatData[4], y, imuData.quaternion.y);
    printf("Q5: %d Q4: %d temp z: %d z: %f\n", quatData[7], quatData[6], z, imuData.quaternion.z);
    
    return TRUE; // Add success check
}

/**
 * Function: IMU_Get_Quat()
 * @param   
 * @return  
 * @brief 
 * @precond Must call IMU_Read_Quaternion() before to get most recent data
 **/
void IMU_Get_Quat(Quat *q)
{
    q->w = imuData.quaternion.w;
    q->x = imuData.quaternion.x;
    q->y = imuData.quaternion.y;
    q->z = imuData.quaternion.z;
}

/**
 * Function: IMU_Get_Quat_Scalar()
 * @param   
 * @return  x
 * @brief 
 **/
float IMU_Get_Quat_W()
{
    return imuData.quaternion.w;
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

