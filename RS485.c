#include "RS485.h"

//485module pins include VCC GND RXD TXD, EN Pin can be ignore.
//In this project 485 module using USART1(PA.9 PA.10) and a 'enable' pin(PA.2)
//It is not the finial connection method.
/*
*
*									PA.9  ---- TX
*									PA.10 ---- RX	
*									PA.2  ---- EN
*							
*									needed extend pin:
*									GND
*									VCC (perfer 5V, also 3.3V is avaliable but not the best)
*
*		2017/3/1 
*		YcHepth
*/


static void RS485_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	
	NVIC_Init(&NVIC_InitStructure);
}

void RS485_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//PA.9 TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//PA.10 RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//RS485 EN
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	RS485_NVIC_Config();
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	USART_Cmd(USART1,ENABLE);
	
	RS485_RX_EN;
}

void RS485_Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
	USART_SendData(pUSARTx,ch);
	
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE) == RESET);
}

void RS485_Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;
	
	RS485_TX_EN;
	
	for(i = 0; i< num ; i++)
	{
		USART_SendData(pUSARTx,array[i]);
	}
	
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC) == RESET);

	RS485_RX_EN;
}

void RS485_Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k = 0;
	RS485_TX_EN;
	
	do
	{
		RS485_Usart_SendByte(pUSARTx,*(str + k));
		k++;
	}while(*(str + k) != '\0');
	
	while(USART_GetFlagStatus( pUSARTx,USART_FLAG_TC) == RESET);
	
	RS485_RX_EN;
}

void RS485_Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	RS485_TX_EN;

	temp_h = (ch & 0xff00) >> 8;
	
	temp_l = ch&0xff;
	
	USART_SendData(pUSARTx,temp_h);
	while(USART_GetFlagStatus( pUSARTx,USART_FLAG_TXE) == RESET);
	
	USART_SendData(pUSARTx,temp_l);
	while(USART_GetFlagStatus( pUSARTx,USART_FLAG_TXE) == RESET);
}

