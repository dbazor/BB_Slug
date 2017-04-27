/*
 * File:   Motor_Test_Main.c
 * Author: Pavlo Vlastos
 * Based on Talking_Test.c by Daniel Bazor
 * Created on February 18, 2017, 1:17 PM
 */
/************************************************************************/
/*  File Description:                                                   */
/************************************************************************/
#include "BB_BOARD.h"
#include <plib.h>
#include <stdio.h>
#include "BB_Motor.h"
#include "BB_Encoder.h"
#include "BB_LEDS.h"
#include <peripheral/i2c.h>
#include "BB_I2C.h"

// Additional Includes (April 7, 2017)
#include "BNO55_Register_Map.h"
#include "BNO55_I2C_driver.h"
#include "BB_IMU.h"
// 

/* ------------------------------------------------------------ */
/*				Definitions										*/
/* ------------------------------------------------------------ */
#define cntMsDelay 10000
//#define Nop() asm( "nop" )                 //No-operation; asm stands for assembly, using an assembly command in C.  Cool!


// KEEP IN MIND THAT THE ADDRESS BYTE IS REALLY 7 BITS, THE ADDRESS IS SHIFTED TO THE LEFT BY 1 BIT
// FOR READ/WRITE
#define SLAVE_ADDR 0x28

/* ------------------------------------------------------------ */
/*				Prototypes										*/
/* ------------------------------------------------------------ */
// void DeviceInit();
//void DelayInit();
//void DelayMs(int cms);
//function declaration from Northwestern
int RcvData(unsigned int address);
void SendData(int, unsigned int);
void Delayms(unsigned t);
//extern interrupt void InputCapture2(void);


/* ------------------------------------------------------------ */
/*				Private Variables								*/

/* ------------------------------------------------------------ */


//typedef enum i2c_states {
//    START,
//    WRITE,
//    READ,
//    ACK,
//    RESTART,
//    NACK,
//    STOP,
//    IDLE
//} i2c_states;

/* ------------------------------------------------------------ */
/*				Main											*/

