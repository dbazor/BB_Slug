#line 1 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/BNO55_I2C_driver.c"
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
#line 1 "c:/users/alexander wolf/documents/projekte/227 balacing robot/pic32 bno055/mpu_imu_v001/typedefs.h"

typedef struct {
 int x;
 int y;
 int z;
} tAxisB;

typedef struct {
 int x;
 int y;
 int z;
} tAxis;

typedef struct {
 float x;
 float y;
 float z;
} tAxisF;

typedef struct {
 int Heading;
 int Roll;
 int Pitch;
} eAxis;



typedef struct {
 tAxis accel;
 tAxis gyro;
 tAxis mag;
 eAxis euler;
 short temp;
 float gdt;
} tSensor;
#line 5 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/BNO55_I2C_driver.c"
tSensor BNO55;

char RAW_DATA_ACCEL[6];
short RAW_DATA_TEMPERATUR;
char RAW_DATA_GYRO[6];
char RAW_DATA_MAGNET[6];
char EULER_DATA[6];
#line 21 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/BNO55_I2C_driver.c"
void Config_BNO55(){
 unsigned char dat;
 int i;
#line 37 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/BNO55_I2C_driver.c"
 dat = 0x00;
 MPU_I2C_Write( 0x28 ,  0x3D , 1, &dat);
 delay_ms(50);


 dat = 0x01;
 MPU_I2C_Write( 0x28 ,  0x07 , 1, &dat);


 dat = 0b00001101;
 MPU_I2C_Write( 0x28 ,  0x08 ,1, &dat);


 dat = 0b00011000;
 MPU_I2C_Write( 0x28 ,  0x0A , 1, &dat);

 dat = 0x00;
 MPU_I2C_Write( 0x28 ,  0x0B , 1, &dat);



 dat = 0b000110;
 MPU_I2C_Write( 0x28 ,  0x09 , 1, &dat);


 dat = 0x00;
 MPU_I2C_Write( 0x28 ,  0x07 ,1, &dat);


 dat = 0x01;
 MPU_I2C_Write( 0x28 ,  0x3B ,1, &dat);


 dat = 0x01;
 MPU_I2C_Write( 0x28 ,  0x40 ,1, &dat );


 dat = 0x00;
 MPU_I2C_Write( 0x28 ,  0x3E , 1, &dat );




 for(i = 0; i <= 5; i++){

 dat =  0b0000001100 ;
 MPU_I2C_Write( 0x28 ,  0x3D , 1, &dat);
 Delay_ms(50);
 MPU_I2C_Read( 0x28 ,  0x3D  , 1, &dat);
 if(dat ==  0b0000001100 ) {
 UART2_write_text("Configuration succes\x0D\x0D");
 return;
 }
 }
 UART2_write_text("Configuration ERROR\x0D");
}


short BNO55_Detect() {
 unsigned char dat = 0;

 MPU_I2C_Read( 0x28 ,  0x00  , 1, &dat);

 if(dat ==  0xA0 ) {
 return 1;
 }else{
 return -1;
 }
}

void Print_MPU_register( unsigned char registerAdress , unsigned char dat){
 unsigned char dat_temp, mask;
 char stn_out[100], bit_string[10], adress_string[10];
 int i;

 mask = 0b00000001;

 for( i = 7; i>=0; i--){
 dat_temp = dat & mask;
 if(dat_temp > 0)
 bit_string[i] = '1';
 else
 bit_string[i] = '0';
 mask = mask << 1;
 }
 bit_string[9] = 0;

 ShortToHex( (unsigned short)registerAdress, adress_string);
 strcpy(stn_out,"Adresse = 0x");
 strcat(stn_out,adress_string);
 strcat(stn_out,"  Inhalt = 0b");
 strcat(stn_out,bit_string);
 strcat(stn_out,"\x0D");

 UART2_write_text(stn_out);
 delay_ms(20);

}


static void BNO55_ReadEuler() {

 MPU_I2C_Read_Multi( 0x28 ,  0X1A , 6, &EULER_DATA[0]);

 BNO55.euler.Heading = (int)((EULER_DATA[1] << 8) | EULER_DATA[0] );
 BNO55.euler.Roll = (int)((EULER_DATA[3] << 8) | EULER_DATA[2] );
 BNO55.euler.Pitch = (int)((EULER_DATA[5] << 8) | EULER_DATA[4] );
#line 150 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/BNO55_I2C_driver.c"
}


static void BNO55_ReadAccel() {
 volatile int temp_data;

 MPU_I2C_Read_Multi( 0x28 ,  0X08 , 6, &RAW_DATA_ACCEL[0]);
 BNO55.accel.x = (int)((RAW_DATA_ACCEL[1] << 8) | RAW_DATA_ACCEL[0] );
 BNO55.accel.y = (int)((RAW_DATA_ACCEL[3] << 8) | RAW_DATA_ACCEL[2] );
 BNO55.accel.z = (int)((RAW_DATA_ACCEL[5] << 8) | RAW_DATA_ACCEL[4] );

}

static void BNO55_ReadGyro() {

 MPU_I2C_Read_Multi( 0x28 ,  0X14 , 6, &RAW_DATA_GYRO[0]);

 BNO55.gyro.x = (int)((RAW_DATA_GYRO[1] << 8) | RAW_DATA_GYRO[0] );
 BNO55.gyro.y = (int)((RAW_DATA_GYRO[3] << 8) | RAW_DATA_GYRO[2] );
 BNO55.gyro.z = (int)((RAW_DATA_GYRO[5] << 8) | RAW_DATA_GYRO[4] );

}

