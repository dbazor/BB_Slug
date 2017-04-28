#line 1 "C:/Users/Alexander Wolf/Documents/PROJEKTE/PROGRAMMIERUNG/PIC/Programme/Eigene Programme II/MPU IMU/MPU IMU 9150/PIC32/MPU_IMU_v001/MPU_IMU_I2C.c"


void MPU_I2C_Write(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
 unsigned int i;

 I2C2_Start();
 I2C2_Write(s_addr & 0xFE);
 I2C2_Write(r_addr);
 for (i = 0 ; i < len ; i++){
 I2C2_Write(*dat++);
 }
 I2C2_Stop();
}



void MPU_I2C_Read(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
 unsigned int i;
 I2C2_Start();
 I2C2_Write(s_addr & 0xFE);
 I2C2_Write(r_addr);
 I2C2_Restart();
 I2C2_Write(s_addr | 0x01);
 for (i = 0; i < (len-1); i++){
 *dat++ = I2C2_Read(_I2C_ACK);
 }
 *dat = I2C2_Read(_I2C_NACK);
 I2C2_Stop();
}

void MPU_I2C_Read_Int(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
 unsigned int i;
 unsigned short *pt;
 I2C2_Start();
 I2C2_Write(s_addr & 0xFE);
 I2C2_Write(r_addr);
 I2C2_Restart();
 I2C2_Write(s_addr | 0x01);
 for (i = 0 ; i < ((len << 1)-1) ; i++){
 if (i%2) {
 pt = pt - 1;
 }
 else {
 pt = dat + i + 1;
 }
 *pt = I2C2_Read(_I2C_ACK);
 }
 *(pt - 1) = I2C2_Read(_I2C_NACK);
 I2C2_Stop();
}
