/*
 * Project name:
     MPU BOSCH BNO55
 * Copyright:
     Alexander Wolf
 * Revision History:

 * Description:

 * Test configuration:
     MCU:             PIC32 - P32MX795F512L

     Dev.Board:       EasyPIC7

     Oscillator:      80.0000 MHz
     ext. modules:    mpu_imu click board - 
     SW:              mikroC PRO for PIC32

 * NOTES:
 
   - read ACCEL ,Gyro, Temp, Mag in IMU mode
     or
   - read Euler Angels

   connect: 
      RA2 = SCL
      RA3 = SDA
      RE8 = Interrupt
      RC1 = Reset

 */

#include "BNO55_Register_Map.h"
#include "BNO55_I2C_driver.h"


// MPU IMU module connections
sbit INT_IMU at RE8_bit;
sbit INT_IMU_Direction at TRISE0_bit;

sbit RESET_IMU at RC1_bit;
sbit RESET_IMU_Direction at TRISC1_bit;

// End of MPU IMU module connections

bit IMU_interrupt_bit;


// -----------------------------------------------------------------------------

void Init_Ext_Int1(){

  INTCONSET = 0x0006;                 // Setzen Polaritat auf Rising Edge in IntCon Register mit bits 1 & 2 auf 1 --> 0b00000000 00000000 00000000 00000110

  INT1IP0_bit = 1;                   // Set INT2 interrupt
  INT1IP1_bit = 1;                   // Set interrupt priorities
  INT1IP2_bit = 1;                   // Set inrrupt priority to 4
  INT1IE_bit  = 1;                   // Set interrupt on INT2 (RE9) to be enabled
  INT1IF_bit  = 0;

  /*
  111 = Interrupt of group priority 7 or lower start the TP timer
  110 = Interrupt of group priority 6 or lower start the TP timer
  101 = Interrupt of group priority 5 or lower start the TP timer
  100 = Interrupt of group priority 4 or lower start the TP timer
  011 = Interrupt of group priority 3 or lower start the TP timer
  010 = Interrupt of group priority 2 or lower start the
  */

}

void Interrupt_EXTERN_1() iv IVT_EXTERNAL_1 ilevel 7 ics ICS_SRS {
  IMU_interrupt_bit = 1;
  INT1IF_bit = 0;                    // Reset INT2 flag
}


void Init_BNO55(){
  short result;

  RESET_IMU = 1;
  delay_ms(100);
  RESET_IMU = 0;
  delay_ms(10);
  RESET_IMU = 1;
  delay_ms(500);

  result = BNO55_Detect();
  if( result == 1)
      UART2_write_text("BNO55 detected\x0D\x0D");
  else
      UART2_write_text("BNO55 NOT detected\x0D\x0D");

  Config_BNO55();

  // READ & Print Registers to check settings;

  
  
  UART2_write_text("Identification Register:\x0D");
  Print_MPU_register_map(0x00 , 0x00 );

  UART2_write_text("TEMP_SOURCE Register:\x0D");
  Print_MPU_register_map(TEMP_SOURCE , TEMP_SOURCE );
  UART2_write_text("SYS_TRIGGER Register:\x0D");
  Print_MPU_register_map(SYS_TRIGGER , SYS_TRIGGER );
  UART2_write_text("PWR_MODE Register:\x0D");
  Print_MPU_register_map(PWR_MODE , PWR_MODE );
  UART2_write_text("OPR_MODE Register:\x0D");
  Print_MPU_register_map(OPR_MODE , OPR_MODE );
  UART2_write_text("UNIT_SEL Register:\x0D");
  Print_MPU_register_map(UNIT_SEL , UNIT_SEL );
  UART2_write_text("GYR_Config_1 Register:\x0D");
  Print_MPU_register_map(GYR_Config_1 , GYR_Config_1 );
  UART2_write_text("GYR_Config_0 Register:\x0D");
  Print_MPU_register_map(GYR_Config_0 , GYR_Config_0 );
  UART2_write_text("MAG_Config Register:\x0D");
  Print_MPU_register_map(MAG_Config , MAG_Config );
  UART2_write_text("ACC_Config Register:\x0D");
  Print_MPU_register_map(ACC_Config , ACC_Config );
  UART2_write_text("INT_EN Register:\x0D");
  Print_MPU_register_map(INT_EN , INT_EN );
  UART2_write_text("INT_MSK Register:\x0D");
  Print_MPU_register_map(INT_MSK , INT_MSK );

}



void main() {
  short result;
  
  AD1PCFG = 0xFFFF;                          // Configure AN pins as digital I/O

  INT_IMU_Direction = 1;
  RESET_IMU_Direction = 0;

  // Initialize I2C module
  I2C2_Init(100000);
  Delay_ms(100);


  // Initialize UART module
  UART2_Init(115200);
  Delay_ms(100);
  UART2_write_text("uController started\x0D");
 
  Init_Ext_Int1();
 
  Init_BNO55();


  while(1) {
 
  Print_BNO55_Data();
  delay_ms(500);
 
  if(IMU_interrupt_bit){
  
     IMU_interrupt_bit = 0;
     }

  }
}