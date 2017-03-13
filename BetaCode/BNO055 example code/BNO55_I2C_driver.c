#include "BNO55_Register_Map.h"
#include "BNO55_I2C_driver.h"
#include "typedefs.h"

tSensor BNO55;

char  RAW_DATA_ACCEL[6];
short RAW_DATA_TEMPERATUR;
char  RAW_DATA_GYRO[6];
char  RAW_DATA_MAGNET[6];
char  EULER_DATA[6];
/*
unsigned char RAW_DATA_ACCEL[6];
short         RAW_DATA_TEMPERATUR;
unsigned char RAW_DATA_GYRO[6];
unsigned char RAW_DATA_MAGNET[6];
unsigned char EULER_DATA[6];
*/
//unsigned char MULTIREAD_TEST[6];

void Config_BNO55(){
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
   MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_OPR_MODE, 1,  &dat);
   delay_ms(50);

   // Select page 1 to configure sensors
   dat = 0x01;
   MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_PAGE_ID, 1,  &dat);

   // Configure ACC
   dat = 0b00001101;   // 4g , Bandwidth = 62,5Hz , normal op Mode
   MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_ACC_CONFIG,1,  &dat);

   // Configure GYR
   dat = 0b00011000;    // Range 2000dps; Bandw = 47Hz
   MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_GYRO_CONFIG_0, 1,  &dat);

   dat = 0x00;      // Normal OpMode
   MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_GYRO_CONFIG_1, 1,  &dat);

   // Configure MAG
   // 20Hz output Rate, OpMode = regular, Power Mode Normal  S.29
   dat = 0b000110;
   MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_MAG_CONFIG, 1,  &dat);

   // Select page 0 to read sensors
   dat = 0x00;
   MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_PAGE_ID,1,  &dat);

   // Select BNO055 sensor units (temperature in degrees C, rate in dps, accel in mg)
   dat = 0x01;
   MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_UNIT_SEL,1,  &dat);

   // Select BNO055 gyro temperature source
   dat = 0x01;
   MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_TEMP_SOURCE,1,  &dat );

    // Select BNO055 system power mode
   dat = 0x00;
   MPU_I2C_Write(BNO55_I2C_ADDR,  BNO055_PWR_MODE, 1, &dat );


  // Write Configuration to BNO55 Registers
  // EXIT Config mode and switch to selected Operation mode
  for(i = 0; i <= 5; i++){      // S 21
  //       dat = AMG_CON;
      dat = NDOF_CON;
      MPU_I2C_Write(BNO55_I2C_ADDR, OPR_MODE, 1, &dat);
      Delay_ms(50);
      MPU_I2C_Read(BNO55_I2C_ADDR,  OPR_MODE , 1, &dat);
      if(dat == NDOF_CON) {
          UART2_write_text("Configuration succes\x0D\x0D");
          return;
          }
      }
  UART2_write_text("Configuration ERROR\x0D");
}


