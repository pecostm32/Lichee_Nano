//--------------------------------------------------------------------------------------

#include "types.h"
#include "ccu_control.h"
#include "display_control.h"
#include "scaler_control.h"
#include "ff.h"

#include "display_lib.h"

//--------------------------------------------------------------------------------------

#define MAX_INPUT_SIZE          2048 * 2048         //The input image can at max be 2048 * 2048 pixels

#define MAX_OUTPUT_SIZE         1280 * 1280         //The output image is at max 1280 * 1280 pixels

//--------------------------------------------------------------------------------------

FIL fp;

extern FONTDATA font_5;

uint32 input_buffer[MAX_INPUT_SIZE];
uint32 output_buffer[MAX_OUTPUT_SIZE];

//--------------------------------------------------------------------------------------

void scaler_load_settings(void)
{
  int index;
  
  //Show it is the scaler setup being done
  display_set_fg_color(0x00FF4040);
  display_set_font(&font_5);
  display_text(50, 150, "SCALER SETUP");
  
  //Enable the display engine front end
  *CCU_BUS_CLK_GATE1 |= CCU_BCGR1_DEFE_EN;

  //De-assert the display engine back end reset
  *CCU_BUS_SOFT_RST1 |= CCU_BSRR1_DEFE_RST;

  //Enable the DEFE clock in the CCU. (FE Clock Register)
  *CCU_DEFE_CLK = 0x80000000;

  //Enable the front end engine with cpu ram access enabled
  *DEFE_EN_REG = DEFE_EN_ENABLE | DEFE_EN_BIST_EN;
  
  //No color space conversion
  *DEFE_BYPASS_REG = DEFE_BYPASS_CSC_BYPASS_EN | DEFE_BYPASS_DEFE_BYPASS_EN; //For test just skip the scaler
  
  //Horizontal scaling only over a 4 pixel range
  *DEFE_AGTH_SEL_REG = DEFE_AGTH_SEL_4_1;
  
  //Only needed when interrupts are used
  //*DEFE_LINT_CTRL_REG = 0;
  
   //Set reading from the input buffer. Only channel 0 is being used.
  *DEFE_BUF_ADDR0_REG = (uint32)input_buffer;
  //*DEFE_BUF_ADDR1_REG = 0;
  //*DEFE_BUF_ADDR2_REG = 0;
  
  //No idea about this one
  //*DEFE_FIELD_CTRL_REG = 0;
  
  //Tiles are not used
  //*DEFE_TB_OFF0_REG = 0;
  //*DEFE_TB_OFF1_REG = 0;
  //*DEFE_TB_OFF2_REG = 0;
  
  //The input data has 1024 pixels per line
  *DEFE_LINESTRD0_REG  = 1024;
  //*DEFE_LINESTRD1_REG  = 0;
  //*DEFE_LINESTRD2_REG  = 0;
  
  //Non tile based RGB888 (ARGB)
  *DEFE_INPUT_FMT_REG  = 0x00000051;
  
  //Set writing to the output buffer
  *DEFE_WB_ADDR_REG = (uint32)output_buffer;
  
  //Non interlaced planer RGB888
  *DEFE_OUTPUT_FMT_REG = 0;
  
  //No interrupts
  *DEFE_INT_EN_REG = 0;
  
  //Clear all pending interrupts
  *DEFE_INT_STATUS_REG = 0;
  
  //No color space conversion used
//  *DEFE_CSC_COEF00_REG = 0;
//  *DEFE_CSC_COEF01_REG = 0;
//  *DEFE_CSC_COEF02_REG = 0;
//  *DEFE_CSC_COEF03_REG = 0;
//  *DEFE_CSC_COEF10_REG = 0;
//  *DEFE_CSC_COEF11_REG = 0;
//  *DEFE_CSC_COEF12_REG = 0;
//  *DEFE_CSC_COEF13_REG = 0;
//  *DEFE_CSC_COEF20_REG = 0;
//  *DEFE_CSC_COEF21_REG = 0;
//  *DEFE_CSC_COEF22_REG = 0;
//  *DEFE_CSC_COEF23_REG = 0;
  
  //Input image is 1024 * 600
  *DEFE_INSIZE_REG = (599 << 16) | 1023;
  
  //The output image is 640 * 600
//  *DEFE_OUTSIZE_REG    = (599 << 16) | 639;
  *DEFE_OUTSIZE_REG = (599 << 16) | 1023;
  
  //1024 / 640 = 1.6, integer part is 1, fractional part based on 16 bits
//  *DEFE_HORZFACT_REG = (1 << 16) | 39321;
  *DEFE_HORZFACT_REG = (1 << 16);
  *DEFE_VERTFACT_REG = (1 << 16);

  //Enable write back line stride
  *DEFE_WB_LINESTRD_EN_REG = 0x00000001;
  *DEFE_WB_LINESTRD_REG = 1024;

  //Enable CPU access to the coefficient memory
  *DEFE_FRM_CTRL_REG |= DEFE_FRM_CTRL_COEF_ACCESS;

  for(index=0;index<32;index++)
  {
    //All coefficients set to 1
    DEFE_CH0_HORZCOEF_REGN[index] = 0x40404040;
    DEFE_CH0_VERTCOEF_REGN[index] = 0x40404040;
  }
//  DEFE_CH1_HORZCOEF_REGN[index] = 0;
//  DEFE_CH1_VERTCOEF_REGN[index] = 0;

  //Disable CPU access to the coefficient memory
  *DEFE_FRM_CTRL_REG &= ~DEFE_FRM_CTRL_COEF_ACCESS;

  display_hex(500, 150, 8 , *DEFE_BUF_ADDR0_REG);
  display_hex(500, 180, 8 , *DEFE_WB_ADDR_REG);

  display_text(250, 150, "DONE");
}

