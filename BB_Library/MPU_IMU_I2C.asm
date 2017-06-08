_MPU_I2C_Write:
;MPU_IMU_I2C.c,3 :: 		void MPU_I2C_Write(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
    ADDIU	SP, SP, -12
    SW	RA, 0(SP)
;MPU_IMU_I2C.c,6 :: 		I2C2_Start();                         // issue I2C start signal
    SW	R25, 4(SP)
    JAL	_I2C2_Start+0
    NOP	
;MPU_IMU_I2C.c,7 :: 		I2C2_Write(s_addr & 0xFE);            // send byte via I2C  (device address + W(&0xFE))
    ANDI	R2, R25, 254
    ANDI	R25, R2, 255
    JAL	_I2C2_Write+0
    NOP	
;MPU_IMU_I2C.c,8 :: 		I2C2_Write(r_addr);                   // send byte (address of EEPROM location)
    ANDI	R25, R26, 255
    JAL	_I2C2_Write+0
    NOP	
;MPU_IMU_I2C.c,9 :: 		for (i = 0 ; i < len ; i++){
; i start address is: 16 (R4)
    MOVZ	R4, R0, R0
; i end address is: 16 (R4)
L_MPU_I2C_Write0:
; i start address is: 16 (R4)
    ANDI	R3, R4, 65535
    ANDI	R2, R27, 255
    SLTU	R2, R3, R2
    BNE	R2, R0, L__MPU_I2C_Write12
    NOP	
    J	L_MPU_I2C_Write1
    NOP	
L__MPU_I2C_Write12:
;MPU_IMU_I2C.c,10 :: 		I2C2_Write(*dat++);                 // send data (data to be written)
    SB	R25, 8(SP)
    LBU	R25, 0(R28)
    JAL	_I2C2_Write+0
    NOP	
    LBU	R25, 8(SP)
    ADDIU	R2, R28, 1
    MOVZ	R28, R2, R0
;MPU_IMU_I2C.c,9 :: 		for (i = 0 ; i < len ; i++){
    ADDIU	R2, R4, 1
    ANDI	R4, R2, 65535
;MPU_IMU_I2C.c,11 :: 		}
; i end address is: 16 (R4)
    J	L_MPU_I2C_Write0
    NOP	
L_MPU_I2C_Write1:
;MPU_IMU_I2C.c,12 :: 		I2C2_Stop();                          // issue I2C stop signal
    JAL	_I2C2_Stop+0
    NOP	
;MPU_IMU_I2C.c,13 :: 		}
L_end_MPU_I2C_Write:
    LW	R25, 4(SP)
    LW	RA, 0(SP)
    ADDIU	SP, SP, 12
    JR	RA
    NOP	
; end of _MPU_I2C_Write
_MPU_I2C_Read:
;MPU_IMU_I2C.c,17 :: 		void MPU_I2C_Read(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
    ADDIU	SP, SP, -12
    SW	RA, 0(SP)
;MPU_IMU_I2C.c,19 :: 		I2C2_Start();                         // issue I2C start signal
    SW	R25, 4(SP)
    JAL	_I2C2_Start+0
    NOP	
;MPU_IMU_I2C.c,20 :: 		I2C2_Write(s_addr & 0xFE);            // send byte via I2C  (device address + W(&0xFE))
    ANDI	R2, R25, 254
    SB	R25, 8(SP)
    ANDI	R25, R2, 255
    JAL	_I2C2_Write+0
    NOP	
;MPU_IMU_I2C.c,21 :: 		I2C2_Write(r_addr);                   // send byte (data address)
    ANDI	R25, R26, 255
    JAL	_I2C2_Write+0
    NOP	
    LBU	R25, 8(SP)
;MPU_IMU_I2C.c,22 :: 		I2C2_Restart();                       // issue I2C signal repeated start
    JAL	_I2C2_Restart+0
    NOP	
;MPU_IMU_I2C.c,23 :: 		I2C2_Write(s_addr | 0x01);            // send byte (device address + R(|0x01))
    ORI	R2, R25, 1
    ANDI	R25, R2, 255
    JAL	_I2C2_Write+0
    NOP	
;MPU_IMU_I2C.c,24 :: 		for (i = 0; i < (len-1); i++){
; i start address is: 16 (R4)
    MOVZ	R4, R0, R0
; i end address is: 16 (R4)
L_MPU_I2C_Read3:
; i start address is: 16 (R4)
    ANDI	R2, R27, 255
    ADDIU	R2, R2, -1
    ANDI	R3, R4, 65535
    SEH	R2, R2
    SLTU	R2, R3, R2
    BNE	R2, R0, L__MPU_I2C_Read14
    NOP	
    J	L_MPU_I2C_Read4
    NOP	
L__MPU_I2C_Read14:
;MPU_IMU_I2C.c,25 :: 		*dat++ = I2C2_Read(_I2C_ACK);      // Read the data (acknowledge)
    SB	R25, 8(SP)
    MOVZ	R25, R0, R0
    JAL	_I2C2_Read+0
    NOP	
    LBU	R25, 8(SP)
    SB	R2, 0(R28)
    ADDIU	R2, R28, 1
    MOVZ	R28, R2, R0
;MPU_IMU_I2C.c,24 :: 		for (i = 0; i < (len-1); i++){
    ADDIU	R2, R4, 1
    ANDI	R4, R2, 65535
;MPU_IMU_I2C.c,26 :: 		}
; i end address is: 16 (R4)
    J	L_MPU_I2C_Read3
    NOP	
L_MPU_I2C_Read4:
;MPU_IMU_I2C.c,27 :: 		*dat = I2C2_Read(_I2C_NACK);         // Read the data (NO acknowledge)
    SB	R25, 8(SP)
    ORI	R25, R0, 1
    JAL	_I2C2_Read+0
    NOP	
    LBU	R25, 8(SP)
    SB	R2, 0(R28)
;MPU_IMU_I2C.c,28 :: 		I2C2_Stop();                         // issue I2C stop signal
    JAL	_I2C2_Stop+0
    NOP	
;MPU_IMU_I2C.c,29 :: 		}
L_end_MPU_I2C_Read:
    LW	R25, 4(SP)
    LW	RA, 0(SP)
    ADDIU	SP, SP, 12
    JR	RA
    NOP	
; end of _MPU_I2C_Read
_MPU_I2C_Read_Int:
;MPU_IMU_I2C.c,31 :: 		void MPU_I2C_Read_Int(unsigned char s_addr, unsigned char r_addr, unsigned char len, unsigned char *dat) {
    ADDIU	SP, SP, -20
    SW	RA, 0(SP)
;MPU_IMU_I2C.c,34 :: 		I2C2_Start();                         // issue I2C start signal
    SW	R25, 4(SP)
    JAL	_I2C2_Start+0
    NOP	
;MPU_IMU_I2C.c,35 :: 		I2C2_Write(s_addr & 0xFE);            // send byte via I2C  (device address + W(&0xFE))
    ANDI	R2, R25, 254
    SB	R25, 8(SP)
    ANDI	R25, R2, 255
    JAL	_I2C2_Write+0
    NOP	
;MPU_IMU_I2C.c,36 :: 		I2C2_Write(r_addr);                      // send byte (data address)
    ANDI	R25, R26, 255
    JAL	_I2C2_Write+0
    NOP	
    LBU	R25, 8(SP)
;MPU_IMU_I2C.c,37 :: 		I2C2_Restart();                       // issue I2C signal repeated start
    JAL	_I2C2_Restart+0
    NOP	
;MPU_IMU_I2C.c,38 :: 		I2C2_Write(s_addr | 0x01);            // send byte (device address + R(|0x01))
    ORI	R2, R25, 1
    ANDI	R25, R2, 255
    JAL	_I2C2_Write+0
    NOP	
;MPU_IMU_I2C.c,39 :: 		for (i = 0 ; i < ((len << 1)-1) ; i++){
; i start address is: 16 (R4)
    MOVZ	R4, R0, R0
; i end address is: 16 (R4)
L_MPU_I2C_Read_Int6:
; i start address is: 16 (R4)
    ANDI	R2, R27, 255
    SLL	R2, R2, 1
    ADDIU	R2, R2, -1
    ANDI	R3, R4, 65535
    ANDI	R2, R2, 65535
    SLTU	R2, R3, R2
    BNE	R2, R0, L__MPU_I2C_Read_Int16
    NOP	
    J	L_MPU_I2C_Read_Int7
    NOP	
L__MPU_I2C_Read_Int16:
;MPU_IMU_I2C.c,40 :: 		if (i%2) {
    ANDI	R2, R4, 1
    BNE	R2, R0, L__MPU_I2C_Read_Int18
    NOP	
    J	L_MPU_I2C_Read_Int9
    NOP	
L__MPU_I2C_Read_Int18:
;MPU_IMU_I2C.c,41 :: 		pt = pt - 1;
    LW	R2, 12(SP)
    ADDIU	R2, R2, -1
    SW	R2, 12(SP)
;MPU_IMU_I2C.c,42 :: 		}
    J	L_MPU_I2C_Read_Int10
    NOP	
L_MPU_I2C_Read_Int9:
;MPU_IMU_I2C.c,44 :: 		pt = dat + i + 1;
    ANDI	R2, R4, 65535
    ADDU	R2, R28, R2
    ADDIU	R2, R2, 1
    SW	R2, 12(SP)
;MPU_IMU_I2C.c,45 :: 		}
L_MPU_I2C_Read_Int10:
;MPU_IMU_I2C.c,46 :: 		*pt = I2C2_Read(_I2C_ACK);          // Read the data (acknowledge)
    SB	R25, 8(SP)
    MOVZ	R25, R0, R0
    JAL	_I2C2_Read+0
    NOP	
    LBU	R25, 8(SP)
    LW	R3, 12(SP)
    SB	R2, 0(R3)
;MPU_IMU_I2C.c,39 :: 		for (i = 0 ; i < ((len << 1)-1) ; i++){
    ADDIU	R2, R4, 1
    ANDI	R4, R2, 65535
;MPU_IMU_I2C.c,47 :: 		}
; i end address is: 16 (R4)
    J	L_MPU_I2C_Read_Int6
    NOP	
L_MPU_I2C_Read_Int7:
;MPU_IMU_I2C.c,48 :: 		*(pt - 1) = I2C2_Read(_I2C_NACK);     // Read the data (NO acknowledge)
    LW	R2, 12(SP)
    ADDIU	R2, R2, -1
    SW	R2, 16(SP)
    SB	R25, 8(SP)
    ORI	R25, R0, 1
    JAL	_I2C2_Read+0
    NOP	
    LBU	R25, 8(SP)
    LW	R3, 16(SP)
    SB	R2, 0(R3)
;MPU_IMU_I2C.c,49 :: 		I2C2_Stop();                          // issue I2C stop signal
    JAL	_I2C2_Stop+0
    NOP	
;MPU_IMU_I2C.c,50 :: 		}
L_end_MPU_I2C_Read_Int:
    LW	R25, 4(SP)
    LW	RA, 0(SP)
    ADDIU	SP, SP, 20
    JR	RA
    NOP	
; end of _MPU_I2C_Read_Int