short BNO55_Detect() {
  unsigned char dat = 0;
  
  MPU_I2C_Read(BNO55_I2C_ADDR, RM_CHIP_ID , 1, &dat);

  if(dat == RM_CHIP_ID_content) {
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
  //                   Adresse         Register             wohin schreiben
   MPU_I2C_Read_Multi(BNO55_I2C_ADDR, EULER_DATA_ADR, 6, &EULER_DATA[0]);   // Read Accel data from MPU werte bereits in funktion berechnet

   BNO55.euler.Heading = (int)((EULER_DATA[1] << 8) | EULER_DATA[0] );
   BNO55.euler.Roll    = (int)((EULER_DATA[3] << 8) | EULER_DATA[2] );
   BNO55.euler.Pitch   = (int)((EULER_DATA[5] << 8) | EULER_DATA[4] );
/*
   S 30
   Pitch -180° to +180° (turing clockwise increases values)
   Roll -90° to +90° (increasing with increasing inclination)
   Heading / Yaw 0° to 360° (turning clockwise increases values)
 */
}


static void BNO55_ReadAccel() {
  volatile int temp_data;

   MPU_I2C_Read_Multi(BNO55_I2C_ADDR, ACCEL_DATA_ADR, 6, &RAW_DATA_ACCEL[0]);   // Read Accel data from MPU werte bereits in funktion berechnet
   BNO55.accel.x = (int)((RAW_DATA_ACCEL[1] << 8) | RAW_DATA_ACCEL[0] );
   BNO55.accel.y = (int)((RAW_DATA_ACCEL[3] << 8) | RAW_DATA_ACCEL[2] );
   BNO55.accel.z = (int)((RAW_DATA_ACCEL[5] << 8) | RAW_DATA_ACCEL[4] );
 
}

static void BNO55_ReadGyro() {

   MPU_I2C_Read_Multi(BNO55_I2C_ADDR, GYRO_DATA_ADR, 6, &RAW_DATA_GYRO[0]);   // Read Accel data from MPU

   BNO55.gyro.x = (int)((RAW_DATA_GYRO[1] << 8) | RAW_DATA_GYRO[0] );
   BNO55.gyro.y = (int)((RAW_DATA_GYRO[3] << 8) | RAW_DATA_GYRO[2] );
   BNO55.gyro.z = (int)((RAW_DATA_GYRO[5] << 8) | RAW_DATA_GYRO[4] );

}

static void BNO55_ReadMagnet() {

   MPU_I2C_Read_Multi(BNO55_I2C_ADDR, MAG_DATA_ADR, 6, &RAW_DATA_MAGNET[0]);   // Read Accel data from MPU

   BNO55.mag.x = (int)((RAW_DATA_MAGNET[1] << 8) | RAW_DATA_MAGNET[0] );
   BNO55.mag.y = (int)((RAW_DATA_MAGNET[3] << 8) | RAW_DATA_MAGNET[2] );
   BNO55.mag.z = (int)((RAW_DATA_MAGNET[5] << 8) | RAW_DATA_MAGNET[4] );

}


static void BNO55_ReadTemp() {
   unsigned char dat = 0;

   MPU_I2C_Read(BNO55_I2C_ADDR, TEMPERATUR_DATA_ADR , 1, &dat);
   RAW_DATA_TEMPERATUR  = (short) dat;        // 1°C = 1 LSB
}

void BNO55_Read() {
   BNO55_ReadTemp();

   BNO55_ReadEuler();
 /*
   BNO55_ReadAccel();
   BNO55_ReadGyro();

   BNO55_ReadMagnet();
*/
}


void Print_BNO55_Data()  {

char  gyro_x_out[50], gyro_y_out[50], gyro_z_out[50], 
      accel_x_out[50], accel_y_out[50], accel_z_out[50],
      mag_x_out[50], mag_y_out[50], mag_z_out[50],
      heading[50], roll[50], pitch[50],
      temp_out[50] ;
float gyro_x_float, gyro_y_float, gyro_z_float, 
      accel_x_float, accel_y_float, accel_z_float,
      heading_float, roll_float, pitch_float,
      mag_x_float, mag_y_float, mag_z_float;
char  stn_out[100], stn_out_temo[50];

  BNO55_Read();
  

  // Calculate the Euler angles values in degrees

  heading_float = (float)BNO55.euler.Heading;     // S 35 . da  1 degree = 16 LSB
  roll_float    = (float)BNO55.euler.Roll   ;
  pitch_float   = (float)BNO55.euler.Pitch  ;

  heading_float =  heading_float /16.0;
  roll_float    =  roll_float    /16.0;
  pitch_float   =  pitch_float   /16.0;
 
 
  /*
  heading_float = (float)BNO55.euler.Heading;
  roll_float    = (float)BNO55.euler.Roll;
  pitch_float   = (float)BNO55.euler.Pitch;
   */

 
  sprintf(heading," Heading = %1.0f" , heading_float);
  sprintf(roll   ," Roll = %1.0f"    , roll_float);
  sprintf(pitch  ," Pitch = %1.0f"   , pitch_float );
/*

  // calculate the accleration value into actual mg's
  accel_x_float = (float)BNO55.accel.x;
  accel_y_float = (float)BNO55.accel.y;
  accel_z_float = (float)BNO55.accel.z;

  sprintf(accel_x_out,"  Accel X = %1.3f", accel_x_float / 1000.0 );
  sprintf(accel_y_out," Y = %1.3f", accel_y_float / 1000.0 );
  sprintf(accel_z_out," Z = %1.3f", accel_z_float / 1000.0 );

  // Calculate the gyro value into actual degrees per second
  gyro_x_float = (float)BNO55.gyro.x /16.0;
  gyro_y_float = (float)BNO55.gyro.y /16.0;
  gyro_z_float = (float)BNO55.gyro.z /16.0;

  sprintf(gyro_x_out," Gyro X = %1.3f", gyro_x_float);
  sprintf(gyro_y_out," Y = %1.3f", gyro_y_float);
  sprintf(gyro_z_out," Z = %1.3f", gyro_z_float);

  // Calculate the magnetometer values in milliGauss
  mag_x_float = (float)BNO55.mag.x /1.6;
  mag_y_float = (float)BNO55.mag.y /1.6;
  mag_z_float = (float)BNO55.mag.z /1.6;

  sprintf(mag_x_out," Mag X = %1.3f", mag_x_float);
  sprintf(mag_y_out," Y = %1.3f", mag_y_float);
  sprintf(mag_z_out," Z = %1.3f", mag_z_float);
  
  sprintf(stn_out,"Temp = %1.1f  ", (float) RAW_DATA_TEMPERATUR);
 */
  UART2_Write_Text(stn_out);
  /*
  UART2_Write_Text(mag_x_out);
  UART2_Write_Text(mag_y_out);
  UART2_Write_Text(mag_z_out);


  UART2_Write_Text(gyro_x_out);
  UART2_Write_Text(gyro_y_out);
  UART2_Write_Text(gyro_z_out);

  UART2_Write_Text(accel_x_out);
  UART2_Write_Text(accel_y_out);
  UART2_Write_Text(accel_z_out);
 */

  UART2_Write_Text(heading);
  UART2_Write_Text(roll);
  UART2_Write_Text(pitch);
 /*   */
  
  UART2_Write(0x0D);

}


void Print_MPU_register_map(int startAdress, int EndAdress){
  unsigned char dat, dat_temp, mask, registerAdress;
  char stn_out[100], bit_string[10], adress_string[10];
  int i;
  
  for(registerAdress = startAdress; registerAdress <= EndAdress; registerAdress++){

    MPU_I2C_Read(BNO55_I2C_ADDR, registerAdress , 1, &dat);

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

////////////////////////////////////////////////////////////////////////////////
// READ & Write funktions                                                     //
////////////////////////////////////////////////////////////////////////////////

 //   MPU_I2C_Write(mpu_I2C_ADDR, mpu_rm_PWR_MGMT_1 , 1, 0x80);
void MPU_I2C_Write(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
  volatile unsigned int i;
  volatile unsigned char s_addr_internW , s_addr_internR;

  s_addr_internW = (s_addr << 1) & 0xFE;     // shift der Adresse um 1 Bit nach links da für Adresse nur obersten 7 Bits verwendet werden + Write 0bit an stelle 0

  I2C2_Start();                         // issue I2C start signal
  I2C2_Write(s_addr_internW);            // send byte via I2C  (device address + W(&0xFE))
  I2C2_Write(r_addr);                   // send byte (address of EEPROM location)
  for (i = 0 ; i < len ; i++){
    I2C2_Write(*dat++);                 // send data (data to be written)
  }
  I2C2_Stop();                          // issue I2C stop signal
}

void MPU_I2C_Read(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
  unsigned int i;
  unsigned char s_addr_internW , s_addr_internR;

  s_addr_internW = (s_addr << 1) & 0xFE;     // shift der Adresse um 1 Bit nach links da für Adresse nur obersten 7 Bits verwendet werden + Write 0bit an stelle 0
  s_addr_internR = (s_addr << 1) | 0x01;     // shift der Adresse um 1 Bit nach links da für Adresse nur obersten 7 Bits verwendet werden + Read  1bit an stelle 0

  I2C2_Start();                         // issue I2C start signal
  I2C2_Write( s_addr_internW);            // send byte via I2C  (device address + W(&0xFE))
  I2C2_Write(r_addr);                   // send byte (data address)
  I2C2_Restart();                       // issue I2C signal repeated start
  I2C2_Write(s_addr_internR);            // send byte (device address + R(|0x01))
  for (i = 0; i < (len-1); i++){
    *dat++ = I2C2_Read(_I2C_ACK);      // Read the data (acknowledge)
  }
  *dat = I2C2_Read(_I2C_NACK);         // Read the data (NO acknowledge)
  I2C2_Stop();
 
}


void MPU_I2C_Read_Multi(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
  volatile unsigned int i;
  volatile unsigned char s_addr_internW , s_addr_internR;

  s_addr_internW = (s_addr << 1) & 0xFE;     // shift der Adresse um 1 Bit nach links da für Adresse nur obersten 7 Bits verwendet werden + Write 0bit an stelle 0
  s_addr_internR = (s_addr << 1) | 0x01;     // shift der Adresse um 1 Bit nach links da für Adresse nur obersten 7 Bits verwendet werden + Read  1bit an stelle 0

  I2C2_Start();                         // issue I2C start signal
  I2C2_Write( s_addr_internW);            // send byte via I2C  (device address + W(&0xFE))
  I2C2_Write(r_addr);                   // send byte (data address)
  I2C2_Restart();                       // issue I2C signal repeated start
  I2C2_Write(s_addr_internR);            // send byte (device address + R(|0x01))
  for (i = 0; i < (len-1); i++){
    *dat = I2C2_Read(_I2C_ACK);      // Read the data (acknowledge)
    dat++;
  }
  *dat = I2C2_Read(_I2C_NACK);         // Read the data (NO acknowledge)
  I2C2_Stop();
}