/* ------------------------------------------------------------ */
int main() {

    BB_BOARD_Init();
    printf(" before init \n");
    IMU_Init();
    printf("After init \n");
    //    UINT8 dataLocation = BNO055_GYR_OFFSET_Z_MSB;
    //    UINT8 sndData = 1; // clear each loop to be sure it is new
    //    UINT8 rcvData = 0xFA; // clear each loop to be sure it is new
    //
    //    while (!BB_I2C_Read(BNO55_I2C_ADDR, dataLocation, &rcvData)) {
    //        printf("Error: in Write to OPR MODE \n");
    //    }
    //    printf("rcvData: %x\n sndData: %x\n", rcvData, sndData);
    //    rcvData = 0xFD;
    //
    //    while (!BB_I2C_Write(BNO55_I2C_ADDR, dataLocation, &sndData)) {
    //        printf("Error: in Write to OPR MODE \n");
    //    }
    //    //Delayms(50);
    //    while (!BB_I2C_Read(BNO55_I2C_ADDR, dataLocation, &rcvData)) {
    //        printf("Error: in Write to OPR MODE \n");
    //    }
    //    printf("rcvData: %x\n End of loop\n\n", rcvData);
    //    Delayms(2000);
    //    dataLocation = BNO055_EUL_HEADING_LSB;

    IMU_Data ReturnData;
    UINT8 data;
    IMU_Data TestStruct;
    Calibration_IMU CaiStat;
    while (1) {

        // sanity check    

        //        BB_I2C_Read(BNO55_I2C_ADDR, BNO055_EUL_PITCH_LSB, &data);
        //        printf(" EUL PITCH LSB = %d \n", data);
        //        BB_I2C_Read(BNO55_I2C_ADDR, BNO055_EUL_PITCH_MSB, &data);
        //        printf(" EUL PITCH MSB = %d \n", data);
        CaiStat = IMU_Get_Calibration();
        printf(" The Calibratin Status of the IMU: %d \n", CaiStat.SYS_Cal);
        printf(" Before the function call \n");
        TestStruct = IMU_Get_GYR_Angles();
        printf("\n\n");
        printf("heading = %f \n", TestStruct.GYR.Heading);
        printf("roll    = %f \n", TestStruct.GYR.Roll);
        printf("pitch   = %f \n", TestStruct.GYR.Pitch);

        //        IMU_Read_Euler_Angles(ReturnData);
        //        float roll_float = (float) ReturnData.euler.Roll;
        //        float pitch_float = (float) ReturnData.euler.Pitch;
        //        float heading_float = (float) ReturnData.euler.Heading;
        //

        Delayms(500);

        //UINT8 rcvData1[6] = {2, 2, 2, 2, 2, 2}; // clear each loop to be sure it is new
        //        while (!BB_I2C_Read_Multi(SLAVE_ADDR, dataLocation, 2, &rcvData1[0])) {
        //            printf("Error: in Write to OPR MODE \n");
        //        }
        //        printf(" after read multi \n");
        //        int i;
        //        for (i = 0; i < 6; i++) {
        //            printf("euler angle %d : %x\n", i, rcvData1[i]);
        //            rcvData1[i] = i;
        //
        //        }
        //
        //        printf(" after read multi \n");
        //        int i;
        //        for (i = 0; i < 6; i++) {
        //            while (!BB_I2C_Read(SLAVE_ADDR, dataLocation++, &rcvData1[i])) {
        //                printf("Error: in Write to OPR MODE \n");
        //            }
        //            printf("euler angle %d : %x\n", i, rcvData1[i]);
        //            //rcvData1[i] = i;
        //
        //        }
        //        printf("End of loop\n\n");
        //        while (!BB_I2C_Read(BNO55_I2C_ADDR, dataLocation, &rcvData)) {
        //            printf("Error: in Write to OPR MODE \n");
        //        }
        //        printf("rcvData: %x\n End of loop\n\n", rcvData);
        //        dataLocation++;
        //        Delayms(3000);
        ////        
        ////
        //        while (!BB_I2C_Read(BNO55_I2C_ADDR, dataLocation, &rcvData)) {
        //            printf("Error: in Write to OPR MODE \n");
        //        }
        //        printf("rcvData: %x\n sndData: %x\n", rcvData, sndData);
        //        rcvData = 0xFD;
        //
        //        while (!BB_I2C_Write(BNO55_I2C_ADDR, dataLocation, &sndData)) {
        //            printf("Error: in Write to OPR MODE \n");
        //        }
        //        //Delayms(50);
        //        while (!BB_I2C_Read(BNO55_I2C_ADDR, dataLocation, &rcvData)) {
        //            printf("Error: in Write to OPR MODE \n");
        //        }
        //        printf("rcvData: %x\n End of loop\n\n", rcvData);

        //sndData++;
    }

    return 0;
}

// BB_I2C_Read_Multi(SLAVE_ADDR, dataLocation, 6, &rcvData[0]);
//        printf(" after read multi \n");
//        int i;
//        for (i = 0; i < 6; i++) {
//            printf("euler angle %d : %x\n", i, rcvData[i]);
//
//        }

