#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED
#include "tm4c123gh6pm.h"
#include "stdint.h"

void SysTick_Init();
void SysTick_Wait(uint32_t delay);
void SysTick_Wait_1ms(uint32_t delay);

#endif