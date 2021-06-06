#ifndef LCD_CMD_DATA_H_INCLUDED
#define LCD_CMD_DATA_H_INCLUDED
#include "tm4c123gh6pm.h"
#include "delay.h"
#include "stdint.h"

void LCD_Data(uint8_t D);
void LCD_Cmd(uint8_t cmd);

#endif