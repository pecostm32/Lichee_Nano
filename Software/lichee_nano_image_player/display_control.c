//----------------------------------------------------------------------------------------------------------------------------------

#include "types.h"
#include "display_control.h"
#include "ccu_control.h"
#include "gpio_control.h"

#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------

void sys_disable_display(void)
{
  volatile uint32 *ptr;

  //Clear the DEBE registers
  for(ptr=DEBE_MODE_CTRL;ptr<=DEBE_COEF23;ptr++)
    *ptr = 0;
    
  //Disable the LCD timing control module
  *TCON_CTRL = 0;
  
  //Disable the LCD timing interrupts
  *TCON_INT0 = 0;
  
  //Disable the LCD clock
  *TCON_CLK_CTRL &= 0x0FFFFFFF;
  
  //Disable all the TCON0 display outputs;
  *TCON0_IO_CTRL1 = 0xFFFFFFFF;
  
  //Disable all the TCON1 display outputs;
  *TCON1_IO_CTRL1 = 0xFFFFFFFF;
  
  //Enable the display engine back end
  *CCU_BUS_CLK_GATE1 &= ~CCU_BCGR1_DEBE_EN;

  //De-assert the display engine back end reset
  *CCU_BUS_SOFT_RST1 &= ~CCU_BSRR1_DEBE_RST;
}

//----------------------------------------------------------------------------------------------------------------------------------
//TCON0 is for LCD timing
//TCON1 is for TV timing

//With the 800 * 480 display the setup like it is done in the scope code gives a frame time of 8.4ms
//The front porch of the vertical sync is just a small amount of lines, so there must be some other setting
//that takes care of this.

