_Init_Ext_Int1:
;MPU_IMU_PIC32_v001.c,51 :: 		void Init_Ext_Int1(){
;MPU_IMU_PIC32_v001.c,53 :: 		INTCONSET = 0x0006;                 // Setzen Polaritat auf Rising Edge in IntCon Register mit bits 1 & 2 auf 1 --> 0b00000000 00000000 00000000 00000110
ORI	R2, R0, 6
SW	R2, Offset(INTCONSET+0)(GP)
;MPU_IMU_PIC32_v001.c,55 :: 		INT1IP0_bit = 1;                   // Set INT2 interrupt
LUI	R2, BitMask(INT1IP0_bit+0)
ORI	R2, R2, BitMask(INT1IP0_bit+0)
_SX	
;MPU_IMU_PIC32_v001.c,56 :: 		INT1IP1_bit = 1;                   // Set interrupt priorities
LUI	R2, BitMask(INT1IP1_bit+0)
ORI	R2, R2, BitMask(INT1IP1_bit+0)
_SX	
;MPU_IMU_PIC32_v001.c,57 :: 		INT1IP2_bit = 1;                   // Set inrrupt priority to 4
LUI	R2, BitMask(INT1IP2_bit+0)
ORI	R2, R2, BitMask(INT1IP2_bit+0)
_SX	
;MPU_IMU_PIC32_v001.c,58 :: 		INT1IE_bit  = 1;                   // Set interrupt on INT2 (RE9) to be enabled
LUI	R2, BitMask(INT1IE_bit+0)
ORI	R2, R2, BitMask(INT1IE_bit+0)
_SX	
;MPU_IMU_PIC32_v001.c,59 :: 		INT1IF_bit  = 0;
LUI	R2, BitMask(INT1IF_bit+0)
ORI	R2, R2, BitMask(INT1IF_bit+0)
_SX	
;MPU_IMU_PIC32_v001.c,70 :: 		}
L_end_Init_Ext_Int1:
JR	RA
NOP	
; end of _Init_Ext_Int1
_Interrupt_EXTERN_1:
;MPU_IMU_PIC32_v001.c,72 :: 		void Interrupt_EXTERN_1() iv IVT_EXTERNAL_1 ilevel 7 ics ICS_SRS {
RDPGPR	SP, SP
ADDIU	SP, SP, -12
MFC0	R30, 12, 2
SW	R30, 8(SP)
MFC0	R30, 14, 0
SW	R30, 4(SP)
MFC0	R30, 12, 0
SW	R30, 0(SP)
INS	R30, R0, 1, 15
ORI	R30, R0, 7168
MTC0	R30, 12, 0
;MPU_IMU_PIC32_v001.c,73 :: 		IMU_interrupt_bit = 1;
LBU	R2, Offset(_IMU_interrupt_bit+0)(GP)
ORI	R2, R2, BitMask(_IMU_interrupt_bit+0)
SB	R2, Offset(_IMU_interrupt_bit+0)(GP)
;MPU_IMU_PIC32_v001.c,74 :: 		INT1IF_bit = 0;                    // Reset INT2 flag
LUI	R2, BitMask(INT1IF_bit+0)
ORI	R2, R2, BitMask(INT1IF_bit+0)
_SX	
;MPU_IMU_PIC32_v001.c,75 :: 		}
L_end_Interrupt_EXTERN_1:
DI	
EHB	
LW	R30, 8(SP)
MTC0	R30, 12, 2
LW	R30, 4(SP)
MTC0	R30, 14, 0
LW	R30, 0(SP)
MTC0	R30, 12, 0
ADDIU	SP, SP, 12
WRPGPR	SP, SP
ERET	
; end of _Interrupt_EXTERN_1
_Init_BNO55:
;MPU_IMU_PIC32_v001.c,78 :: 		void Init_BNO55(){
ADDIU	SP, SP, -12
SW	RA, 0(SP)
;MPU_IMU_PIC32_v001.c,81 :: 		RESET_IMU = 1;
SW	R25, 4(SP)
SW	R26, 8(SP)
LUI	R2, BitMask(RC1_bit+0)
ORI	R2, R2, BitMask(RC1_bit+0)
_SX	
;MPU_IMU_PIC32_v001.c,82 :: 		delay_ms(100);
LUI	R24, 40
ORI	R24, R24, 45226
L_Init_BNO550:
ADDIU	R24, R24, -1
BNE	R24, R0, L_Init_BNO550
NOP	
;MPU_IMU_PIC32_v001.c,83 :: 		RESET_IMU = 0;
LUI	R2, BitMask(RC1_bit+0)
ORI	R2, R2, BitMask(RC1_bit+0)
_SX	
;MPU_IMU_PIC32_v001.c,84 :: 		delay_ms(10);
LUI	R24, 4
ORI	R24, R24, 4522
L_Init_BNO552:
ADDIU	R24, R24, -1
BNE	R24, R0, L_Init_BNO552
NOP	
;MPU_IMU_PIC32_v001.c,85 :: 		RESET_IMU = 1;
LUI	R2, BitMask(RC1_bit+0)
ORI	R2, R2, BitMask(RC1_bit+0)
_SX	
;MPU_IMU_PIC32_v001.c,86 :: 		delay_ms(500);
LUI	R24, 203
ORI	R24, R24, 29524
L_Init_BNO554:
ADDIU	R24, R24, -1
BNE	R24, R0, L_Init_BNO554
NOP	
NOP	
NOP	
;MPU_IMU_PIC32_v001.c,88 :: 		result = BNO55_Detect();
JAL	_BNO55_Detect+0
NOP	
;MPU_IMU_PIC32_v001.c,89 :: 		if( result == 1)
SEB	R3, R2
ORI	R2, R0, 1
BEQ	R3, R2, L__Init_BNO5520
NOP	
J	L_Init_BNO556
NOP	
L__Init_BNO5520:
;MPU_IMU_PIC32_v001.c,90 :: 		UART2_write_text("BNO55 detected\x0D\x0D");
LUI	R25, hi_addr(?lstr1_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr1_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
J	L_Init_BNO557
NOP	
L_Init_BNO556:
;MPU_IMU_PIC32_v001.c,92 :: 		UART2_write_text("BNO55 NOT detected\x0D\x0D");
LUI	R25, hi_addr(?lstr2_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr2_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
L_Init_BNO557:
;MPU_IMU_PIC32_v001.c,94 :: 		Config_BNO55();
JAL	_Config_BNO55+0
NOP	
;MPU_IMU_PIC32_v001.c,100 :: 		UART2_write_text("Identification Register:\x0D");
LUI	R25, hi_addr(?lstr3_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr3_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,101 :: 		Print_MPU_register_map(0x00 , 0x00 );
MOVZ	R26, R0, R0
MOVZ	R25, R0, R0
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,103 :: 		UART2_write_text("TEMP_SOURCE Register:\x0D");
LUI	R25, hi_addr(?lstr4_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr4_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,104 :: 		Print_MPU_register_map(TEMP_SOURCE , TEMP_SOURCE );
ORI	R26, R0, 64
ORI	R25, R0, 64
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,105 :: 		UART2_write_text("SYS_TRIGGER Register:\x0D");
LUI	R25, hi_addr(?lstr5_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr5_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,106 :: 		Print_MPU_register_map(SYS_TRIGGER , SYS_TRIGGER );
ORI	R26, R0, 63
ORI	R25, R0, 63
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,107 :: 		UART2_write_text("PWR_MODE Register:\x0D");
LUI	R25, hi_addr(?lstr6_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr6_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,108 :: 		Print_MPU_register_map(PWR_MODE , PWR_MODE );
ORI	R26, R0, 62
ORI	R25, R0, 62
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,109 :: 		UART2_write_text("OPR_MODE Register:\x0D");
LUI	R25, hi_addr(?lstr7_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr7_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,110 :: 		Print_MPU_register_map(OPR_MODE , OPR_MODE );
ORI	R26, R0, 61
ORI	R25, R0, 61
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,111 :: 		UART2_write_text("UNIT_SEL Register:\x0D");
LUI	R25, hi_addr(?lstr8_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr8_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,112 :: 		Print_MPU_register_map(UNIT_SEL , UNIT_SEL );
ORI	R26, R0, 59
ORI	R25, R0, 59
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,113 :: 		UART2_write_text("GYR_Config_1 Register:\x0D");
LUI	R25, hi_addr(?lstr9_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr9_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,114 :: 		Print_MPU_register_map(GYR_Config_1 , GYR_Config_1 );
ORI	R26, R0, 11
ORI	R25, R0, 11
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,115 :: 		UART2_write_text("GYR_Config_0 Register:\x0D");
LUI	R25, hi_addr(?lstr10_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr10_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,116 :: 		Print_MPU_register_map(GYR_Config_0 , GYR_Config_0 );
ORI	R26, R0, 10
ORI	R25, R0, 10
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,117 :: 		UART2_write_text("MAG_Config Register:\x0D");
LUI	R25, hi_addr(?lstr11_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr11_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,118 :: 		Print_MPU_register_map(MAG_Config , MAG_Config );
ORI	R26, R0, 9
ORI	R25, R0, 9
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,119 :: 		UART2_write_text("ACC_Config Register:\x0D");
LUI	R25, hi_addr(?lstr12_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr12_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,120 :: 		Print_MPU_register_map(ACC_Config , ACC_Config );
ORI	R26, R0, 8
ORI	R25, R0, 8
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,121 :: 		UART2_write_text("INT_EN Register:\x0D");
LUI	R25, hi_addr(?lstr13_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr13_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,122 :: 		Print_MPU_register_map(INT_EN , INT_EN );
ORI	R26, R0, 16
ORI	R25, R0, 16
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,123 :: 		UART2_write_text("INT_MSK Register:\x0D");
LUI	R25, hi_addr(?lstr14_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr14_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,124 :: 		Print_MPU_register_map(INT_MSK , INT_MSK );
ORI	R26, R0, 15
ORI	R25, R0, 15
JAL	_Print_MPU_register_map+0
NOP	
;MPU_IMU_PIC32_v001.c,126 :: 		}
L_end_Init_BNO55:
LW	R26, 8(SP)
LW	R25, 4(SP)
LW	RA, 0(SP)
ADDIU	SP, SP, 12
JR	RA
NOP	
; end of _Init_BNO55
_main:
;MPU_IMU_PIC32_v001.c,130 :: 		void main() {
;MPU_IMU_PIC32_v001.c,133 :: 		AD1PCFG = 0xFFFF;                          // Configure AN pins as digital I/O
ORI	R2, R0, 65535
SW	R2, Offset(AD1PCFG+0)(GP)
;MPU_IMU_PIC32_v001.c,135 :: 		INT_IMU_Direction = 1;
LUI	R2, BitMask(TRISE0_bit+0)
ORI	R2, R2, BitMask(TRISE0_bit+0)
_SX	
;MPU_IMU_PIC32_v001.c,136 :: 		RESET_IMU_Direction = 0;
LUI	R2, BitMask(TRISC1_bit+0)
ORI	R2, R2, BitMask(TRISC1_bit+0)
_SX	
;MPU_IMU_PIC32_v001.c,139 :: 		I2C2_Init(100000);
LUI	R25, 1
ORI	R25, R25, 34464
JAL	_I2C2_Init+0
NOP	
;MPU_IMU_PIC32_v001.c,140 :: 		Delay_ms(100);
LUI	R24, 40
ORI	R24, R24, 45226
L_main8:
ADDIU	R24, R24, -1
BNE	R24, R0, L_main8
NOP	
;MPU_IMU_PIC32_v001.c,144 :: 		UART2_Init(115200);
LUI	R25, 1
ORI	R25, R25, 49664
JAL	_UART2_Init+0
NOP	
;MPU_IMU_PIC32_v001.c,145 :: 		Delay_ms(100);
LUI	R24, 40
ORI	R24, R24, 45226
L_main10:
ADDIU	R24, R24, -1
BNE	R24, R0, L_main10
NOP	
;MPU_IMU_PIC32_v001.c,146 :: 		UART2_write_text("uController started\x0D");
LUI	R25, hi_addr(?lstr15_MPU_IMU_PIC32_v001+0)
ORI	R25, R25, lo_addr(?lstr15_MPU_IMU_PIC32_v001+0)
JAL	_UART2_Write_Text+0
NOP	
;MPU_IMU_PIC32_v001.c,148 :: 		Init_Ext_Int1();
JAL	_Init_Ext_Int1+0
NOP	
;MPU_IMU_PIC32_v001.c,150 :: 		Init_BNO55();
JAL	_Init_BNO55+0
NOP	
;MPU_IMU_PIC32_v001.c,153 :: 		while(1) {
L_main12:
;MPU_IMU_PIC32_v001.c,155 :: 		Print_BNO55_Data();
JAL	_Print_BNO55_Data+0
NOP	
;MPU_IMU_PIC32_v001.c,156 :: 		delay_ms(500);
LUI	R24, 203
ORI	R24, R24, 29524
L_main14:
ADDIU	R24, R24, -1
BNE	R24, R0, L_main14
NOP	
NOP	
NOP	
;MPU_IMU_PIC32_v001.c,158 :: 		if(IMU_interrupt_bit){
LBU	R2, Offset(_IMU_interrupt_bit+0)(GP)
EXT	R2, R2, BitPos(_IMU_interrupt_bit+0), 1
BNE	R2, R0, L__main23
NOP	
J	L_main16
NOP	
L__main23:
;MPU_IMU_PIC32_v001.c,160 :: 		IMU_interrupt_bit = 0;
LBU	R2, Offset(_IMU_interrupt_bit+0)(GP)
INS	R2, R0, BitPos(_IMU_interrupt_bit+0), 1
SB	R2, Offset(_IMU_interrupt_bit+0)(GP)
;MPU_IMU_PIC32_v001.c,161 :: 		}
L_main16:
;MPU_IMU_PIC32_v001.c,163 :: 		}
J	L_main12
NOP	
;MPU_IMU_PIC32_v001.c,164 :: 		}
L_end_main:
L__main_end_loop:
J	L__main_end_loop
NOP	
; end of _main
