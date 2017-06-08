#define    ACCEL_4G_TO_SCALE_G           0.000488281    // +- 4g = 14 bits == 8 g für 16384bits D.h. 0.000488281 g pro bit


#define    GYRO_2000DPS_TO_SCALE_DEG     0.0305         // 2000°/s = 16bits == 2000 auf 65536 D.h.  0.0305°/s pro bit

short BNO55_Detect();
void Config_BNO55();
void Print_MPU_register_map(int, int);
void Print_MPU_register(unsigned char, unsigned char);
void Print_BNO55_Data();

void MPU_I2C_Write(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat);
void MPU_I2C_Read_Multi(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat);
void MPU_I2C_Read(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat);