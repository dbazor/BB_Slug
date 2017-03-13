#line 1 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/MPU_IMU_PIC32_v001.c"
#line 1 "c:/users/alexander wolf/documents/projekte/227 balacing robot/pic32 bno055/mpu_imu_v001/bno55_register_map.h"
#line 1 "c:/users/alexander wolf/documents/projekte/227 balacing robot/pic32 bno055/mpu_imu_v001/bno55_i2c_driver.h"





short BNO55_Detect();
void Config_BNO55();
void Print_MPU_register_map(int , int );
void Print_MPU_register( unsigned char, unsigned char);
void Print_BNO55_Data();

void MPU_I2C_Write(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat);
void MPU_I2C_Read_Multi(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat);
void MPU_I2C_Read(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat);
#line 38 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/MPU_IMU_PIC32_v001.c"
sbit INT_IMU at RE8_bit;
sbit INT_IMU_Direction at TRISE0_bit;

sbit RESET_IMU at RC1_bit;
sbit RESET_IMU_Direction at TRISC1_bit;



bit IMU_interrupt_bit;




void Init_Ext_Int1(){

 INTCONSET = 0x0006;

 INT1IP0_bit = 1;
 INT1IP1_bit = 1;
 INT1IP2_bit = 1;
 INT1IE_bit = 1;
 INT1IF_bit = 0;
#line 70 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/MPU_IMU_PIC32_v001.c"
}

void Interrupt_EXTERN_1() iv IVT_EXTERNAL_1 ilevel 7 ics ICS_SRS {
 IMU_interrupt_bit = 1;
 INT1IF_bit = 0;
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





 UART2_write_text("Identification Register:\x0D");
 Print_MPU_register_map(0x00 , 0x00 );

 UART2_write_text("TEMP_SOURCE Register:\x0D");
 Print_MPU_register_map( 0x40  ,  0x40  );
 UART2_write_text("SYS_TRIGGER Register:\x0D");
 Print_MPU_register_map( 0x3F  ,  0x3F  );
 UART2_write_text("PWR_MODE Register:\x0D");
 Print_MPU_register_map( 0x3E  ,  0x3E  );
 UART2_write_text("OPR_MODE Register:\x0D");
 Print_MPU_register_map( 0x3D  ,  0x3D  );
 UART2_write_text("UNIT_SEL Register:\x0D");
 Print_MPU_register_map( 0x3B  ,  0x3B  );
 UART2_write_text("GYR_Config_1 Register:\x0D");
 Print_MPU_register_map( 0x0B  ,  0x0B  );
 UART2_write_text("GYR_Config_0 Register:\x0D");
 Print_MPU_register_map( 0x0A  ,  0x0A  );
 UART2_write_text("MAG_Config Register:\x0D");
 Print_MPU_register_map( 0x09  ,  0x09  );
 UART2_write_text("ACC_Config Register:\x0D");
 Print_MPU_register_map( 0x08  ,  0x08  );
 UART2_write_text("INT_EN Register:\x0D");
 Print_MPU_register_map( 0x10  ,  0x10  );
 UART2_write_text("INT_MSK Register:\x0D");
 Print_MPU_register_map( 0x0F  ,  0x0F  );

}



void main() {
 short result;

 AD1PCFG = 0xFFFF;

 INT_IMU_Direction = 1;
 RESET_IMU_Direction = 0;


 I2C2_Init(100000);
 Delay_ms(100);



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
