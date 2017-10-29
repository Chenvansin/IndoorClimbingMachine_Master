#include "valve.h"
#include "SysTick.h"

//
//	Reset ----> FF00 -----> ON
//
//	Set ------> 0000 -----> OFF
//
//

void valve_ctrl_IO_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_4);
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
//	GPIO_SetBits(GPIOB,GPIO_Pin_5);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void valve1_on(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);
//	Delay_10us(50000);
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
//	Delay_10us(50000);
	
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
//	Delay_10us(50000);
}

void valve1_off(void)  
{
	GPIO_SetBits(GPIOC,GPIO_Pin_2);
//	Delay_10us(50000);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
//	Delay_10us(50000);
}

void valve2_on(void)
{
	GPIO_ResetBits(GPIOC,GPIO_Pin_4);
//	Delay_10us(50000);
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
//	Delay_10us(50000);
	
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
//	Delay_10us(50000);
}

void valve2_off(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_4);
//	Delay_10us(50000);
//	GPIO_SetBits(GPIOB,GPIO_Pin_0);
//	Delay_10us(50000);
	
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
//	Delay_10us(50000);
}


//}
