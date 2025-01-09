//----------------------------------------------------------------------------------------------------------------------------------

#include "fpga_control.h"

//----------------------------------------------------------------------------------------------------------------------------------

void fpga_init(void)
{
  //First set pin high in data register to avoid spikes when changing from input to output
  FPGA_CLK_INIT();
  
  //Initialize the three control lines for output
  FPGA_CTRL_INIT();
}

//----------------------------------------------------------------------------------------------------------------------------------

void fpga_strobe_control(uint8 address)
{
  //Set the bus for writing
  FPGA_BUS_DIR_OUT();

  //Write the data to the bus  
  FPGA_SET_DATA(address);
  
  //Clock the data into the FPGA
  FPGA_PULSE_CONTROL_STROBE();
}

//----------------------------------------------------------------------------------------------------------------------------------

void fpga_write_byte(uint8 data)
{
  //Set the bus for writing
  FPGA_BUS_DIR_OUT();
  
  //Write the data to the bus  
  FPGA_SET_DATA(data);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();
}

//----------------------------------------------------------------------------------------------------------------------------------
#if 0
uint8 fpga_read_byte(void)
{
  //Set the bus for reading
  FPGA_BUS_DIR_IN();
  
  //Set the control lines for reading a command
  FPGA_DATA_READ();

  //Clock the data to the output of the FPGA
  FPGA_PULSE_CLK();
 
  //Read the data
  return(FPGA_GET_DATA());
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------

void fpga_write_short(uint16 data)
{
  //Set the bus for writing
  FPGA_BUS_DIR_OUT();
  
  //Write the msb to the bus  
  FPGA_SET_DATA(data >> 8);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();
  
  //Write the lsb to the bus  
  FPGA_SET_DATA(data);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();
}

//----------------------------------------------------------------------------------------------------------------------------------
#if 0
uint16 fpga_read_short(void)
{
  uint16 data;
  
  //Set the bus for reading
  FPGA_BUS_DIR_IN();
  
  //Set the control lines for reading a command
  FPGA_DATA_READ();

  //Clock the data to the output of the FPGA
  FPGA_PULSE_CLK();

  //Get the msb
  data = FPGA_GET_DATA() << 8;

  //Clock the data to the output of the FPGA
  FPGA_PULSE_CLK();

  //Get the lsb
  data |= FPGA_GET_DATA();
  
  //Read the data
  return(data);
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------

void fpga_write_int(uint32 data)
{
  //Set the bus for writing
  FPGA_BUS_DIR_OUT();
  
  //Write the msb to the bus  
  FPGA_SET_DATA(data >> 24);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();

  //Write the mlsb to the bus  
  FPGA_SET_DATA(data >> 16);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();
  
  //Write the lmsb to the bus  
  FPGA_SET_DATA(data >> 8);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();
  
  //Write the lsb to the bus  
  FPGA_SET_DATA(data);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();
}

//----------------------------------------------------------------------------------------------------------------------------------
#if 0
uint32 fpga_read_int(void)
{
  uint32 data;
  
  //Set the bus for reading
  FPGA_BUS_DIR_IN();
  
  //Set the control lines for reading a command
  FPGA_DATA_READ();

  //Clock the data to the output of the FPGA
  FPGA_PULSE_CLK();

  //Get the msb
  data = FPGA_GET_DATA() << 24;

  //Clock the data to the output of the FPGA
  FPGA_PULSE_CLK();

  //Get the mlsb
  data |= FPGA_GET_DATA() << 16;

  //Clock the data to the output of the FPGA
  FPGA_PULSE_CLK();

  //Get the lmsb
  data |= FPGA_GET_DATA() << 8;

  //Clock the data to the output of the FPGA
  FPGA_PULSE_CLK();

  //Get the lsb
  data |= FPGA_GET_DATA();
  
  //Read the data
  return(data);
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------

void fpga_write_long(uint64 data)
{
  //Set the bus for writing
  FPGA_BUS_DIR_OUT();

  //Write the msb to the bus  
  FPGA_SET_DATA(data >> 40);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();

  //Write the msb to the bus  
  FPGA_SET_DATA(data >> 32);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();
  
  //Write the msb to the bus  
  FPGA_SET_DATA(data >> 24);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();

  //Write the mlsb to the bus  
  FPGA_SET_DATA(data >> 16);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();
  
  //Write the lmsb to the bus  
  FPGA_SET_DATA(data >> 8);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();
  
  //Write the lsb to the bus  
  FPGA_SET_DATA(data);
  
  //Clock the data into the FPGA
  FPGA_PULSE_DATA_STROBE();
}

//----------------------------------------------------------------------------------------------------------------------------------

void fpga_delay(uint32 usec)
{
  uint32 loops = usec * 54;

  __asm__ __volatile__ ("1:\n" "subs %0, %1, #1\n"  "bne 1b":"=r"(loops):"0"(loops));
}

//----------------------------------------------------------------------------------------------------------------------------------
