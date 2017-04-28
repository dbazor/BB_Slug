_Config_BNO55:
;BNO55_I2C_driver.c,21 :: 		void Config_BNO55(){
ADDIU	SP, SP, -24
SW	RA, 0(SP)
;BNO55_I2C_driver.c,37 :: 		dat = 0x00;
SW	R25, 4(SP)
SW	R26, 8(SP)
SW	R27, 12(SP)
SW	R28, 16(SP)
SB	R0, 20(SP)
;BNO55_I2C_driver.c,38 :: 		MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_OPR_MODE, 1,  &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 61
ORI	R25, R0, 40
JAL	_MPU_I2C_Write+0
NOP	
;BNO55_I2C_driver.c,39 :: 		delay_ms(50);
LUI	R24, 20
ORI	R24, R24, 22612
L_Config_BNO550:
ADDIU	R24, R24, -1
BNE	R24, R0, L_Config_BNO550
NOP	
NOP	
NOP	
;BNO55_I2C_driver.c,42 :: 		dat = 0x01;
ORI	R2, R0, 1
SB	R2, 20(SP)
;BNO55_I2C_driver.c,43 :: 		MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_PAGE_ID, 1,  &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 7
ORI	R25, R0, 40
JAL	_MPU_I2C_Write+0
NOP	
;BNO55_I2C_driver.c,46 :: 		dat = 0b00001101;   // 4g , Bandwidth = 62,5Hz , normal op Mode
ORI	R2, R0, 13
SB	R2, 20(SP)
;BNO55_I2C_driver.c,47 :: 		MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_ACC_CONFIG,1,  &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 8
ORI	R25, R0, 40
JAL	_MPU_I2C_Write+0
NOP	
;BNO55_I2C_driver.c,50 :: 		dat = 0b00011000;    // Range 2000dps; Bandw = 47Hz
ORI	R2, R0, 24
SB	R2, 20(SP)
;BNO55_I2C_driver.c,51 :: 		MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_GYRO_CONFIG_0, 1,  &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 10
ORI	R25, R0, 40
JAL	_MPU_I2C_Write+0
NOP	
;BNO55_I2C_driver.c,53 :: 		dat = 0x00;      // Normal OpMode
SB	R0, 20(SP)
;BNO55_I2C_driver.c,54 :: 		MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_GYRO_CONFIG_1, 1,  &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 11
ORI	R25, R0, 40
JAL	_MPU_I2C_Write+0
NOP	
;BNO55_I2C_driver.c,58 :: 		dat = 0b000110;
ORI	R2, R0, 6
SB	R2, 20(SP)
;BNO55_I2C_driver.c,59 :: 		MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_MAG_CONFIG, 1,  &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 9
ORI	R25, R0, 40
JAL	_MPU_I2C_Write+0
NOP	
;BNO55_I2C_driver.c,62 :: 		dat = 0x00;
SB	R0, 20(SP)
;BNO55_I2C_driver.c,63 :: 		MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_PAGE_ID,1,  &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 7
ORI	R25, R0, 40
JAL	_MPU_I2C_Write+0
NOP	
;BNO55_I2C_driver.c,66 :: 		dat = 0x01;
ORI	R2, R0, 1
SB	R2, 20(SP)
;BNO55_I2C_driver.c,67 :: 		MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_UNIT_SEL,1,  &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 59
ORI	R25, R0, 40
JAL	_MPU_I2C_Write+0
NOP	
;BNO55_I2C_driver.c,70 :: 		dat = 0x01;
ORI	R2, R0, 1
SB	R2, 20(SP)
;BNO55_I2C_driver.c,71 :: 		MPU_I2C_Write(BNO55_I2C_ADDR, BNO055_TEMP_SOURCE,1,  &dat );
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 64
ORI	R25, R0, 40
JAL	_MPU_I2C_Write+0
NOP	
;BNO55_I2C_driver.c,74 :: 		dat = 0x00;
SB	R0, 20(SP)
;BNO55_I2C_driver.c,75 :: 		MPU_I2C_Write(BNO55_I2C_ADDR,  BNO055_PWR_MODE, 1, &dat );
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 62
ORI	R25, R0, 40
JAL	_MPU_I2C_Write+0
NOP	
;BNO55_I2C_driver.c,80 :: 		for(i = 0; i <= 5; i++){      // S 21
; i start address is: 24 (R6)
MOVZ	R6, R0, R0
; i end address is: 24 (R6)
L_Config_BNO552:
; i start address is: 24 (R6)
SEH	R2, R6
SLTI	R2, R2, 6
BNE	R2, R0, L__Config_BNO5537
NOP	
J	L_Config_BNO553
NOP	
L__Config_BNO5537:
;BNO55_I2C_driver.c,82 :: 		dat = NDOF_CON;
ORI	R2, R0, 12
SB	R2, 20(SP)
;BNO55_I2C_driver.c,83 :: 		MPU_I2C_Write(BNO55_I2C_ADDR, OPR_MODE, 1, &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 61
ORI	R25, R0, 40
JAL	_MPU_I2C_Write+0
NOP	
;BNO55_I2C_driver.c,84 :: 		Delay_ms(50);
LUI	R24, 20
ORI	R24, R24, 22612
L_Config_BNO555:
ADDIU	R24, R24, -1
BNE	R24, R0, L_Config_BNO555
NOP	
NOP	
NOP	
;BNO55_I2C_driver.c,85 :: 		MPU_I2C_Read(BNO55_I2C_ADDR,  OPR_MODE , 1, &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 61
ORI	R25, R0, 40
JAL	_MPU_I2C_Read+0
NOP	
;BNO55_I2C_driver.c,86 :: 		if(dat == NDOF_CON) {
LBU	R3, 20(SP)
ORI	R2, R0, 12
BEQ	R3, R2, L__Config_BNO5538
NOP	
J	L_Config_BNO557
NOP	
L__Config_BNO5538:
; i end address is: 24 (R6)
;BNO55_I2C_driver.c,87 :: 		UART2_write_text("Configuration succes\x0D\x0D");
LUI	R25, hi_addr(?lstr1_BNO55_I2C_driver+0)
ORI	R25, R25, lo_addr(?lstr1_BNO55_I2C_driver+0)
JAL	_UART2_Write_Text+0
NOP	
;BNO55_I2C_driver.c,88 :: 		return;
J	L_end_Config_BNO55
NOP	
;BNO55_I2C_driver.c,89 :: 		}
L_Config_BNO557:
;BNO55_I2C_driver.c,80 :: 		for(i = 0; i <= 5; i++){      // S 21
; i start address is: 24 (R6)
ADDIU	R2, R6, 1
SEH	R6, R2
;BNO55_I2C_driver.c,90 :: 		}
; i end address is: 24 (R6)
J	L_Config_BNO552
NOP	
L_Config_BNO553:
;BNO55_I2C_driver.c,91 :: 		UART2_write_text("Configuration ERROR\x0D");
LUI	R25, hi_addr(?lstr2_BNO55_I2C_driver+0)
ORI	R25, R25, lo_addr(?lstr2_BNO55_I2C_driver+0)
JAL	_UART2_Write_Text+0
NOP	
;BNO55_I2C_driver.c,92 :: 		}
L_end_Config_BNO55:
LW	R28, 16(SP)
LW	R27, 12(SP)
LW	R26, 8(SP)
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 24
JR	RA
NOP	
; end of _Config_BNO55
_BNO55_Detect:
;BNO55_I2C_driver.c,95 :: 		short BNO55_Detect() {
ADDIU	SP, SP, -24
SW	RA, 0(SP)
;BNO55_I2C_driver.c,96 :: 		unsigned char dat = 0;
SW	R25, 4(SP)
SW	R26, 8(SP)
SW	R27, 12(SP)
SW	R28, 16(SP)
MOVZ	R30, R0, R0
SB	R30, 20(SP)
;BNO55_I2C_driver.c,98 :: 		MPU_I2C_Read(BNO55_I2C_ADDR, RM_CHIP_ID , 1, &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
MOVZ	R26, R0, R0
ORI	R25, R0, 40
JAL	_MPU_I2C_Read+0
NOP	
;BNO55_I2C_driver.c,100 :: 		if(dat == RM_CHIP_ID_content) {
LBU	R3, 20(SP)
ORI	R2, R0, 160
BEQ	R3, R2, L__BNO55_Detect40
NOP	
J	L_BNO55_Detect8
NOP	
L__BNO55_Detect40:
;BNO55_I2C_driver.c,101 :: 		return 1;
ORI	R2, R0, 1
J	L_end_BNO55_Detect
NOP	
;BNO55_I2C_driver.c,102 :: 		}else{
L_BNO55_Detect8:
;BNO55_I2C_driver.c,103 :: 		return -1;
ORI	R2, R0, 255
;BNO55_I2C_driver.c,105 :: 		}
;BNO55_I2C_driver.c,103 :: 		return -1;
;BNO55_I2C_driver.c,105 :: 		}
L_end_BNO55_Detect:
LW	R28, 16(SP)
LW	R27, 12(SP)
LW	R26, 8(SP)
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 24
JR	RA
NOP	
; end of _BNO55_Detect
_Print_MPU_register:
;BNO55_I2C_driver.c,107 :: 		void Print_MPU_register( unsigned char registerAdress , unsigned char dat){
ADDIU	SP, SP, -128
SW	RA, 0(SP)
;BNO55_I2C_driver.c,112 :: 		mask = 0b00000001;
; mask start address is: 20 (R5)
ORI	R5, R0, 1
;BNO55_I2C_driver.c,114 :: 		for( i = 7; i>=0; i--){
; i start address is: 16 (R4)
ORI	R4, R0, 7
; mask end address is: 20 (R5)
; i end address is: 16 (R4)
L_Print_MPU_register10:
; i start address is: 16 (R4)
; mask start address is: 20 (R5)
SEH	R2, R4
SLTI	R2, R2, 0
BEQ	R2, R0, L__Print_MPU_register42
NOP	
J	L_Print_MPU_register11
NOP	
L__Print_MPU_register42:
;BNO55_I2C_driver.c,115 :: 		dat_temp = dat & mask;
AND	R2, R26, R5
;BNO55_I2C_driver.c,116 :: 		if(dat_temp > 0)
ANDI	R2, R2, 255
SLTIU	R2, R2, 1
BEQ	R2, R0, L__Print_MPU_register43
NOP	
J	L_Print_MPU_register13
NOP	
L__Print_MPU_register43:
;BNO55_I2C_driver.c,117 :: 		bit_string[i] = '1';
ADDIU	R3, SP, 106
SEH	R2, R4
ADDU	R3, R3, R2
ORI	R2, R0, 49
SB	R2, 0(R3)
J	L_Print_MPU_register14
NOP	
L_Print_MPU_register13:
;BNO55_I2C_driver.c,119 :: 		bit_string[i] = '0';
ADDIU	R3, SP, 106
SEH	R2, R4
ADDU	R3, R3, R2
ORI	R2, R0, 48
SB	R2, 0(R3)
L_Print_MPU_register14:
;BNO55_I2C_driver.c,120 :: 		mask = mask << 1;
ANDI	R2, R5, 255
; mask end address is: 20 (R5)
SLL	R2, R2, 1
; mask start address is: 20 (R5)
ANDI	R5, R2, 255
;BNO55_I2C_driver.c,114 :: 		for( i = 7; i>=0; i--){
ADDIU	R2, R4, -1
SEH	R4, R2
;BNO55_I2C_driver.c,121 :: 		}
; mask end address is: 20 (R5)
; i end address is: 16 (R4)
J	L_Print_MPU_register10
NOP	
L_Print_MPU_register11:
;BNO55_I2C_driver.c,122 :: 		bit_string[9] = 0;
ADDIU	R2, SP, 106
ADDIU	R2, R2, 9
SB	R0, 0(R2)
;BNO55_I2C_driver.c,124 :: 		ShortToHex( (unsigned short)registerAdress, adress_string);
ADDIU	R2, SP, 116
SB	R26, 4(SP)
MOVZ	R26, R2, R0
JAL	_ShortToHex+0
NOP	
;BNO55_I2C_driver.c,125 :: 		strcpy(stn_out,"Adresse = 0x");
ADDIU	R2, SP, 6
SB	R25, 5(SP)
LUI	R26, hi_addr(?lstr3_BNO55_I2C_driver+0)
ORI	R26, R26, lo_addr(?lstr3_BNO55_I2C_driver+0)
MOVZ	R25, R2, R0
JAL	_strcpy+0
NOP	
;BNO55_I2C_driver.c,126 :: 		strcat(stn_out,adress_string);
ADDIU	R3, SP, 116
ADDIU	R2, SP, 6
MOVZ	R26, R3, R0
MOVZ	R25, R2, R0
JAL	_strcat+0
NOP	
;BNO55_I2C_driver.c,127 :: 		strcat(stn_out,"  Inhalt = 0b");
ADDIU	R2, SP, 6
LUI	R26, hi_addr(?lstr4_BNO55_I2C_driver+0)
ORI	R26, R26, lo_addr(?lstr4_BNO55_I2C_driver+0)
MOVZ	R25, R2, R0
JAL	_strcat+0
NOP	
;BNO55_I2C_driver.c,128 :: 		strcat(stn_out,bit_string);
ADDIU	R3, SP, 106
ADDIU	R2, SP, 6
MOVZ	R26, R3, R0
MOVZ	R25, R2, R0
JAL	_strcat+0
NOP	
;BNO55_I2C_driver.c,129 :: 		strcat(stn_out,"\x0D");
ADDIU	R2, SP, 6
LUI	R26, hi_addr(?lstr5_BNO55_I2C_driver+0)
ORI	R26, R26, lo_addr(?lstr5_BNO55_I2C_driver+0)
MOVZ	R25, R2, R0
JAL	_strcat+0
NOP	
LBU	R25, 5(SP)
LBU	R26, 4(SP)
;BNO55_I2C_driver.c,131 :: 		UART2_write_text(stn_out);
ADDIU	R2, SP, 6
SB	R25, 4(SP)
MOVZ	R25, R2, R0
JAL	_UART2_Write_Text+0
NOP	
LBU	R25, 4(SP)
;BNO55_I2C_driver.c,132 :: 		delay_ms(20);
LUI	R24, 8
ORI	R24, R24, 9044
L_Print_MPU_register15:
ADDIU	R24, R24, -1
BNE	R24, R0, L_Print_MPU_register15
NOP	
NOP	
NOP	
;BNO55_I2C_driver.c,134 :: 		}
L_end_Print_MPU_register:
LW	RA, 0(SP)
ADDIU	SP, SP, 128
JR	RA
NOP	
; end of _Print_MPU_register
BNO55_I2C_driver_BNO55_ReadEuler:
;BNO55_I2C_driver.c,137 :: 		static void BNO55_ReadEuler() {
ADDIU	SP, SP, -20
SW	RA, 0(SP)
;BNO55_I2C_driver.c,139 :: 		MPU_I2C_Read_Multi(BNO55_I2C_ADDR, EULER_DATA_ADR, 6, &EULER_DATA[0]);   // Read Accel data from MPU werte bereits in funktion berechnet
SW	R25, 4(SP)
SW	R26, 8(SP)
SW	R27, 12(SP)
SW	R28, 16(SP)
LUI	R28, hi_addr(_EULER_DATA+0)
ORI	R28, R28, lo_addr(_EULER_DATA+0)
ORI	R27, R0, 6
ORI	R26, R0, 26
ORI	R25, R0, 40
JAL	_MPU_I2C_Read_Multi+0
NOP	
;BNO55_I2C_driver.c,141 :: 		BNO55.euler.Heading = (int)((EULER_DATA[1] << 8) | EULER_DATA[0] );
LBU	R2, Offset(_EULER_DATA+1)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_EULER_DATA+0)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+18)(GP)
;BNO55_I2C_driver.c,142 :: 		BNO55.euler.Roll    = (int)((EULER_DATA[3] << 8) | EULER_DATA[2] );
LBU	R2, Offset(_EULER_DATA+3)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_EULER_DATA+2)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+20)(GP)
;BNO55_I2C_driver.c,143 :: 		BNO55.euler.Pitch   = (int)((EULER_DATA[5] << 8) | EULER_DATA[4] );
LBU	R2, Offset(_EULER_DATA+5)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_EULER_DATA+4)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+22)(GP)
;BNO55_I2C_driver.c,150 :: 		}
L_end_BNO55_ReadEuler:
LW	R28, 16(SP)
LW	R27, 12(SP)
LW	R26, 8(SP)
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 20
JR	RA
NOP	
; end of BNO55_I2C_driver_BNO55_ReadEuler
BNO55_I2C_driver_BNO55_ReadAccel:
;BNO55_I2C_driver.c,153 :: 		static void BNO55_ReadAccel() {
ADDIU	SP, SP, -20
SW	RA, 0(SP)
;BNO55_I2C_driver.c,156 :: 		MPU_I2C_Read_Multi(BNO55_I2C_ADDR, ACCEL_DATA_ADR, 6, &RAW_DATA_ACCEL[0]);   // Read Accel data from MPU werte bereits in funktion berechnet
SW	R25, 4(SP)
SW	R26, 8(SP)
SW	R27, 12(SP)
SW	R28, 16(SP)
LUI	R28, hi_addr(_RAW_DATA_ACCEL+0)
ORI	R28, R28, lo_addr(_RAW_DATA_ACCEL+0)
ORI	R27, R0, 6
ORI	R26, R0, 8
ORI	R25, R0, 40
JAL	_MPU_I2C_Read_Multi+0
NOP	
;BNO55_I2C_driver.c,157 :: 		BNO55.accel.x = (int)((RAW_DATA_ACCEL[1] << 8) | RAW_DATA_ACCEL[0] );
LBU	R2, Offset(_RAW_DATA_ACCEL+1)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_RAW_DATA_ACCEL+0)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+0)(GP)
;BNO55_I2C_driver.c,158 :: 		BNO55.accel.y = (int)((RAW_DATA_ACCEL[3] << 8) | RAW_DATA_ACCEL[2] );
LBU	R2, Offset(_RAW_DATA_ACCEL+3)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_RAW_DATA_ACCEL+2)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+2)(GP)
;BNO55_I2C_driver.c,159 :: 		BNO55.accel.z = (int)((RAW_DATA_ACCEL[5] << 8) | RAW_DATA_ACCEL[4] );
LBU	R2, Offset(_RAW_DATA_ACCEL+5)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_RAW_DATA_ACCEL+4)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+4)(GP)
;BNO55_I2C_driver.c,161 :: 		}
L_end_BNO55_ReadAccel:
LW	R28, 16(SP)
LW	R27, 12(SP)
LW	R26, 8(SP)
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 20
JR	RA
NOP	
; end of BNO55_I2C_driver_BNO55_ReadAccel
BNO55_I2C_driver_BNO55_ReadGyro:
;BNO55_I2C_driver.c,163 :: 		static void BNO55_ReadGyro() {
ADDIU	SP, SP, -20
SW	RA, 0(SP)
;BNO55_I2C_driver.c,165 :: 		MPU_I2C_Read_Multi(BNO55_I2C_ADDR, GYRO_DATA_ADR, 6, &RAW_DATA_GYRO[0]);   // Read Accel data from MPU
SW	R25, 4(SP)
SW	R26, 8(SP)
SW	R27, 12(SP)
SW	R28, 16(SP)
LUI	R28, hi_addr(_RAW_DATA_GYRO+0)
ORI	R28, R28, lo_addr(_RAW_DATA_GYRO+0)
ORI	R27, R0, 6
ORI	R26, R0, 20
ORI	R25, R0, 40
JAL	_MPU_I2C_Read_Multi+0
NOP	
;BNO55_I2C_driver.c,167 :: 		BNO55.gyro.x = (int)((RAW_DATA_GYRO[1] << 8) | RAW_DATA_GYRO[0] );
LBU	R2, Offset(_RAW_DATA_GYRO+1)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_RAW_DATA_GYRO+0)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+6)(GP)
;BNO55_I2C_driver.c,168 :: 		BNO55.gyro.y = (int)((RAW_DATA_GYRO[3] << 8) | RAW_DATA_GYRO[2] );
LBU	R2, Offset(_RAW_DATA_GYRO+3)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_RAW_DATA_GYRO+2)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+8)(GP)
;BNO55_I2C_driver.c,169 :: 		BNO55.gyro.z = (int)((RAW_DATA_GYRO[5] << 8) | RAW_DATA_GYRO[4] );
LBU	R2, Offset(_RAW_DATA_GYRO+5)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_RAW_DATA_GYRO+4)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+10)(GP)
;BNO55_I2C_driver.c,171 :: 		}
L_end_BNO55_ReadGyro:
LW	R28, 16(SP)
LW	R27, 12(SP)
LW	R26, 8(SP)
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 20
JR	RA
NOP	
; end of BNO55_I2C_driver_BNO55_ReadGyro
BNO55_I2C_driver_BNO55_ReadMagnet:
;BNO55_I2C_driver.c,173 :: 		static void BNO55_ReadMagnet() {
ADDIU	SP, SP, -20
SW	RA, 0(SP)
;BNO55_I2C_driver.c,175 :: 		MPU_I2C_Read_Multi(BNO55_I2C_ADDR, MAG_DATA_ADR, 6, &RAW_DATA_MAGNET[0]);   // Read Accel data from MPU
SW	R25, 4(SP)
SW	R26, 8(SP)
SW	R27, 12(SP)
SW	R28, 16(SP)
LUI	R28, hi_addr(_RAW_DATA_MAGNET+0)
ORI	R28, R28, lo_addr(_RAW_DATA_MAGNET+0)
ORI	R27, R0, 6
ORI	R26, R0, 14
ORI	R25, R0, 40
JAL	_MPU_I2C_Read_Multi+0
NOP	
;BNO55_I2C_driver.c,177 :: 		BNO55.mag.x = (int)((RAW_DATA_MAGNET[1] << 8) | RAW_DATA_MAGNET[0] );
LBU	R2, Offset(_RAW_DATA_MAGNET+1)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_RAW_DATA_MAGNET+0)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+12)(GP)
;BNO55_I2C_driver.c,178 :: 		BNO55.mag.y = (int)((RAW_DATA_MAGNET[3] << 8) | RAW_DATA_MAGNET[2] );
LBU	R2, Offset(_RAW_DATA_MAGNET+3)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_RAW_DATA_MAGNET+2)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+14)(GP)
;BNO55_I2C_driver.c,179 :: 		BNO55.mag.z = (int)((RAW_DATA_MAGNET[5] << 8) | RAW_DATA_MAGNET[4] );
LBU	R2, Offset(_RAW_DATA_MAGNET+5)(GP)
SLL	R3, R2, 8
LBU	R2, Offset(_RAW_DATA_MAGNET+4)(GP)
OR	R2, R3, R2
SH	R2, Offset(_BNO55+16)(GP)
;BNO55_I2C_driver.c,181 :: 		}
L_end_BNO55_ReadMagnet:
LW	R28, 16(SP)
LW	R27, 12(SP)
LW	R26, 8(SP)
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 20
JR	RA
NOP	
; end of BNO55_I2C_driver_BNO55_ReadMagnet
BNO55_I2C_driver_BNO55_ReadTemp:
;BNO55_I2C_driver.c,184 :: 		static void BNO55_ReadTemp() {
ADDIU	SP, SP, -24
SW	RA, 0(SP)
;BNO55_I2C_driver.c,185 :: 		unsigned char dat = 0;
SW	R25, 4(SP)
SW	R26, 8(SP)
SW	R27, 12(SP)
SW	R28, 16(SP)
MOVZ	R30, R0, R0
SB	R30, 20(SP)
;BNO55_I2C_driver.c,187 :: 		MPU_I2C_Read(BNO55_I2C_ADDR, TEMPERATUR_DATA_ADR , 1, &dat);
ADDIU	R2, SP, 20
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ORI	R26, R0, 52
ORI	R25, R0, 40
JAL	_MPU_I2C_Read+0
NOP	
;BNO55_I2C_driver.c,188 :: 		RAW_DATA_TEMPERATUR  = (short) dat;        // 1°C = 1 LSB
LB	R2, 20(SP)
SB	R2, Offset(_RAW_DATA_TEMPERATUR+0)(GP)
;BNO55_I2C_driver.c,189 :: 		}
L_end_BNO55_ReadTemp:
LW	R28, 16(SP)
LW	R27, 12(SP)
LW	R26, 8(SP)
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 24
JR	RA
NOP	
; end of BNO55_I2C_driver_BNO55_ReadTemp
_BNO55_Read:
;BNO55_I2C_driver.c,191 :: 		void BNO55_Read() {
ADDIU	SP, SP, -4
SW	RA, 0(SP)
;BNO55_I2C_driver.c,192 :: 		BNO55_ReadTemp();
JAL	BNO55_I2C_driver_BNO55_ReadTemp+0
NOP	
;BNO55_I2C_driver.c,194 :: 		BNO55_ReadEuler();
JAL	BNO55_I2C_driver_BNO55_ReadEuler+0
NOP	
;BNO55_I2C_driver.c,201 :: 		}
L_end_BNO55_Read:
LW	RA, 0(SP)
ADDIU	SP, SP, 4
JR	RA
NOP	
; end of _BNO55_Read
_Print_BNO55_Data:
;BNO55_I2C_driver.c,204 :: 		void Print_BNO55_Data()  {
ADDIU	SP, SP, -260
SW	RA, 0(SP)
;BNO55_I2C_driver.c,217 :: 		BNO55_Read();
SW	R25, 4(SP)
JAL	_BNO55_Read+0
NOP	
;BNO55_I2C_driver.c,222 :: 		heading_float = (float)BNO55.euler.Heading;     // S 35 . da  1degree = 16 LSB
LH	R4, Offset(_BNO55+18)(GP)
JAL	__SignedIntegralToFLoat+0
NOP	
; heading_float start address is: 36 (R9)
MOVZ	R9, R2, R0
;BNO55_I2C_driver.c,223 :: 		roll_float    = (float)BNO55.euler.Roll   ;
LH	R4, Offset(_BNO55+20)(GP)
JAL	__SignedIntegralToFLoat+0
NOP	
; roll_float start address is: 68 (R17)
MOVZ	R17, R2, R0
;BNO55_I2C_driver.c,224 :: 		pitch_float   = (float)BNO55.euler.Pitch  ;
LH	R4, Offset(_BNO55+22)(GP)
JAL	__SignedIntegralToFLoat+0
NOP	
; pitch_float start address is: 12 (R3)
MOVZ	R3, R2, R0
;BNO55_I2C_driver.c,226 :: 		heading_float =  heading_float /16.0;
LUI	R6, 16768
ORI	R6, R6, 0
MOVZ	R4, R9, R0
JAL	__Div_FP+0
NOP	
; heading_float end address is: 36 (R9)
; heading_float start address is: 64 (R16)
MOVZ	R16, R2, R0
;BNO55_I2C_driver.c,227 :: 		roll_float    =  roll_float    /16.0;
LUI	R6, 16768
ORI	R6, R6, 0
MOVZ	R4, R17, R0
JAL	__Div_FP+0
NOP	
; roll_float end address is: 68 (R17)
; roll_float start address is: 84 (R21)
MOVZ	R21, R2, R0
;BNO55_I2C_driver.c,228 :: 		pitch_float   =  pitch_float   /16.0;
LUI	R6, 16768
ORI	R6, R6, 0
MOVZ	R4, R3, R0
JAL	__Div_FP+0
NOP	
; pitch_float end address is: 12 (R3)
; pitch_float start address is: 88 (R22)
MOVZ	R22, R2, R0
;BNO55_I2C_driver.c,238 :: 		sprintf(heading," Heading = %1.0f" , heading_float);
ADDIU	R3, SP, 8
ADDIU	SP, SP, -12
SW	R16, 8(SP)
; heading_float end address is: 64 (R16)
LUI	R2, hi_addr(?lstr_6_BNO55_I2C_driver+0)
ORI	R2, R2, lo_addr(?lstr_6_BNO55_I2C_driver+0)
SW	R2, 4(SP)
SW	R3, 0(SP)
JAL	_sprintf+0
NOP	
ADDIU	SP, SP, 12
;BNO55_I2C_driver.c,239 :: 		sprintf(roll   ," Roll = %1.0f"    , roll_float);
ADDIU	R3, SP, 58
ADDIU	SP, SP, -12
SW	R21, 8(SP)
; roll_float end address is: 84 (R21)
LUI	R2, hi_addr(?lstr_7_BNO55_I2C_driver+0)
ORI	R2, R2, lo_addr(?lstr_7_BNO55_I2C_driver+0)
SW	R2, 4(SP)
SW	R3, 0(SP)
JAL	_sprintf+0
NOP	
ADDIU	SP, SP, 12
;BNO55_I2C_driver.c,240 :: 		sprintf(pitch  ," Pitch = %1.0f"   , pitch_float );
ADDIU	R3, SP, 108
ADDIU	SP, SP, -12
SW	R22, 8(SP)
; pitch_float end address is: 88 (R22)
LUI	R2, hi_addr(?lstr_8_BNO55_I2C_driver+0)
ORI	R2, R2, lo_addr(?lstr_8_BNO55_I2C_driver+0)
SW	R2, 4(SP)
SW	R3, 0(SP)
JAL	_sprintf+0
NOP	
ADDIU	SP, SP, 12
;BNO55_I2C_driver.c,272 :: 		UART2_Write_Text(stn_out);
ADDIU	R2, SP, 158
MOVZ	R25, R2, R0
JAL	_UART2_Write_Text+0
NOP	
;BNO55_I2C_driver.c,288 :: 		UART2_Write_Text(heading);
ADDIU	R2, SP, 8
MOVZ	R25, R2, R0
JAL	_UART2_Write_Text+0
NOP	
;BNO55_I2C_driver.c,289 :: 		UART2_Write_Text(roll);
ADDIU	R2, SP, 58
MOVZ	R25, R2, R0
JAL	_UART2_Write_Text+0
NOP	
;BNO55_I2C_driver.c,290 :: 		UART2_Write_Text(pitch);
ADDIU	R2, SP, 108
MOVZ	R25, R2, R0
JAL	_UART2_Write_Text+0
NOP	
;BNO55_I2C_driver.c,293 :: 		UART2_Write(0x0D);
ORI	R25, R0, 13
JAL	_UART2_Write+0
NOP	
;BNO55_I2C_driver.c,295 :: 		}
L_end_Print_BNO55_Data:
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 260
JR	RA
NOP	
; end of _Print_BNO55_Data
_Print_MPU_register_map:
;BNO55_I2C_driver.c,298 :: 		void Print_MPU_register_map(int startAdress, int EndAdress){
ADDIU	SP, SP, -140
SW	RA, 0(SP)
;BNO55_I2C_driver.c,303 :: 		for(registerAdress = startAdress; registerAdress <= EndAdress; registerAdress++){
SW	R27, 4(SP)
SW	R28, 8(SP)
; registerAdress start address is: 24 (R6)
SEH	R6, R25
; registerAdress end address is: 24 (R6)
L_Print_MPU_register_map17:
; registerAdress start address is: 24 (R6)
ANDI	R3, R6, 255
SEH	R2, R26
SLT	R2, R2, R3
BEQ	R2, R0, L__Print_MPU_register_map52
NOP	
J	L_Print_MPU_register_map18
NOP	
L__Print_MPU_register_map52:
;BNO55_I2C_driver.c,305 :: 		MPU_I2C_Read(BNO55_I2C_ADDR, registerAdress , 1, &dat);
ADDIU	R2, SP, 16
SH	R26, 12(SP)
SH	R25, 14(SP)
MOVZ	R28, R2, R0
ORI	R27, R0, 1
ANDI	R26, R6, 255
ORI	R25, R0, 40
JAL	_MPU_I2C_Read+0
NOP	
LH	R25, 14(SP)
LH	R26, 12(SP)
;BNO55_I2C_driver.c,307 :: 		mask = 0b00000001;
; mask start address is: 20 (R5)
ORI	R5, R0, 1
;BNO55_I2C_driver.c,308 :: 		for( i = 7; i>=0; i--){
; i start address is: 16 (R4)
ORI	R4, R0, 7
; mask end address is: 20 (R5)
; i end address is: 16 (R4)
; registerAdress end address is: 24 (R6)
L_Print_MPU_register_map20:
; i start address is: 16 (R4)
; mask start address is: 20 (R5)
; registerAdress start address is: 24 (R6)
SEH	R2, R4
SLTI	R2, R2, 0
BEQ	R2, R0, L__Print_MPU_register_map53
NOP	
J	L_Print_MPU_register_map21
NOP	
L__Print_MPU_register_map53:
;BNO55_I2C_driver.c,309 :: 		dat_temp = dat & mask;
LBU	R2, 16(SP)
AND	R2, R2, R5
;BNO55_I2C_driver.c,310 :: 		if(dat_temp > 0)
ANDI	R2, R2, 255
SLTIU	R2, R2, 1
BEQ	R2, R0, L__Print_MPU_register_map54
NOP	
J	L_Print_MPU_register_map23
NOP	
L__Print_MPU_register_map54:
;BNO55_I2C_driver.c,311 :: 		bit_string[i] = '1';
ADDIU	R3, SP, 117
SEH	R2, R4
ADDU	R3, R3, R2
ORI	R2, R0, 49
SB	R2, 0(R3)
J	L_Print_MPU_register_map24
NOP	
L_Print_MPU_register_map23:
;BNO55_I2C_driver.c,313 :: 		bit_string[i] = '0';
ADDIU	R3, SP, 117
SEH	R2, R4
ADDU	R3, R3, R2
ORI	R2, R0, 48
SB	R2, 0(R3)
L_Print_MPU_register_map24:
;BNO55_I2C_driver.c,314 :: 		mask = mask << 1;
ANDI	R2, R5, 255
; mask end address is: 20 (R5)
SLL	R2, R2, 1
; mask start address is: 20 (R5)
ANDI	R5, R2, 255
;BNO55_I2C_driver.c,308 :: 		for( i = 7; i>=0; i--){
ADDIU	R2, R4, -1
SEH	R4, R2
;BNO55_I2C_driver.c,315 :: 		}
; mask end address is: 20 (R5)
; i end address is: 16 (R4)
J	L_Print_MPU_register_map20
NOP	
L_Print_MPU_register_map21:
;BNO55_I2C_driver.c,316 :: 		bit_string[9] = 0;
ADDIU	R2, SP, 117
ADDIU	R2, R2, 9
SB	R0, 0(R2)
;BNO55_I2C_driver.c,318 :: 		strcpy(stn_out,"Adresse =");
ADDIU	R2, SP, 17
SH	R26, 12(SP)
SH	R25, 14(SP)
LUI	R26, hi_addr(?lstr9_BNO55_I2C_driver+0)
ORI	R26, R26, lo_addr(?lstr9_BNO55_I2C_driver+0)
MOVZ	R25, R2, R0
JAL	_strcpy+0
NOP	
;BNO55_I2C_driver.c,319 :: 		ShortToStr( (short)registerAdress, adress_string);
ADDIU	R2, SP, 127
MOVZ	R26, R2, R0
SEB	R25, R6
JAL	_ShortToStr+0
NOP	
;BNO55_I2C_driver.c,320 :: 		strcat(stn_out,adress_string);
ADDIU	R3, SP, 127
ADDIU	R2, SP, 17
MOVZ	R26, R3, R0
MOVZ	R25, R2, R0
JAL	_strcat+0
NOP	
;BNO55_I2C_driver.c,321 :: 		ShortToHex((unsigned short)registerAdress, adress_string);
ADDIU	R2, SP, 127
MOVZ	R26, R2, R0
ANDI	R25, R6, 255
JAL	_ShortToHex+0
NOP	
;BNO55_I2C_driver.c,322 :: 		strcat(stn_out," (0x");
ADDIU	R2, SP, 17
LUI	R26, hi_addr(?lstr10_BNO55_I2C_driver+0)
ORI	R26, R26, lo_addr(?lstr10_BNO55_I2C_driver+0)
MOVZ	R25, R2, R0
JAL	_strcat+0
NOP	
;BNO55_I2C_driver.c,323 :: 		strcat(stn_out,adress_string);
ADDIU	R3, SP, 127
ADDIU	R2, SP, 17
MOVZ	R26, R3, R0
MOVZ	R25, R2, R0
JAL	_strcat+0
NOP	
;BNO55_I2C_driver.c,324 :: 		strcat(stn_out,")  Inhalt = 0b");
ADDIU	R2, SP, 17
LUI	R26, hi_addr(?lstr11_BNO55_I2C_driver+0)
ORI	R26, R26, lo_addr(?lstr11_BNO55_I2C_driver+0)
MOVZ	R25, R2, R0
JAL	_strcat+0
NOP	
;BNO55_I2C_driver.c,325 :: 		strcat(stn_out,bit_string);
ADDIU	R3, SP, 117
ADDIU	R2, SP, 17
MOVZ	R26, R3, R0
MOVZ	R25, R2, R0
JAL	_strcat+0
NOP	
;BNO55_I2C_driver.c,326 :: 		strcat(stn_out,"\x0D");
ADDIU	R2, SP, 17
LUI	R26, hi_addr(?lstr12_BNO55_I2C_driver+0)
ORI	R26, R26, lo_addr(?lstr12_BNO55_I2C_driver+0)
MOVZ	R25, R2, R0
JAL	_strcat+0
NOP	
LH	R25, 14(SP)
LH	R26, 12(SP)
;BNO55_I2C_driver.c,328 :: 		UART2_write_text(stn_out);
ADDIU	R2, SP, 17
SH	R25, 12(SP)
MOVZ	R25, R2, R0
JAL	_UART2_Write_Text+0
NOP	
LH	R25, 12(SP)
;BNO55_I2C_driver.c,330 :: 		delay_ms(20);
LUI	R24, 8
ORI	R24, R24, 9044
L_Print_MPU_register_map25:
ADDIU	R24, R24, -1
BNE	R24, R0, L_Print_MPU_register_map25
NOP	
NOP	
NOP	
;BNO55_I2C_driver.c,303 :: 		for(registerAdress = startAdress; registerAdress <= EndAdress; registerAdress++){
ADDIU	R2, R6, 1
ANDI	R6, R2, 255
;BNO55_I2C_driver.c,331 :: 		}
; registerAdress end address is: 24 (R6)
J	L_Print_MPU_register_map17
NOP	
L_Print_MPU_register_map18:
;BNO55_I2C_driver.c,332 :: 		}
L_end_Print_MPU_register_map:
LW	R28, 8(SP)
LW	R27, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 140
JR	RA
NOP	
; end of _Print_MPU_register_map
_MPU_I2C_Write:
;BNO55_I2C_driver.c,339 :: 		void MPU_I2C_Write(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
ADDIU	SP, SP, -12
SW	RA, 0(SP)
;BNO55_I2C_driver.c,343 :: 		s_addr_internW = (s_addr << 1) & 0xFE;     // shift der Adresse um 1 Bit nach links da für Adresse nur obersten 7 Bits verwendet werden + Write 0bit an stelle 0
SW	R25, 4(SP)
ANDI	R2, R25, 255
SLL	R2, R2, 1
ANDI	R2, R2, 254
; s_addr_internW start address is: 16 (R4)
ANDI	R4, R2, 255
;BNO55_I2C_driver.c,345 :: 		I2C2_Start();                         // issue I2C start signal
JAL	_I2C2_Start+0
NOP	
;BNO55_I2C_driver.c,346 :: 		I2C2_Write(s_addr_internW);            // send byte via I2C  (device address + W(&0xFE))
ANDI	R25, R4, 255
; s_addr_internW end address is: 16 (R4)
JAL	_I2C2_Write+0
NOP	
;BNO55_I2C_driver.c,347 :: 		I2C2_Write(r_addr);                   // send byte (address of EEPROM location)
ANDI	R25, R26, 255
JAL	_I2C2_Write+0
NOP	
;BNO55_I2C_driver.c,348 :: 		for (i = 0 ; i < len ; i++){
; i start address is: 16 (R4)
MOVZ	R4, R0, R0
; i end address is: 16 (R4)
L_MPU_I2C_Write27:
; i start address is: 16 (R4)
ANDI	R3, R4, 65535
ANDI	R2, R27, 255
SLTU	R2, R3, R2
BNE	R2, R0, L__MPU_I2C_Write56
NOP	
J	L_MPU_I2C_Write28
NOP	
L__MPU_I2C_Write56:
;BNO55_I2C_driver.c,349 :: 		I2C2_Write(*dat++);                 // send data (data to be written)
SB	R25, 8(SP)
LBU	R25, 0(R28)
JAL	_I2C2_Write+0
NOP	
LBU	R25, 8(SP)
ADDIU	R2, R28, 1
MOVZ	R28, R2, R0
;BNO55_I2C_driver.c,348 :: 		for (i = 0 ; i < len ; i++){
ADDIU	R2, R4, 1
ANDI	R4, R2, 65535
;BNO55_I2C_driver.c,350 :: 		}
; i end address is: 16 (R4)
J	L_MPU_I2C_Write27
NOP	
L_MPU_I2C_Write28:
;BNO55_I2C_driver.c,351 :: 		I2C2_Stop();                          // issue I2C stop signal
JAL	_I2C2_Stop+0
NOP	
;BNO55_I2C_driver.c,352 :: 		}
L_end_MPU_I2C_Write:
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 12
JR	RA
NOP	
; end of _MPU_I2C_Write
_MPU_I2C_Read:
;BNO55_I2C_driver.c,354 :: 		void MPU_I2C_Read(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
ADDIU	SP, SP, -12
SW	RA, 0(SP)
;BNO55_I2C_driver.c,358 :: 		s_addr_internW = (s_addr << 1) & 0xFE;     // shift der Adresse um 1 Bit nach links da für Adresse nur obersten 7 Bits verwendet werden + Write 0bit an stelle 0
SW	R25, 4(SP)
ANDI	R2, R25, 255
SLL	R3, R2, 1
ANDI	R2, R3, 254
; s_addr_internW start address is: 16 (R4)
ANDI	R4, R2, 255
;BNO55_I2C_driver.c,359 :: 		s_addr_internR = (s_addr << 1) | 0x01;     // shift der Adresse um 1 Bit nach links da für Adresse nur obersten 7 Bits verwendet werden + Read  1bit an stelle 0
ORI	R2, R3, 1
; s_addr_internR start address is: 20 (R5)
ANDI	R5, R2, 255
;BNO55_I2C_driver.c,361 :: 		I2C2_Start();                         // issue I2C start signal
JAL	_I2C2_Start+0
NOP	
;BNO55_I2C_driver.c,362 :: 		I2C2_Write( s_addr_internW);            // send byte via I2C  (device address + W(&0xFE))
ANDI	R25, R4, 255
; s_addr_internW end address is: 16 (R4)
JAL	_I2C2_Write+0
NOP	
;BNO55_I2C_driver.c,363 :: 		I2C2_Write(r_addr);                   // send byte (data address)
ANDI	R25, R26, 255
JAL	_I2C2_Write+0
NOP	
;BNO55_I2C_driver.c,364 :: 		I2C2_Restart();                       // issue I2C signal repeated start
JAL	_I2C2_Restart+0
NOP	
;BNO55_I2C_driver.c,365 :: 		I2C2_Write(s_addr_internR);            // send byte (device address + R(|0x01))
ANDI	R25, R5, 255
; s_addr_internR end address is: 20 (R5)
JAL	_I2C2_Write+0
NOP	
;BNO55_I2C_driver.c,366 :: 		for (i = 0; i < (len-1); i++){
; i start address is: 16 (R4)
MOVZ	R4, R0, R0
; i end address is: 16 (R4)
L_MPU_I2C_Read30:
; i start address is: 16 (R4)
ANDI	R2, R27, 255
ADDIU	R2, R2, -1
ANDI	R3, R4, 65535
SEH	R2, R2
SLTU	R2, R3, R2
BNE	R2, R0, L__MPU_I2C_Read58
NOP	
J	L_MPU_I2C_Read31
NOP	
L__MPU_I2C_Read58:
;BNO55_I2C_driver.c,367 :: 		*dat++ = I2C2_Read(_I2C_ACK);      // Read the data (acknowledge)
SB	R25, 8(SP)
MOVZ	R25, R0, R0
JAL	_I2C2_Read+0
NOP	
LBU	R25, 8(SP)
SB	R2, 0(R28)
ADDIU	R2, R28, 1
MOVZ	R28, R2, R0
;BNO55_I2C_driver.c,366 :: 		for (i = 0; i < (len-1); i++){
ADDIU	R2, R4, 1
ANDI	R4, R2, 65535
;BNO55_I2C_driver.c,368 :: 		}
; i end address is: 16 (R4)
J	L_MPU_I2C_Read30
NOP	
L_MPU_I2C_Read31:
;BNO55_I2C_driver.c,369 :: 		*dat = I2C2_Read(_I2C_NACK);         // Read the data (NO acknowledge)
SB	R25, 8(SP)
ORI	R25, R0, 1
JAL	_I2C2_Read+0
NOP	
LBU	R25, 8(SP)
SB	R2, 0(R28)
;BNO55_I2C_driver.c,370 :: 		I2C2_Stop();
JAL	_I2C2_Stop+0
NOP	
;BNO55_I2C_driver.c,372 :: 		}
L_end_MPU_I2C_Read:
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 12
JR	RA
NOP	
; end of _MPU_I2C_Read
_MPU_I2C_Read_Multi:
;BNO55_I2C_driver.c,375 :: 		void MPU_I2C_Read_Multi(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
ADDIU	SP, SP, -12
SW	RA, 0(SP)
;BNO55_I2C_driver.c,379 :: 		s_addr_internW = (s_addr << 1) & 0xFE;     // shift der Adresse um 1 Bit nach links da für Adresse nur obersten 7 Bits verwendet werden + Write 0bit an stelle 0
SW	R25, 4(SP)
ANDI	R2, R25, 255
SLL	R3, R2, 1
ANDI	R2, R3, 254
; s_addr_internW start address is: 16 (R4)
ANDI	R4, R2, 255
;BNO55_I2C_driver.c,380 :: 		s_addr_internR = (s_addr << 1) | 0x01;     // shift der Adresse um 1 Bit nach links da für Adresse nur obersten 7 Bits verwendet werden + Read  1bit an stelle 0
ORI	R2, R3, 1
; s_addr_internR start address is: 20 (R5)
ANDI	R5, R2, 255
;BNO55_I2C_driver.c,382 :: 		I2C2_Start();                         // issue I2C start signal
JAL	_I2C2_Start+0
NOP	
;BNO55_I2C_driver.c,383 :: 		I2C2_Write( s_addr_internW);            // send byte via I2C  (device address + W(&0xFE))
ANDI	R25, R4, 255
; s_addr_internW end address is: 16 (R4)
JAL	_I2C2_Write+0
NOP	
;BNO55_I2C_driver.c,384 :: 		I2C2_Write(r_addr);                   // send byte (data address)
ANDI	R25, R26, 255
JAL	_I2C2_Write+0
NOP	
;BNO55_I2C_driver.c,385 :: 		I2C2_Restart();                       // issue I2C signal repeated start
JAL	_I2C2_Restart+0
NOP	
;BNO55_I2C_driver.c,386 :: 		I2C2_Write(s_addr_internR);            // send byte (device address + R(|0x01))
ANDI	R25, R5, 255
; s_addr_internR end address is: 20 (R5)
JAL	_I2C2_Write+0
NOP	
;BNO55_I2C_driver.c,387 :: 		for (i = 0; i < (len-1); i++){
; i start address is: 16 (R4)
MOVZ	R4, R0, R0
; i end address is: 16 (R4)
L_MPU_I2C_Read_Multi33:
; i start address is: 16 (R4)
ANDI	R2, R27, 255
ADDIU	R2, R2, -1
ANDI	R3, R4, 65535
SEH	R2, R2
SLTU	R2, R3, R2
BNE	R2, R0, L__MPU_I2C_Read_Multi60
NOP	
J	L_MPU_I2C_Read_Multi34
NOP	
L__MPU_I2C_Read_Multi60:
;BNO55_I2C_driver.c,388 :: 		*dat = I2C2_Read(_I2C_ACK);      // Read the data (acknowledge)
SB	R25, 8(SP)
MOVZ	R25, R0, R0
JAL	_I2C2_Read+0
NOP	
LBU	R25, 8(SP)
SB	R2, 0(R28)
;BNO55_I2C_driver.c,389 :: 		dat++;
ADDIU	R2, R28, 1
MOVZ	R28, R2, R0
;BNO55_I2C_driver.c,387 :: 		for (i = 0; i < (len-1); i++){
ADDIU	R2, R4, 1
ANDI	R4, R2, 65535
;BNO55_I2C_driver.c,390 :: 		}
; i end address is: 16 (R4)
J	L_MPU_I2C_Read_Multi33
NOP	
L_MPU_I2C_Read_Multi34:
;BNO55_I2C_driver.c,391 :: 		*dat = I2C2_Read(_I2C_NACK);         // Read the data (NO acknowledge)
SB	R25, 8(SP)
ORI	R25, R0, 1
JAL	_I2C2_Read+0
NOP	
LBU	R25, 8(SP)
SB	R2, 0(R28)
;BNO55_I2C_driver.c,392 :: 		I2C2_Stop();
JAL	_I2C2_Stop+0
NOP	
;BNO55_I2C_driver.c,393 :: 		}
L_end_MPU_I2C_Read_Multi:
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 12
JR	RA
NOP	
; end of _MPU_I2C_Read_Multi
