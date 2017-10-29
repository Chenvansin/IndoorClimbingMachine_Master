#ifndef __RS485_H__
#define __RS485_H__

#include "stm32f10x.h"
#include <stdio.h>

static void RS485_Delay(__IO uint32_t nTime)
{
	for(;nTime != 0;nTime--);
}

#define RS485_RX_EN RS485_Delay(1000);GPIO_ResetBits(GPIOC,GPIO_Pin_2);RS485_Delay(1000);
#define RS485_TX_EN GPIO_SetBits(GPIOC,GPIO_Pin_2);RS485_Delay(1000);

void RS485_USART_Config(void);
void RS485_Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
void RS485_Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);

void RS485_Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void RS485_Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);


#endif

