//----------------------------------------------------------------------------------------------------------------------------------

#include "types.h"

#include "ccu_control.h"
#include "timer.h"
#include "interrupt.h"
#include "display_control.h"
#include "sd_card_interface.h"
#include "fpga_control.h"
#include "lichee_nano_image_player.h"
#include "ff.h"
#include "usb_mass_storage_interface.h"
#include "display_lib.h"
#include "scaler_control.h"

#include "arm32.h"

#include <string.h>

//----------------------------------------------------------------------------------------------------------------------------------

extern IRQHANDLERFUNCION interrupthandlers[];

//----------------------------------------------------------------------------------------------------------------------------------

#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT    600

#define SCREEN_SIZE     (SCREEN_WIDTH * SCREEN_HEIGHT)

#define PICTURE_HEADER_SIZE               70
#define PICTURE_PIXELS                    1024   //1264
#define PICTURE_LINES                     600    //527
#define PICTURE_DATA_SIZE                 (PICTURE_PIXELS * PICTURE_LINES * 2)         //trace data 2 bytes per pixel for 16 bit RGB565
#define PICTURE_FILE_SIZE                 (PICTURE_HEADER_SIZE + PICTURE_DATA_SIZE)    //Bitmap header + pixel data

//----------------------------------------------------------------------------------------------------------------------------------

//This first buffer is defined as 32 bits to be able to write it to file
uint32 maindisplaybuffer[SCREEN_SIZE / 2];

FATFS fs;
FIL   viewfp;                         //Since files are not opened concurrent using a global file pointer

char viewfilename[64];


uint8 viewbitmapheader[PICTURE_HEADER_SIZE];

volatile uint32 usbdisconnect = 0;

//----------------------------------------------------------------------------------------------------------------------------------

void setup_display_lib(void)
{
  display_set_bg_color(0x00000000);
  
  display_set_screen_buffer((uint16 *)maindisplaybuffer);
  
  display_set_dimensions(SCREEN_WIDTH, SCREEN_HEIGHT);
}

//----------------------------------------------------------------------------------------------------------------------------------
//Setup the bitmap header
//Consist of basic bitmap header followed by a DIB header (BITMAPINFOHEADER + BITMAPV3INFOHEADER)
//The bitmap height is using a negative value for reversing the top to bottom lines. This allows for just writing the frame buffer to the file
const uint8 bmpheader[PICTURE_HEADER_SIZE] =
{
  //Header identifier
  'B', 'M',

  //Size of the file in bytes
   PICTURE_FILE_SIZE        & 0xFF,
  (PICTURE_FILE_SIZE >>  8) & 0xFF,
  (PICTURE_FILE_SIZE >> 16) & 0xFF,
  (PICTURE_FILE_SIZE >> 24) & 0xFF,

  //Reserved
  0, 0, 0, 0,

  //Offset to the pixel array
   PICTURE_HEADER_SIZE        & 0xFF,
  (PICTURE_HEADER_SIZE >>  8) & 0xFF,
  (PICTURE_HEADER_SIZE >> 16) & 0xFF,
  (PICTURE_HEADER_SIZE >> 24) & 0xFF,

  //Size of DIB header (BITMAPV3INFOHEADER seems to not be recognized on Windows XP and older Ubuntu and maybe other linux variants)
  56, 0, 0, 0,

  //Bitmap width in pixels
   PICTURE_PIXELS        & 0xFF,
  (PICTURE_PIXELS >>  8) & 0xFF,
  (PICTURE_PIXELS >> 16) & 0xFF,
  (PICTURE_PIXELS >> 24) & 0xFF,

  //Bitmap height in pixels (Negative to get the right vertical orientation)
   -PICTURE_LINES        & 0xFF,
  (-PICTURE_LINES >>  8) & 0xFF,
  (-PICTURE_LINES >> 16) & 0xFF,
  (-PICTURE_LINES >> 24) & 0xFF,

  //Number of color planes
  1, 0,

  //Number of bits per pixel
  16, 0,

  //Compression method (BI_BITFIELDS)
  3, 0, 0, 0,

  //Pixel array size
   PICTURE_DATA_SIZE        & 0xFF,
  (PICTURE_DATA_SIZE >>  8) & 0xFF,
  (PICTURE_DATA_SIZE >> 16) & 0xFF,
  (PICTURE_DATA_SIZE >> 24) & 0xFF,

  //Horizontal resolution
  0, 0, 0, 0,

  //Vertical resolution
  0, 0, 0, 0,

  //Number of colors in the pallete
  0, 0, 0, 0,

  //Number of colors important
  0, 0, 0, 0,

  //Mask fields for BI_BITFIELDS compression
  //Red mask 0x0000F800
  0, 0xF8, 0, 0,

  //Green mask 0x000007E0
  0xE0, 7, 0, 0,

  //Blue mask 0x0000001F
  0x1F, 0, 0, 0,

  //Alpha mask 0x00000000
  0, 0, 0, 0,
};

