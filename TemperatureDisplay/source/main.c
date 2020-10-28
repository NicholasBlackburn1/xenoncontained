/**
 * 
 * Learning xbox 360 homebrew dev in c 
 * By Nicholas Blackburn & docker!
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <console/console.h>
#include <xenos/xenos.h>
#include <xenos/xe.h>
#include <xenon_smc/xenon_smc.h>
#include "video_init.h"
#include <input/input.h>
// Xenon includes
#include <libfat/fat.h>
#include <xenos/xenos.h>
#include <console/console.h>
#include <time/time.h>
#include <ppc/timebase.h>
#include <usb/usbmain.h>
#include <sys/iosupport.h>
#include <ppc/register.h>
#include <xenon_nand/xenon_sfcx.h>
#include <xenon_nand/xenon_config.h>
#include <xenon_soc/xenon_secotp.h>
#include <xenon_soc/xenon_power.h>
#include <xenon_soc/xenon_io.h>
#include <xenon_sound/sound.h>
#include <xenon_smc/xenon_smc.h>
#include <xenon_smc/xenon_gpio.h>
#include <xb360/xb360.h>
#include <diskio/ata.h>
#include "asciiart.h"

/**
 * Controls cpu fan speed
 */
void xenon_set_cpu_fan_speed(unsigned val)
{

   unsigned char msg[16] = {0x94, (val & 0x7F) | 0x80};

   xenon_smc_send_message(msg);
}

/**
 * sets gpu fan speed
 */
void xenon_set_gpu_fan_speed(unsigned val)
{

   unsigned char msg[16] = {0x89, (val & 0x7F) | 0x80};

   xenon_smc_send_message(msg);
}

void do_asciiart()
{
	char *p = asciiart;
	while (*p)
		console_putch(*p++);
}
/**
 * Main function initss and sets up xbox control
 */

int main()
{
   

   // Over Drives Xbox 
	xenon_make_it_faster(XENON_SPEED_FULL);

   xenos_init(VIDEO_MODE_AUTO);
   console_init();

   kmem_init();
   usb_init();
   usb_do_poll();

   console_set_colors(CONSOLE_COLOR_BLACK,CONSOLE_COLOR_GREEN);
   do_asciiart();

   printf("\n");
   printf(" press a to See temp\n");
   printf("press b to Hear something\n");
   printf(" press x to close program\n");

   uint8_t buf[16];
   float CPU_TMP = 0, GPU_TMP = 0, MEM_TMP = 0, MOBO_TMP = 0;
   struct controller_data_s oldc;
   while (1)
   {
      /* gets the Temp Sensor Data via buffer*/
      memset(buf, 0, 16);
      buf[0] = 0x07;
      
      xenon_smc_send_message(buf);
      xenon_smc_receive_response(buf);
      

      struct controller_data_s c;
 		if (get_controller_data(&c, 0))
 		{
 
 			if((c.a)&&(!oldc.a))
 			{
            getTemp(buf,CPU_TMP,GPU_TMP,MEM_TMP,MOBO_TMP);
            printf("a pressed\n");
 			}
 		
         else if((c.x)&&(!oldc.x))
 			{
 				printf("exiting..");
            exit(1);
 			}
 			oldc=c;
 		}
 		usb_do_poll();

   }

   return 0;
}

/**
 * gets the xbox temp and Reports it in C
 *
 */

void getTemp(uint8_t buf[16],float CPU_TMP, float GPU_TMP, float MEM_TMP, float MOBO_TMP){
      
      memset(buf, 0, 16);
   
      buf[0] = 0x07;
   
      xenon_smc_send_message(buf);
      xenon_smc_receive_response(buf);
   
      CPU_TMP = (float)((buf[0 * 2 + 1] | (buf[0 * 2 + 2] << 8)) / 256.0);
      GPU_TMP = (float)((buf[1 * 2 + 1] | (buf[1 * 2 + 2] << 8)) / 256.0);
      MEM_TMP = (float)((buf[2 * 2 + 1] | (buf[2 * 2 + 2] << 8)) / 256.0);
      MOBO_TMP = (float)((buf[3 * 2 + 1] | (buf[3 * 2 + 2] << 8)) / 256.0);
      
      if(CPU_TMP > 46.00){
         console_set_colors(CONSOLE_COLOR_BLACK,CONSOLE_COLOR_GREEN);
         printf("CPU = %4.2f \n", CPU_TMP);
         printf("GPU = %4.2f C \n", GPU_TMP); 
         printf("MEM = %4.2f C \n", MEM_TMP); 
         printf("Mobo = %4.2f C \n", MOBO_TMP);
         printf("\r");

      }else if (CPU_TMP > 48.20){
          console_set_colors(CONSOLE_COLOR_BLACK,CONSOLE_COLOR_YELLOW);
         printf("CPU = %4.2f \n", CPU_TMP);
         printf("GPU = %4.2f C \n", GPU_TMP); 
         printf("MEM = %4.2f C \n", MEM_TMP); 
         printf("Mobo = %4.2f C \n", MOBO_TMP);
         printf("\r");

      }else if (CPU_TMP > 49.20){
         console_set_colors(CONSOLE_COLOR_BLACK,CONSOLE_COLOR_ORANGE);
         printf("CPU = %4.2f \n", CPU_TMP);
         printf("GPU = %4.2f C \n", GPU_TMP); 
         printf("MEM = %4.2f C \n", MEM_TMP); 
         printf("Mobo = %4.2f C \n", MOBO_TMP);
         printf("\r");

      }else if (CPU_TMP > 50.20){
         console_set_colors(CONSOLE_COLOR_BLACK,CONSOLE_COLOR_RED);
         printf("CPU = %4.2f \n", CPU_TMP);
         printf("GPU = %4.2f C \n", GPU_TMP); 
         printf("MEM = %4.2f C \n", MEM_TMP); 
         printf("Mobo = %4.2f C \n", MOBO_TMP);
         printf("\r");

      }
      
      printf("CPU = %4.2f \n", CPU_TMP);
      printf("GPU = %4.2f C \n", GPU_TMP); 
      printf("MEM = %4.2f C \n", MEM_TMP); 
      printf("Mobo = %4.2f C \n", MOBO_TMP);
      printf("\r");
}