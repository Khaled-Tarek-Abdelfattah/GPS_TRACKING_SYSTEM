#include "lcd_tracking.h"
void lcd_tracking1(uint32_t walked_dist)
{
			uint32_t hundreds = walked_dist/100;
		uint32_t tens = walked_dist - (hundreds*100);
		 uint32_t tens1 = tens/10;
			uint32_t ones = tens - (tens1*10);


		SysTick_Wait_1ms(10);
		LCD_Data(hundreds+48);
		SysTick_Wait_1ms(10);
		LCD_Data(tens1+48);
		SysTick_Wait_1ms(10);
		LCD_Data(ones+48);
}