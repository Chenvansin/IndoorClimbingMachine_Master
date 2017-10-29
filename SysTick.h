#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_10us(__IO uint32_t nTime);
#endif
