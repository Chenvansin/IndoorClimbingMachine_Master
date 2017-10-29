#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Config(void);
void USART3_Config(void);
void Usart1_Nvic_Config(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, unsigned char ch);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
#endif