//int main()
//{
//    BB_BOARD_Init();
//
//    int rcv = 0xFF; //For received data
//    int dataLocation = 0; //  UNIT_SEL; // do this later
//    unsigned char rcvData = 0xF0;
//    unsigned char sndData = 0x01;
//    unsigned char dat;
//
//    //    dataLocation = BNO055_ACC_DATA_X_LSB; // this is a read only value!
//    dataLocation = BNO055_CHIP_ID;
//    printf("Starting\n");
//
//    BB_I2C_Read(SLAVE_ADDR, dataLocation, &rcvData);
//    printf("0 Chip id: %x\n", rcvData);
//    Delayms(1000);
//    //    RcvData(dataLocation);
//    //    printf("0 Chip id: %d\n", rcvData);
//    //    Delayms(1000);
//    dataLocation = BNO055_GYR_OFFSET_Z_MSB;
//    while (1) {
//        //         Read test
//        UINT8 addr = 0;
//        for (addr = 0; addr < 6; addr++) {
//            BB_I2C_Read(SLAVE_ADDR, addr, &rcvData);
//            printf("0 Chip id: %x\n", rcvData);
//            Delayms(1000);
//        }
//        // TEST: 
//        // 1) Read register 
//        // 2) Attempt to write to register
//        // 3) Read register again
//        // 4) Attempt to write to register again
//        // 5) Read register for the last time
//
//        // Select BNO055 config mode
//
//        BB_I2C_Read(SLAVE_ADDR, dataLocation, &rcvData);
//        printf("1 Received byte: %x\n", rcvData);
//        Delayms(1000);
//
//        sndData = 0x01;
//        BB_I2C_Write(SLAVE_ADDR, dataLocation, &sndData);
//        printf("2 Transmitted byte: %x\n", sndData);
//        Delayms(1000);
//
//        BB_I2C_Read(SLAVE_ADDR, dataLocation++, &rcvData);
//        printf("3 Received byte: %x\n", rcvData);
//        Delayms(1000);
//        //
//        //        sndData = 0x23;
//        //        BB_I2C_Write(SLAVE_ADDR, dataLocation, &sndData);
//        //        printf("4 Transmitted byte: %x\n", sndData);
//        //        Delayms(1000);
//        //
//        //        BB_I2C_Read(SLAVE_ADDR, dataLocation, &rcvData);
//        //        printf("5 Received byte: %x\n", rcvData);
//        //        Delayms(1000);
//
//        //        MPU_I2C_Write(BNO55_I2C_ADDR, OPR_MODE, &dat);
//        //        Delayms(50);
//        //        MPU_I2C_Read(BNO55_I2C_ADDR, OPR_MODE, dat);
//        //        if (dat == NDOF_CON) {
//        //            printf("Configured\n");
//        //        }
//
//        printf("end of loop \n \n");
//        Delayms(5000);
//        //        sndData++;
//    }
//
//
//    return 0;
//}

void Delayms(unsigned t)
// This uses Timer 1, can be changed to another timer. Assumes FPB = SYS_FREQ
{
    OpenTimer1(T1_ON | T1_PS_1_256, 0xFFFF);
    while (t--) { // t x 1ms loop
        WriteTimer1(0);
        while (ReadTimer1() < SYS_FREQ / 256 / 1000);
    }
    CloseTimer1();
} // Delayms

/***************************************************
 * SendData(int data, unsigned int address)        *
 *						    *
 * Sends a byte of data (DATA) over the I2C line   *
 *	to I2C address ADDRESS			    *
 *						    *
 * Returns: nothing				    *
 ***************************************************/
void SendData(int data, unsigned int address) {
    StartI2C1(); //Send the Start Bit
    IdleI2C1(); //Wait to complete

    MasterWriteI2C1((address << 1) | 0); //Sends the slave address over the I2C line. This must happen first so the 
    //proper slave is selected to receive data.
    IdleI2C1(); //Wait to complete

    MasterWriteI2C1(data); //Sends data byte over I2C line
    IdleI2C1(); //Wait to complete

    StopI2C1(); //Send the Stop condition
    IdleI2C1(); //Wait to complete

} //end function


///* ------------------------------------------------------------ */
//
///***	DelayInit
// **
// **	Parameters:
// **		none
// **
// **	Return Value:
// **		none
// **
// **	Errors:
// **		none
// **
// **	Description:
// **		Initialized the hardware for use by delay functions. This
// **		initializes Timer 1 to count at 10Mhz.
///* ------------------------------------------------------------ */
//
//void DelayInit()
//{
//    unsigned int tcfg;
//
//    /* Configure Timer 1. This sets it up to count a 10Mhz with a period of 0xFFFF
//     */
//    tcfg = T1_ON | T1_IDLE_CON | T1_SOURCE_INT | T1_PS_1_8 | T1_GATE_OFF | T1_SYNC_EXT_OFF;
//    OpenTimer1(tcfg, 0xFFFF);
//
//}
//
///* ------------------------------------------------------------ */
//
///***	DelayMs
// **
// **	Parameters:
// **		cms			- number of milliseconds to delay
// **
// **	Return Value:
// **		none
// **
// **	Errors:
// **		none
// **
// **	Description:
// **		Delay the requested number of milliseconds. Uses Timer1.
///* ------------------------------------------------------------ */
//
//void DelayMs(int cms)
//{
//    int ims;
//
//    for (ims = 0; ims < cms; ims++) {
//        WriteTimer1(0);
//        while (ReadTimer1() < cntMsDelay);
//    }
//}

