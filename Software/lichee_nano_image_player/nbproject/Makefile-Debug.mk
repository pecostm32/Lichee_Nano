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
	${OBJECTDIR}/diskio.o \
	${OBJECTDIR}/display_control.o \
	${OBJECTDIR}/display_lib.o \
	${OBJECTDIR}/ff.o \
	${OBJECTDIR}/ffunicode.o \
	${OBJECTDIR}/font_5.o \
	${OBJECTDIR}/fpga_control.o \
	${OBJECTDIR}/interrupt.o \
	${OBJECTDIR}/lichee_nano_image_player.o \
	${OBJECTDIR}/mass_storage_class.o \
	${OBJECTDIR}/memcmp.o \
	${OBJECTDIR}/memcpy.o \
	${OBJECTDIR}/memmove.o \
	${OBJECTDIR}/memset.o \
	${OBJECTDIR}/scaler_control.o \
	${OBJECTDIR}/sd_card_interface.o \
	${OBJECTDIR}/sin_cos_math.o \
	${OBJECTDIR}/start.o \
	${OBJECTDIR}/timer.o \
	${OBJECTDIR}/usb_mass_storage_interface.o


# C Compiler Flags
CFLAGS=-Wall -Wno-write-strings -Wno-char-subscripts -fno-stack-protector -DNO_STDLIB=1 -mcpu='arm926ej-s' -O3

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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lichee_nano_image_player.elf

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lichee_nano_image_player.elf: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	arm-none-eabi-gcc -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/lichee_nano_image_player.elf ${OBJECTFILES} ${LDLIBSOPTIONS} -T./lichee_nano.ld -nostdlib -lgcc

${OBJECTDIR}/ccu_control.o: ccu_control.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ccu_control.o ccu_control.c

${OBJECTDIR}/diskio.o: diskio.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/diskio.o diskio.c

${OBJECTDIR}/display_control.o: display_control.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/display_control.o display_control.c

${OBJECTDIR}/display_lib.o: display_lib.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/display_lib.o display_lib.c

${OBJECTDIR}/ff.o: ff.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ff.o ff.c

${OBJECTDIR}/ffunicode.o: ffunicode.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ffunicode.o ffunicode.c

${OBJECTDIR}/font_5.o: font_5.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/font_5.o font_5.c

${OBJECTDIR}/fpga_control.o: fpga_control.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/fpga_control.o fpga_control.c

${OBJECTDIR}/interrupt.o: interrupt.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/interrupt.o interrupt.c

${OBJECTDIR}/lichee_nano_image_player.o: lichee_nano_image_player.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lichee_nano_image_player.o lichee_nano_image_player.c

${OBJECTDIR}/mass_storage_class.o: mass_storage_class.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mass_storage_class.o mass_storage_class.c

${OBJECTDIR}/memcmp.o: memcmp.s
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/memcmp.o memcmp.s

${OBJECTDIR}/memcpy.o: memcpy.s
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/memcpy.o memcpy.s

${OBJECTDIR}/memmove.o: memmove.s
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/memmove.o memmove.s

${OBJECTDIR}/memset.o: memset.s
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/memset.o memset.s

${OBJECTDIR}/scaler_control.o: scaler_control.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/scaler_control.o scaler_control.c

${OBJECTDIR}/sd_card_interface.o: sd_card_interface.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sd_card_interface.o sd_card_interface.c

${OBJECTDIR}/sin_cos_math.o: sin_cos_math.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sin_cos_math.o sin_cos_math.c

${OBJECTDIR}/start.o: start.s
	${MKDIR} -p ${OBJECTDIR}
	$(AS) $(ASFLAGS) -g -o ${OBJECTDIR}/start.o start.s

${OBJECTDIR}/timer.o: timer.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/timer.o timer.c

${OBJECTDIR}/usb_mass_storage_interface.o: usb_mass_storage_interface.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/usb_mass_storage_interface.o usb_mass_storage_interface.c

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
