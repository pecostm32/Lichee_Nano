#include "ccu_control.h"
#include "dram_control.h"

int main(void)
{
  //Initialize the clock system
  sys_clock_init();
  
  //Initialize the internal DRAM
  sys_dram_init();
  
  //Run the fel program
  unsigned int address = 0xFFFF0020;

  __asm__ __volatile__ ("mov pc, %0\n" :"=r"(address):"0"(address));
}