static void BNO55_ReadMagnet() {

 MPU_I2C_Read_Multi( 0x28 ,  0X0E , 6, &RAW_DATA_MAGNET[0]);

 BNO55.mag.x = (int)((RAW_DATA_MAGNET[1] << 8) | RAW_DATA_MAGNET[0] );
 BNO55.mag.y = (int)((RAW_DATA_MAGNET[3] << 8) | RAW_DATA_MAGNET[2] );
 BNO55.mag.z = (int)((RAW_DATA_MAGNET[5] << 8) | RAW_DATA_MAGNET[4] );

}


static void BNO55_ReadTemp() {
 unsigned char dat = 0;

 MPU_I2C_Read( 0x28 ,  0X34  , 1, &dat);
 RAW_DATA_TEMPERATUR = (short) dat;
}

void BNO55_Read() {
 BNO55_ReadTemp();

 BNO55_ReadEuler();
#line 201 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/BNO55_I2C_driver.c"
}


void Print_BNO55_Data() {

char gyro_x_out[50], gyro_y_out[50], gyro_z_out[50],
 accel_x_out[50], accel_y_out[50], accel_z_out[50],
 mag_x_out[50], mag_y_out[50], mag_z_out[50],
 heading[50], roll[50], pitch[50],
 temp_out[50] ;
float gyro_x_float, gyro_y_float, gyro_z_float,
 accel_x_float, accel_y_float, accel_z_float,
 heading_float, roll_float, pitch_float,
 mag_x_float, mag_y_float, mag_z_float;
char stn_out[100], stn_out_temo[50];

 BNO55_Read();




 heading_float = (float)BNO55.euler.Heading;
 roll_float = (float)BNO55.euler.Roll ;
 pitch_float = (float)BNO55.euler.Pitch ;

 heading_float = heading_float /16.0;
 roll_float = roll_float /16.0;
 pitch_float = pitch_float /16.0;
#line 238 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/BNO55_I2C_driver.c"
 sprintf(heading," Heading = %1.0f" , heading_float);
 sprintf(roll ," Roll = %1.0f" , roll_float);
 sprintf(pitch ," Pitch = %1.0f" , pitch_float );
#line 272 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/BNO55_I2C_driver.c"
 UART2_Write_Text(stn_out);
#line 288 "C:/Users/Alexander Wolf/Documents/PROJEKTE/227 Balacing Robot/PIC32 BNO055/MPU_IMU_v001/BNO55_I2C_driver.c"
 UART2_Write_Text(heading);
 UART2_Write_Text(roll);
 UART2_Write_Text(pitch);


 UART2_Write(0x0D);

}


void Print_MPU_register_map(int startAdress, int EndAdress){
 unsigned char dat, dat_temp, mask, registerAdress;
 char stn_out[100], bit_string[10], adress_string[10];
 int i;

 for(registerAdress = startAdress; registerAdress <= EndAdress; registerAdress++){

 MPU_I2C_Read( 0x28 , registerAdress , 1, &dat);

 mask = 0b00000001;
 for( i = 7; i>=0; i--){
 dat_temp = dat & mask;
 if(dat_temp > 0)
 bit_string[i] = '1';
 else
 bit_string[i] = '0';
 mask = mask << 1;
 }
 bit_string[9] = 0;

 strcpy(stn_out,"Adresse =");
 ShortToStr( (short)registerAdress, adress_string);
 strcat(stn_out,adress_string);
 ShortToHex((unsigned short)registerAdress, adress_string);
 strcat(stn_out," (0x");
 strcat(stn_out,adress_string);
 strcat(stn_out,")  Inhalt = 0b");
 strcat(stn_out,bit_string);
 strcat(stn_out,"\x0D");

 UART2_write_text(stn_out);

 delay_ms(20);
 }
}






void MPU_I2C_Write(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
 volatile unsigned int i;
 volatile unsigned char s_addr_internW , s_addr_internR;

 s_addr_internW = (s_addr << 1) & 0xFE;

 I2C2_Start();
 I2C2_Write(s_addr_internW);
 I2C2_Write(r_addr);
 for (i = 0 ; i < len ; i++){
 I2C2_Write(*dat++);
 }
 I2C2_Stop();
}

void MPU_I2C_Read(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
 unsigned int i;
 unsigned char s_addr_internW , s_addr_internR;

 s_addr_internW = (s_addr << 1) & 0xFE;
 s_addr_internR = (s_addr << 1) | 0x01;

 I2C2_Start();
 I2C2_Write( s_addr_internW);
 I2C2_Write(r_addr);
 I2C2_Restart();
 I2C2_Write(s_addr_internR);
 for (i = 0; i < (len-1); i++){
 *dat++ = I2C2_Read(_I2C_ACK);
 }
 *dat = I2C2_Read(_I2C_NACK);
 I2C2_Stop();

}


void MPU_I2C_Read_Multi(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
 volatile unsigned int i;
 volatile unsigned char s_addr_internW , s_addr_internR;

 s_addr_internW = (s_addr << 1) & 0xFE;
 s_addr_internR = (s_addr << 1) | 0x01;

 I2C2_Start();
 I2C2_Write( s_addr_internW);
 I2C2_Write(r_addr);
 I2C2_Restart();
 I2C2_Write(s_addr_internR);
 for (i = 0; i < (len-1); i++){
 *dat = I2C2_Read(_I2C_ACK);
 dat++;
 }
 *dat = I2C2_Read(_I2C_NACK);
 I2C2_Stop();
}