//--------------------------------------------------------------------------------------

int scaler_process_file(void)
{
  uint32 result;

  //Show the file processing started
  display_set_fg_color(0x00FF4040);
  display_set_font(&font_5);
  display_text(50, 200, "FILE PROCESSING");
  
  //Try to open the file for reading
  result = f_open(&fp, "file_to_scale.bin", FA_READ);

  //Check if file opened ok
  if(result == FR_OK)
  {
    display_text(70, 230, "Input file opened");
    
    //Read the settings into memory
    result = f_read(&fp, (uint8 *)input_buffer, MAX_INPUT_SIZE * 4, 0);

    //Done with the file so close it
    f_close(&fp);

    //Check if still ok to proceed
    if(result == FR_OK)
    {
      display_text(70, 260, "Input file read");

      if((*DEFE_STATUS_REG & DEFE_STATUS_WB_STATUS) == 0)
      {
        display_text(70, 290, "Scaler inactive");
      }
      
      //Allow scaler access to the memory
      *DEFE_EN_REG &= ~DEFE_EN_BIST_EN;
      
      //Start the scaling process
      *DEFE_FRM_CTRL_REG |= DEFE_FRM_CTRL_FRAME_START | DEFE_FRM_CTRL_WB_EN | DEFE_FRM_CTRL_REG_RDY_EN;
      
      if(*DEFE_STATUS_REG & DEFE_STATUS_WB_STATUS)
      {
        display_text(70, 320, "Scaler active");
      }
      
      //Wait for process to be finished
      while(*DEFE_STATUS_REG & DEFE_STATUS_WB_STATUS);

      display_text(70, 350, "Scaler done");
      
      //Allow CPU to access memory
      *DEFE_EN_REG |= DEFE_EN_BIST_EN;
      
      //Try to open the file for writing
      result = f_open(&fp, "scaled_file.bin", FA_CREATE_ALWAYS | FA_WRITE);
      
      if(result == FR_OK)
      {
        display_text(70, 380, "Output file opened");

        result = f_write(&fp, (uint8 *)output_buffer, 1024 * 600 * 4, 0);

        //Done with the file so close it
        f_close(&fp);
        
        if(result == FR_OK)
        {
          display_text(70, 410, "Output file written");

          display_text(250, 200, "DONE");

          return(1);
        }
      }
    }
  }
  
  display_text(250, 200, "FAILED");
  
  return(0);
}

//--------------------------------------------------------------------------------------
