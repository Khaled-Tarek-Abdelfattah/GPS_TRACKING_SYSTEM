#include "lcd_cmd_data.h"


void LCD_Data(uint8_t D){
		GPIO_PORTB_DATA_R = D;
		GPIO_PORTA_DATA_R|=0x40;//Rs to high
		GPIO_PORTA_DATA_R&=~0x20;//Rw to low
		GPIO_PORTA_DATA_R|=0x80;//enable to high
		SysTick_Wait_1ms(10);
		GPIO_PORTA_DATA_R&=~0x80;//enable to low
		SysTick_Wait_1ms(10);
}
void LCD_Cmd(uint8_t cmd){
		GPIO_PORTB_DATA_R = cmd;    //set PB7-0 as the passed command to the function
    GPIO_PORTA_DATA_R&=~0x40;//Rs to low
		GPIO_PORTA_DATA_R&=~0x20;//Rw to low
		GPIO_PORTA_DATA_R|=0x80;//enable tcho high
    SysTick_Wait_1ms(10);        //short delay
    GPIO_PORTA_DATA_R&=~0x80;//enable to low
	}