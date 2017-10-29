#include "encoder.h"
#include "misc.h"

static void TIM4_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM4_Mode_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;   	

	//PB6 ch1  A,PB7 ch2  B
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//ʹ��TIM4ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOAʱ��
	
	GPIO_StructInit(&GPIO_InitStructure);//��GPIO_InitStruct�еĲ�����ȱʡֵ����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//PA6 PA7��������	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                           

	TIM4_NVIC_Config();

	TIM_DeInit(TIM4);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Period = 599*4;  //�趨��������װֵ   TIMx_ARR = 599*4
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //TIM3ʱ��Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;//����ʱ�ӷָ� T_dts = T_ck_int	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ��� 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
                 
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3�������½�������
	TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
	TIM_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲��� 
	TIM_ICInit(TIM4, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM4
	
//	TIM_ARRPreloadConfig(TIM4, ENABLE);//ʹ��Ԥװ��
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//���TIM4�ĸ��±�־λ
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//���и����ж�
	//Reset counter
	TIM4->CNT = 0;//

	TIM_Cmd(TIM4, ENABLE);   //����TIM4��ʱ��

}


void TIM4_Init(void)
{
  TIM4_Mode_Config();
}

