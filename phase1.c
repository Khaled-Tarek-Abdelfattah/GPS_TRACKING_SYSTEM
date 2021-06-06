#include "math.h"
#include "stdint.h"
#define CPAC (*((volatile uint32_t *)0xE000ED88))
void SystemInit()
{
	CPAC |= 0X00F00000;
}
#include "led.h"
#include "distance.h"
#include "delay.h"
#include "lcd_init.h"
#include "lcd_cmd_data.h"
#include "lcd_tracking.h"
#include "constants_phase1.h"
#define  PI  3.141592654




int main(){
		long double walked_dist;
	  lcd_init();
		init();
		LCD_Cmd(Function_set_8bit); /* Select 8-bit Mode of LCD */
		SysTick_Wait_1ms(10);
		LCD_Cmd(DisplayOn);  /* Enable Display and cursor blinking */
		SysTick_Wait_1ms(10);
		LCD_Cmd(0x01); /* clear whatever is written on display */
		
		walked_dist = distance(31.279940,31.277896,30.064716,30.064001);// walked distance in meters
		if(walked_dist>=100){
			lcd_tracking1(walked_dist);
			led_on();
	}
	}