void sys_init_display(uint16 xsize, uint16 ysize, uint16 *address)
{
  int32   time;
  
  //Setup the used port D pins for LCD
  *PORTD_CFG0_REG = 0x22222227;   //PD00 is not used for the display
  *PORTD_CFG1_REG = 0x22272222;   //PD12 is not used for the display
  *PORTD_CFG2_REG = 0x00222222;   //Only 22 pins for port D
   
  //Clear the display memory (Set in bytes so twice the number of pixels)
  memset((uint8 *)address, 0, xsize * ysize * 2);
  
  //Clear the DEBE registers
  memset((uint8 *)DEBE_MODE_CTRL, 0, DEBE_NUMBER_OF_BYTES);
  
  //Disable the LCD timing control module
  *TCON_CTRL = 0;
  
  //Disable the LCD timing interrupts
  *TCON_INT0 = 0;
  
  //Disable the LCD clock
  *TCON_CLK_CTRL &= 0x0FFFFFFF;
  
  //Disable all the TCON0 display outputs;
  *TCON0_IO_CTRL1 = 0xFFFFFFFF;
  
  //Disable all the TCON1 display outputs;
  *TCON1_IO_CTRL1 = 0xFFFFFFFF;

  //Display engine back end section
  //Enable the display engine back end
  *CCU_BUS_CLK_GATE1 |= CCU_BCGR1_DEBE_EN;

  //De-assert the display engine back end reset
  *CCU_BUS_SOFT_RST1 |= CCU_BSRR1_DEBE_RST;

  //What about the DEBE clock setting in the CCU. (BE Clock Register)
  //*CCU_DEBE_CLK = 0x00000000; //On reset and per default uses the PLL_VIDEO clock without dividing??
  
  //Enable the display engine back end
  *DEBE_MODE_CTRL |= DEBE_MODE_CTRL_EN;

  //Set layer0 size to xsize - 1 and ysize - 1
  *DEBE_LAY_SIZE  = ((xsize - 1) & 0x07FF) | (((ysize - 1) & 0x07FF) << 16);
  *DEBE_LAY0_SIZE = *DEBE_LAY_SIZE;
  
  //Set the layer0 line width in bits. (Using 16 bits per pixel)
  *DEBE_LAY0_LINEWIDTH = xsize * 16;
  
  //Set layer0 frame buffer address in bits. (8 bits per byte) With a 32 bit address the top 3 bits get lost. The manual is not clear about this.
  *DEBE_LAY0_FB_ADDR1L = (uint32)address << 3;
  
  //But in the scope there is a write to an un-described register
  *DEBE_LAY0_FB_ADDR1H = (uint32)address >> 29;
  
  //Set the layer 0 attributes to 565 RGB
  *DEBE_LAY0_ATT_CTRL1 = DEBE_LAY0_ATT_CTRL1_RGB565;
  
  //Enable layer0
  *DEBE_MODE_CTRL |= DEBE_MODE_CTRL_LAYER0_EN;
  
  //Load the module registers
  *DEBE_REGBUFF_CTRL |= DEBE_REGBUFF_CTRL_LOAD;
  
  //Start the output channel of the display engine back end
  *DEBE_MODE_CTRL |= DEBE_MODE_CTRL_START;
  
  //Video clock section
  //Set video PLL to 390 MHz ((24 * 65) / 4)
  *CCU_PLL_VIDEO_CTRL = CCU_PLL_ENABLE | CCU_PLL_VIDEO_MODE_SEL_INT | CCU_PLL_VIDEO_FACTOR_N(65) | CCU_PLL_VIDEO_PREDIV_M(4);
  
  //Max checks on PLL becoming stable
  time = 4000;
  
  //Wait for the PLL to become stable, but not endlessly
  while(time && ((*CCU_PLL_VIDEO_CTRL & CCU_PLL_LOCKED) == 0))
    time--;
  
  //Enable the lcd clock
  *CCU_BUS_CLK_GATE1 |= CCU_BCGR1_LCD_EN;

  //Turn the LCD (TCON) clock on. Assume clock source to be PLL_VIDEO * 1
  *CCU_LCD_CLK |= CCU_LCD_CLK_EN;
  
  //De-assert the lcd clock reset
  *CCU_BUS_SOFT_RST1 |= CCU_BSRR1_LCD_RST;
  
  //Use the TCON0 registers
  *TCON_CTRL &= ~TCON_CTRL_IO_MAP_SEL_TCON1;
  
  //Enable the timing generator. TCON source is DE CH1 (FIFO1 enable). TCON_STA_DLY = 23.
  *TCON0_CTRL = 0x80000170;
  
  //Enable all LCD clocks. Dot clock divider is 6. Makes dot clock 65MHz.
  *TCON_CLK_CTRL = 0xF0000006;
  
  //Set timing based on the sizes. xsize - 1 and ysize - 1
  *TCON0_BASIC_TIMING0  = ((ysize - 1) & 0x07FF) | (((xsize - 1) & 0x07FF) << 16);
  
  
  //Correct these for the 1024 x 600
  //Use the default configuration
  //Horizontal total time and horizontal back porch (1344 clocks per line with 160 clocks back porch)
  //This depends on the total line time and the back porch
  //Back porch = (140 - 1);
  *TCON0_BASIC_TIMING1 = 0x53F00B5;      //0x41E0044;

  //Vertical front porch and vertical back porch (12 lines front porch with 23 lines back porch)
  //This is a bit of a mystery for the front porch setting. Even with 0 there is a gap between the last line displayed and the next sync pulse of many lines (~30ms)
  //The back porch setting includes the sync pulse lines too??
  *TCON0_BASIC_TIMING2 = 0x4F90019; // 0x46E0016;
  
  //Horizontal sync pulse width (20 clock pulses) and vertical sync pulse width. 0 means 1 line time for the vertical sync
  //HSPW = (20 - 1) << 16;
  //VSPW = (3 - 1);
  *TCON0_BASIC_TIMING3 = 0x150002;
  
  //
  *TCON0_HV_TIMING = 0;
  
  //
  *TCON0_CPU_IF = 0;
  
  //DCLK1 (1/3 phase offset). IO0:2 invert IO3 not invert.
  *TCON0_IO_CTRL0 = 0x17000000;
  
  //
  *TCON0_IO_CTRL1 = 0;
  
  //Enable the LCD timing control module
  *TCON_CTRL |= TCON_CTRL_MODULE_EN;
}

//----------------------------------------------------------------------------------------------------------------------------------

void sys_init_scaler()
{
  //Display engine front end section
  //Enable the display engine front end
  *CCU_BUS_CLK_GATE1 |= CCU_BCGR1_DEFE_EN;

  //De-assert the display engine back end reset
  *CCU_BUS_SOFT_RST1 |= CCU_BSRR1_DEFE_RST;
  
  //Enable the DEFE clock in the CCU. (FE Clock Register)
  *CCU_DEFE_CLK = 0x10000000;
  
  //Enable the front end engine
  *DEFE_EN_REG = DEFE_EN_ENABLE;  //| DEFE_EN_BIST_EN might be needed
  
  //There are three channels, that can either do full images or tiles
  //Question is what LINE_STRIDE does
  
  
}

//----------------------------------------------------------------------------------------------------------------------------------

void display_bitmap(uint16 xpos, uint16 ypos, uint16 xsize, uint16 ysize, uint16 *source, uint16 *dest)
{
  uint16 y;
  uint16 x;
  
  uint16 *ptr;
  
  for(y=0;y<ysize;y++)
  {
    ptr = dest + (((ypos + y) * 800) + xpos);
    
    for(x=0;x<xsize;x++)
    {
      *ptr++ = *source++;
    }
  }
}

//----------------------------------------------------------------------------------------------------------------------------------
