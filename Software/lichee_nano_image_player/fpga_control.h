//----------------------------------------------------------------------------------------------------------------------------------

#ifndef FPGA_CONTROL_H
#define FPGA_CONTROL_H

//----------------------------------------------------------------------------------------------------------------------------------

#include "types.h"

//----------------------------------------------------------------------------------------------------------------------------------
//On the Lichee nano port E is not fully available. Two of the lower 8 bit part of the port are used for brightness and USB id
//This means a change of the FPGA code
//The data bus is now divided into three parts 3 bits, 3 bits and 2 bits. PE6 and PE2 are skipped
//The control bits are starting on PE10
//----------------------------------------------------------------------------------------------------------------------------------

//FPGA port registers (Port E on the F1C100s)
#define FPGA_BUS_CFG_REG            ((volatile uint32 *)(0x01C20890))
#define FPGA_CTRL_CFG_REG           ((volatile uint32 *)(0x01C20894))
#define FPGA_DATA_REG               ((volatile uint32 *)(0x01C208A0))

//Initialize the control lines for communication with the FPGA (PE10:12 output)
#define FPGA_CTRL_INIT()            (*FPGA_CTRL_CFG_REG = (*FPGA_CTRL_CFG_REG & 0xFFF000FF) | 0x00011100)

//Initialize the two strobe lines to high
#define FPGA_CLK_INIT()             (*FPGA_DATA_REG |= 0x00001400)

//Strobe signals for data or control actions
#define FPGA_PULSE_DATA_STROBE()    (*FPGA_DATA_REG &= 0xFFFFFBFF);(*FPGA_DATA_REG |= 0x00000400)
#define FPGA_PULSE_CONTROL_STROBE() (*FPGA_DATA_REG &= 0xFFFFEFFF);(*FPGA_DATA_REG |= 0x00001000)

//Control the direction of the FPGA data bus and tell the FPGA to control it's High Z state
#define FPGA_BUS_DIR_IN()           (*FPGA_CTRL_CFG_REG &= 0xFFFFFF00);(*FPGA_BUS_CFG_REG &= 0x0F000F00);(*FPGA_DATA_REG = (*FPGA_DATA_REG & 0xFFFFF7FF) | 0x00000000);
#define FPGA_BUS_DIR_OUT()          (*FPGA_DATA_REG = (*FPGA_DATA_REG & 0xFFFFF7FF) | 0x00000800);(*FPGA_CTRL_CFG_REG &= 0xFFFFFF00);(*FPGA_BUS_CFG_REG &= 0x0F000F00);(*FPGA_CTRL_CFG_REG |= 0x00000011);(*FPGA_BUS_CFG_REG |= 0x10111011)

//Put data on or get data from the FPGA data bus
#define FPGA_SET_DATA(x)            (*FPGA_DATA_REG = (*FPGA_DATA_REG & 0xFFFFFC44) | ((x & 0x000000E0) << 2) | ((x & 0x0000001C) << 1) | (x & 0x00000003))
#define FPGA_GET_DATA()             (((*FPGA_DATA_REG & 0x00000380) >> 2) | ((*FPGA_DATA_REG & 0x00000038) >> 1) | (*FPGA_DATA_REG & 0x00000003))

//----------------------------------------------------------------------------------------------------------------------------------

void fpga_init(void);

void fpga_strobe_control(uint8 address);

void fpga_write_byte(uint8 data);
uint8 fpga_read_byte(void);

void fpga_write_short(uint16 data);
uint16 fpga_read_short(void);

void fpga_write_int(uint32 data);
uint32 fpga_read_int(void);

void fpga_write_long(uint64 data);

void fpga_delay(uint32 usec);

//----------------------------------------------------------------------------------------------------------------------------------

#endif /* FPGA_CONTROL_H */