//----------------------------------------------------------------------------------------------------------------------------------
//Simple non optimized function for string copy that returns the position of the terminator
//----------------------------------------------------------------------------------------------------------------------------------

char *strcpy(char *dst, const char *src)
{
  while(*src)
  {
    *dst++ = *src++;
  }

  //Terminate the copy
  *dst = 0;

  return(dst);
}

//----------------------------------------------------------------------------------------------------------------------------------
//this can be simplified since only full numbers will be used

char *print_decimal(char *buffer, int32 value, uint32 decimals, uint32 negative)
{
  char   b[12];
  uint32 i = 12;   //Start beyond the array since the index is pre decremented
  uint32 s;

  //For value 0 no need to do the work
  if(value == 0)
  {
    //Value is zero so just set 0 character
    b[--i] = '0';
  }
  else
  {
    //Process the digits
    while(value)
    {
      //Set current digit to decreased index
      b[--i] = (value % 10) + '0';

      //Check if decimal point needs to be placed
      if(i == 12 - decimals)
      {
        //If so put it in
        b[--i] = '.';
      }

      //Take of the current digit
      value /= 10;
    }
  }

  //Check if negative number and if so put a minus in front of it
  if(negative)
    b[--i] = '-';
  
  //Determine the size of the string
  s = 12 - i;

  //Copy to the buffer
  memcpy(buffer, &b[i], s);

  //terminate the string
  buffer[s] = 0;

  //Return the position of the terminator to allow appending
  return(&buffer[s]);
}

//----------------------------------------------------------------------------------------------------------------------------------

void load_bitmap_data(int filenumber)
{
  uint32 result;
  char *dptr;

  //Set the name in the global buffer for message display
  strcpy(viewfilename, "full_image_");
  
  dptr = print_decimal(&viewfilename[11], filenumber, 0, 0);
  
  strcpy(dptr, ".bmp");

  //Try to open the file for reading
  result = f_open(&viewfp, viewfilename, FA_READ);

  //Check if file opened ok
  if(result == FR_OK)
  {
    //Read the bitmap header to verify if the bitmap can be displayed
    result = f_read(&viewfp, viewbitmapheader, PICTURE_HEADER_SIZE, 0);

    //Check if still ok to proceed
    if(result == FR_OK)
    {
      //Check if the header matches what it should be
      if(memcmp(viewbitmapheader, bmpheader, PICTURE_HEADER_SIZE) == 0)
      {
        //Load the bitmap data directly onto the screen
        result = f_read(&viewfp, (uint8 *)maindisplaybuffer, PICTURE_DATA_SIZE, 0);
      }
    }

    //Done with the file so close it
    f_close(&viewfp);
  }
}

//----------------------------------------------------------------------------------------------------------------------------------

int main(void)
{
  //Initialize data in BSS section
  memset(&BSS_START, 0, &BSS_END - &BSS_START);
  
  //Initialize the clock system
  sys_clock_init();
  
  //Instead of full memory management just the caches enabled
  arm32_icache_enable();
  arm32_dcache_enable();
  
  //Clear the interrupt variables
  memset(interrupthandlers, 0, 256);
  
  //Setup timer interrupt
  timer0_setup();
  
  //Enable interrupts only once. In the original code it is done on more then one location.
  arm32_interrupt_enable();
  
  //Initialize display (PORT D + DEBE)
  sys_init_display(SCREEN_WIDTH, SCREEN_HEIGHT, (uint16 *)maindisplaybuffer);
  
  //Setup the display library for the scope hardware
  setup_display_lib();
  
  //Setup and check SD card on file system being present
  if(f_mount(&fs, "0", 1))
  {
    //On error just hang
    while(1);
  }
  
  //Setup the USB interface
  usb_device_init();

  
#if 1
  //Setup an IO pin to show the interrupt duration
  //PE0 is set as an output and made low
  //PE6 is set low to enable backlight
  *FPGA_BUS_CFG_REG &= 0xF0FFFFF0; 
  *FPGA_BUS_CFG_REG |= 0x01000001;
  *FPGA_DATA_REG    &= 0xFFFFFFFE;
  
  *FPGA_DATA_REG    |= 0x00000040;

#endif

  //Process the scaling stuff here
  scaler_load_settings();
  
  //Set PE0 high for time measurement
  *FPGA_DATA_REG |= 0x00000001;

  scaler_process_file();

  //Set PE0 low for time measurement
  *FPGA_DATA_REG &= 0xFFFFFFFE;

  //Wait for the writing on the SD card to be finished
  timer0_delay(2000);

  //Enable for mass storage connection, so user can copy the files
  usb_device_enable();

  //Wait for the user to eject the drive
  while(usbdisconnect == 0);

  //Wait for the host to be finished. It has to clear the dirty bit
  timer0_delay(500);

  //Reset the disconnect flag to allow new connection
  usbdisconnect = 0;

  //Disable the mass storage connection
  usb_device_disable();
  
  while(1)
  {  
  }
}

//----------------------------------------------------------------------------------------------------------------------------------
