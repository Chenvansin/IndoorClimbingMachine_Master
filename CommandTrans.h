#ifndef __COMMANDTRANS_H__
#define __COMMANDTRANS_H__

#include "stm32f10x.h"
#include <stdio.h>
void COM_USART_Config(void);


void Usart4_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart4_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart4_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

#endif 
