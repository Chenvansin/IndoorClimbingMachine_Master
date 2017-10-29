#ifndef __LED_H__
#define __LED_H__
#include "stm32f10x.h"

#define LED_G(a) if(a) \
		GPIO_SetBits(GPIOB,GPIO_Pin_0);\
		else \
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);

	
#define LED_B(a) if(a) \
		GPIO_SetBits(GPIOB,GPIO_Pin_1);\
		else \
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	
#define LED_R(a) if(a) \
		GPIO_SetBits(GPIOB,GPIO_Pin_5);\
		else \
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);

void IO_Config(void);

#endif
