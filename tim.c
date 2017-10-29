#include "tim.h"

//static void TIM6_NVIC_Config(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//	NVIC_Init(&NVIC_InitStructure);
//}

void TIM3_C1_Mode_config(uint16_t arr, uint8_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//TIM3 CH1 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//TIM3 CH2 	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimBaseInitStructure.TIM_ClockDivision = 0x0;
	TIM_TimBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimBaseInitStructure.TIM_Period = arr;
	TIM_TimBaseInitStructure.TIM_RepetitionCounter=0;	
	TIM_TimBaseInitStructure.TIM_Prescaler = psc;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = 499;
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
//	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_Cmd(TIM3,ENABLE);
}

void TIM3_C2_Mode_config(uint16_t arr, uint8_t psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//TIM3 CH1 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//TIM3 CH2 	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimBaseInitStructure.TIM_ClockDivision = 0x0;
	TIM_TimBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimBaseInitStructure.TIM_Period = arr;
	TIM_TimBaseInitStructure.TIM_RepetitionCounter=0;	
	TIM_TimBaseInitStructure.TIM_Prescaler = psc;
	
	TIM_TimeBaseInit(TIM3,&TIM_TimBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = 499;
	
//	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_Cmd(TIM3,ENABLE);
}

//void TIM6_Mode_config(void)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
//	TIM_TimeBaseStructure.TIM_Period = 0;
//	TIM_TimeBaseStructure.TIM_Prescaler = 71;
//	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure);
//	
//	TIM_ClearFlag(TIM6,TIM_FLAG_Update);
//	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
//	TIM_Cmd(TIM6,DISABLE);
//}

//void TIM6_Config(void)
//{
//	TIM6_NVIC_Config();
//	TIM6_Mode_config();
//}
