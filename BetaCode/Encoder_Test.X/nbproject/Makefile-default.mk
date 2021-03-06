#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Encoder_Test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Encoder_Test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../BB_Library/BB_BOARD.c ../../BB_Library/BB_Encoder.c ../../BB_Library/BB_LEDS.c ../../BB_Library/BB_Motor.c ../../BB_Library/BB_UART.c Encoder_Test.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/418195140/BB_BOARD.o ${OBJECTDIR}/_ext/418195140/BB_Encoder.o ${OBJECTDIR}/_ext/418195140/BB_LEDS.o ${OBJECTDIR}/_ext/418195140/BB_Motor.o ${OBJECTDIR}/_ext/418195140/BB_UART.o ${OBJECTDIR}/Encoder_Test.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/418195140/BB_BOARD.o.d ${OBJECTDIR}/_ext/418195140/BB_Encoder.o.d ${OBJECTDIR}/_ext/418195140/BB_LEDS.o.d ${OBJECTDIR}/_ext/418195140/BB_Motor.o.d ${OBJECTDIR}/_ext/418195140/BB_UART.o.d ${OBJECTDIR}/Encoder_Test.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/418195140/BB_BOARD.o ${OBJECTDIR}/_ext/418195140/BB_Encoder.o ${OBJECTDIR}/_ext/418195140/BB_LEDS.o ${OBJECTDIR}/_ext/418195140/BB_Motor.o ${OBJECTDIR}/_ext/418195140/BB_UART.o ${OBJECTDIR}/Encoder_Test.o

# Source Files
SOURCEFILES=../../BB_Library/BB_BOARD.c ../../BB_Library/BB_Encoder.c ../../BB_Library/BB_LEDS.c ../../BB_Library/BB_Motor.c ../../BB_Library/BB_UART.c Encoder_Test.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Encoder_Test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/418195140/BB_BOARD.o: ../../BB_Library/BB_BOARD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/418195140 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_BOARD.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/418195140/BB_BOARD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/_ext/418195140/BB_BOARD.o.d" -o ${OBJECTDIR}/_ext/418195140/BB_BOARD.o ../../BB_Library/BB_BOARD.c   -legacy-libc
	
${OBJECTDIR}/_ext/418195140/BB_Encoder.o: ../../BB_Library/BB_Encoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/418195140 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_Encoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_Encoder.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/418195140/BB_Encoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/_ext/418195140/BB_Encoder.o.d" -o ${OBJECTDIR}/_ext/418195140/BB_Encoder.o ../../BB_Library/BB_Encoder.c   -legacy-libc
	
${OBJECTDIR}/_ext/418195140/BB_LEDS.o: ../../BB_Library/BB_LEDS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/418195140 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_LEDS.o.d 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_LEDS.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/418195140/BB_LEDS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/_ext/418195140/BB_LEDS.o.d" -o ${OBJECTDIR}/_ext/418195140/BB_LEDS.o ../../BB_Library/BB_LEDS.c   -legacy-libc
	
${OBJECTDIR}/_ext/418195140/BB_Motor.o: ../../BB_Library/BB_Motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/418195140 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_Motor.o.d 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_Motor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/418195140/BB_Motor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/_ext/418195140/BB_Motor.o.d" -o ${OBJECTDIR}/_ext/418195140/BB_Motor.o ../../BB_Library/BB_Motor.c   -legacy-libc
	
${OBJECTDIR}/_ext/418195140/BB_UART.o: ../../BB_Library/BB_UART.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/418195140 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_UART.o.d 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_UART.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/418195140/BB_UART.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/_ext/418195140/BB_UART.o.d" -o ${OBJECTDIR}/_ext/418195140/BB_UART.o ../../BB_Library/BB_UART.c   -legacy-libc
	
${OBJECTDIR}/Encoder_Test.o: Encoder_Test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Encoder_Test.o.d 
	@${RM} ${OBJECTDIR}/Encoder_Test.o 
	@${FIXDEPS} "${OBJECTDIR}/Encoder_Test.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/Encoder_Test.o.d" -o ${OBJECTDIR}/Encoder_Test.o Encoder_Test.c   -legacy-libc
	
else
${OBJECTDIR}/_ext/418195140/BB_BOARD.o: ../../BB_Library/BB_BOARD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/418195140 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_BOARD.o.d 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_BOARD.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/418195140/BB_BOARD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/_ext/418195140/BB_BOARD.o.d" -o ${OBJECTDIR}/_ext/418195140/BB_BOARD.o ../../BB_Library/BB_BOARD.c   -legacy-libc
	
${OBJECTDIR}/_ext/418195140/BB_Encoder.o: ../../BB_Library/BB_Encoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/418195140 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_Encoder.o.d 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_Encoder.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/418195140/BB_Encoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/_ext/418195140/BB_Encoder.o.d" -o ${OBJECTDIR}/_ext/418195140/BB_Encoder.o ../../BB_Library/BB_Encoder.c   -legacy-libc
	
${OBJECTDIR}/_ext/418195140/BB_LEDS.o: ../../BB_Library/BB_LEDS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/418195140 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_LEDS.o.d 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_LEDS.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/418195140/BB_LEDS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/_ext/418195140/BB_LEDS.o.d" -o ${OBJECTDIR}/_ext/418195140/BB_LEDS.o ../../BB_Library/BB_LEDS.c   -legacy-libc
	
${OBJECTDIR}/_ext/418195140/BB_Motor.o: ../../BB_Library/BB_Motor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/418195140 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_Motor.o.d 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_Motor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/418195140/BB_Motor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/_ext/418195140/BB_Motor.o.d" -o ${OBJECTDIR}/_ext/418195140/BB_Motor.o ../../BB_Library/BB_Motor.c   -legacy-libc
	
${OBJECTDIR}/_ext/418195140/BB_UART.o: ../../BB_Library/BB_UART.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/418195140 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_UART.o.d 
	@${RM} ${OBJECTDIR}/_ext/418195140/BB_UART.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/418195140/BB_UART.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/_ext/418195140/BB_UART.o.d" -o ${OBJECTDIR}/_ext/418195140/BB_UART.o ../../BB_Library/BB_UART.c   -legacy-libc
	
${OBJECTDIR}/Encoder_Test.o: Encoder_Test.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Encoder_Test.o.d 
	@${RM} ${OBJECTDIR}/Encoder_Test.o 
	@${FIXDEPS} "${OBJECTDIR}/Encoder_Test.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"../../BB_Library" -MMD -MF "${OBJECTDIR}/Encoder_Test.o.d" -o ${OBJECTDIR}/Encoder_Test.o Encoder_Test.c   -legacy-libc
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Encoder_Test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Encoder_Test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -legacy-libc  -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Encoder_Test.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Encoder_Test.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -legacy-libc -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Encoder_Test.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
