#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-none-eabi-gcc
CCC=arm-none-eabi-g++
CXX=arm-none-eabi-g++
FC=gfortran
AS=arm-none-eabi-gcc

# Macros
CND_PLATFORM=GNU_ARM-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/ccu_control.o \
	${OBJECTDIR}/dram_control.o \
	${OBJECTDIR}/lichee_nano_startup_with_fel.o \
	${OBJECTDIR}/start.o


# C Compiler Flags
CFLAGS=-Wall -Wno-write-strings -Wno-char-subscripts -fno-stack-protector -DNO_STDLIB=1 -mcpu='arm926ej-s' -O0

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=-x assembler-with-cpp -c -O0 -mcpu='arm926ej-s' -mthumb -Wall -fmessage-length=0

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lichee_nano_startup_with_fel.elf

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lichee_nano_startup_with_fel.elf: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	arm-none-eabi-gcc -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lichee_nano_startup_with_fel.elf ${OBJECTFILES} ${LDLIBSOPTIONS} -T./fnirsi_1013d.ld -nostdlib

${OBJECTDIR}/ccu_control.o: ccu_control.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccu_control.o ccu_control.c

${OBJECTDIR}/dram_control.o: dram_control.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/dram_control.o dram_control.c

${OBJECTDIR}/lichee_nano_startup_with_fel.o: lichee_nano_startup_with_fel.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lichee_nano_startup_with_fel.o lichee_nano_startup_with_fel.c

${OBJECTDIR}/start.o: start.s
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/start.o start.s

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
