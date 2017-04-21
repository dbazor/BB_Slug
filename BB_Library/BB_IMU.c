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
 * FUNCTION DEFINITIONS                                                        *
 ******************************************************************************/

/*******************************************************************************
  Function:
    BOOL IMU_Init()

  Summary:
    

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
    Delayms(100);
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
    dat = 0x16;
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
        Delayms(100);
        while (!BB_I2C_Read(BNO55_I2C_ADDR, OPR_MODE, &dat)) {
            printf("Error: in Write to OPR MODE \n");
        }
        if (dat == NDOF_CON) {
            printf("Configured\n");
            return TRUE;
        }
    }

    printf("Error: Failed to configure\n");
    return FALSE;
}

BOOL IMU_Read_Euler_Angles();

BOOL IMU_Get_Calibration();

BOOL IMU_Set_Calibration();



/*******************************************************************************
 * Private Support Functions                                                   *
 ******************************************************************************/

