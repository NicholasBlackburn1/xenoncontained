/* This program will print the system hardware temperatures, by
\  messaging the SMC, using mathematical operations, and printing
\  to the screen.
*/

#include <stdio.h>
#include <console/console.h>
#include <xenos/xenos.h>
#include <xenos/xe.h>
#include <xenon_smc/xenon_smc.h>
#include "video_init.h"

void xenon_set_cpu_fan_speed(unsigned val){

unsigned char msg[16] = { 0x94, (val & 0x7F) | 0x80 };

xenon_smc_send_message(msg);

}

void xenon_set_gpu_fan_speed(unsigned val){

unsigned char msg[16] = { 0x89, (val & 0x7F) | 0x80 };

xenon_smc_send_message(msg);

}

int main(){

	printf("Xenos/console init! By nicholas Blackburn");
	mainInit();

	uint8_t buf[16];
	float CPU_TMP = 0, GPU_TMP = 0, MEM_TMP = 0, MOBO_TMP = 0;
   
   while(1)
   {
      memset(buf, 0, 16);
   
      buf[0] = 0x07;
   
      xenon_smc_send_message(buf);
      xenon_smc_receive_response(buf);
   
      CPU_TMP = (float)((buf[0 * 2 + 1] | (buf[0 * 2 + 2] << 8)) / 256.0);
      GPU_TMP = (float)((buf[1 * 2 + 1] | (buf[1 * 2 + 2] << 8)) / 256.0);
      MEM_TMP = (float)((buf[2 * 2 + 1] | (buf[2 * 2 + 2] << 8)) / 256.0);
      MOBO_TMP = (float)((buf[3 * 2 + 1] | (buf[3 * 2 + 2] << 8)) / 256.0);
      
      printf("CPU = %4.2f C GPU = %4.2f C MEM = %4.2f C Mobo = %4.2f C", CPU_TMP, GPU_TMP, MEM_TMP, MOBO_TMP);
      printf("\r");

      if(CPU_TMP > 45.25){
         printf("Gamer U xb0x 360 is Smoking HOT!!\n THE CPU TEMP IS",CPU_TMP);
         printf("Turining on CPU Fans!!!\n");
         xenon_set_cpu_fan_speed(100);

      }
      if (GPU_TMP < 65.00)
      {
         printf("Gamer U xb0x 360 is Smoking HOT!!\n THE GPU TEMP IS", CPU_TMP);
         printf("Turining on GPU Fans!!!\n");
         xenon_set_gpu_fan_speed(100);

      }
      else{
         printf("u going to want fans soon!")
      }

      

   }

	return 0;
}

