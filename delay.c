#include "delay.h"


void SysTick_Init(){
NVIC_ST_CTRL_R = 0;
NVIC_ST_RELOAD_R = 0X00FFFFFF;
NVIC_ST_CURRENT_R = 0;
NVIC_ST_CTRL_R =0X00000005;
}
void SysTick_Wait(uint32_t delay){
SysTick_Init();
NVIC_ST_RELOAD_R = delay-1; // number of counts
NVIC_ST_CURRENT_R = 0; // any value written to CURRENT clears
while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for flag
}
}
void SysTick_Wait_1ms(uint32_t delay){
unsigned long i;
for(i=0; i<delay; i++){
SysTick_Wait(80000);
}
}// Call this routine to wait for delay*1ms