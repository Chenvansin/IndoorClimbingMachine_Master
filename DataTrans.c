/*
*
*------This File Used For Control Electric Magnet Push-----
*------Temp test based on LED status instand of Magnet-----
*------This part may adding a acknowledge after each action done---
*
*/
#include "bsp.h"
extern void bsp_RunPer10ms(void);
extern void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
//static uint8_t switch_flag = 1;
uint8_t bit[8];
uint8_t group1;
uint8_t group2;
int32_t count = 0;
uint8_t reset_done = 0;
extern u16 encoder_count;
extern u16 encoder_count_ls;
extern uint8_t pause_flag;
extern uint8_t End_flag;
extern uint8_t enter_flag;
extern uint8_t Single_flag;
extern uint8_t Double_flag;
void Delay(__IO u32 nCount); 
extern void SysTick_Delay_Ms( __IO uint32_t ms);
void Magnet_WriteByte(unsigned int byte)
{

	int i;
					
//
	/*	Frame Protocol 
	**	    | (FORCE) | (Mag-Valve) (1) (2) (3) (4) (5) |  
	**			|    1    |   X2   X1    -   -   -   -   -  | 
	**-----------<ReadBuff to num>---------------
	** 1 1 1 - - - - - 
	** Meaning: Debug Mode
	** 1 1 0 - - - - -
	** Meaning: Start Mode
	** 1 0 0 - - - - - 
	** Meaning: STOP Mode
	** 1 0 1 - - - - - 
	** Meaning: Run Mode
	*/
	USART_printf(USART3,"\rEnter now.\r\n");
							
	for(i=7;i>0;i--)
	{
		bit[i] = byte%10;
		byte /= 10;
	}
	
	while(1)
	{
		group1 = bit[3] * 100 + bit[5] * 10 + bit[7];
		group2 = bit[4] * 10 + bit[6];
		
//		if(reset_done == 1)
//		{
//			bsp_RunPer10ms();
//		}
		while(1)
		{
//			if(End_flag == 1)
//			{
//				valve2_off();
//				USART_SendData(USART1,0x8B);
//				USART_printf(USART3,"\rClimbing has been ended.\r\n");
//				while(1);		
//			}
					
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0 && reset_done == 1)
			{
				USART_SendData(USART1,0x8A);
				USART_printf(USART3,"\rUser Position is too low.\r\n");
				SysTick_Delay_Ms(100);
				USART_SendData(USART1,0x87);
				USART_printf(USART3,"\rpause now.\r\n"); 					//enter pause
				valve2_off();
				while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0);
				SysTick_Delay_Ms(300);
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0)
				{
					valve2_off();
					while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0);
				}			
				valve2_on();

				
				while(1)
				{
					encoder_count = TIM4->CNT / 4;
					encoder_count_ls = encoder_count;
					SysTick_Delay_Ms(100);
					encoder_count = TIM4->CNT / 4;									
					
					if(encoder_count == encoder_count_ls && pause_flag == 0) //enter afk and continue loop (in the next loop, detecting AFK_resume / pause_end / B8 / B9)
					{
						pause_flag = 1;					
						USART_SendData(USART1,0x87);
						USART_printf(USART3,"\rAway from machine now...\r\n");
					}

					if(encoder_count != encoder_count_ls && pause_flag == 1) // resume from afk and break
					{
						USART_SendData(USART1,0x85);
						USART_printf(USART3,"\rresume from AFK status...\r\n");
						pause_flag = 0;					
						break;
					}		
					else if(encoder_count != encoder_count_ls && pause_flag == 0) // resume from pause and break
					{
						USART_SendData(USART1,0x85);
						USART_printf(USART3,"\rresume from pause status...\r\n");				
						break;
					}
					
//					if(End_flag == 1 && pause_flag == 0)
//					{
//						while(1)
//						{
//							if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0)
//							{
//								valve2_off();
//								USART_SendData(USART1,0x8B);
//								USART_printf(USART3,"\rClimbing has been ended.\r\n");
//								while(1);
//							}
//						}
//					}
//					else if(End_flag == 1 && pause_flag == 1)
//					{
//						valve2_off();
//						USART_SendData(USART1,0x8B);
//						USART_printf(USART3,"\rClimbing has been ended.\r\n");
//						while(1);					
//					}
				}

				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
				{
					Single_flag = 1;
					break;
				}
					
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0)
				{
					Double_flag = 1;
					break;
				}
			
			}
			else
				break;		
		}
		
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0 && reset_done == 0)
			break;

		if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0 || Single_flag == 1) && switch_flag == 0)
		{
			switch(group1)
			{ 
				case 111:
					GPIO_ResetBits(GPIOC,GPIO_Pin_6);			
					GPIO_ResetBits(GPIOB,GPIO_Pin_13);
					Delay(0x0FFFFF);
					GPIO_SetBits(GPIOC,GPIO_Pin_6);
					GPIO_SetBits(GPIOB,GPIO_Pin_13);	
					GPIO_ResetBits(GPIOB,GPIO_Pin_15);
					Delay(0x0FFFFF);
					GPIO_SetBits(GPIOB,GPIO_Pin_15);
					USART_printf(USART3,"Push: 1\r\n");
					USART_printf(USART3,"Push: 3\r\n");				
					USART_printf(USART3,"Push: 5\r\n");
					while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
					switch_flag = 1;
					Single_flag = 0;
					break;						

				case 110:
					GPIO_ResetBits(GPIOC,GPIO_Pin_6);			
					GPIO_ResetBits(GPIOB,GPIO_Pin_13);
					Delay(0x0FFFFF);
					GPIO_SetBits(GPIOC,GPIO_Pin_6);
					GPIO_SetBits(GPIOB,GPIO_Pin_13);
					USART_printf(USART3,"Push: 1\r\n");
					USART_printf(USART3,"Push: 3\r\n");
					while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
					switch_flag = 1;
					Single_flag = 0;
					break;						

				case 101:
					GPIO_ResetBits(GPIOC,GPIO_Pin_6);			
					GPIO_ResetBits(GPIOB,GPIO_Pin_15);
					Delay(0x0FFFFF);
					GPIO_SetBits(GPIOC,GPIO_Pin_6);
					GPIO_SetBits(GPIOB,GPIO_Pin_15);
					USART_printf(USART3,"Push: 1\r\n");
					USART_printf(USART3,"Push: 5\r\n");
					while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
					switch_flag = 1;
					Single_flag = 0;
					break;							

				case 100:
								GPIO_ResetBits(GPIOC,GPIO_Pin_6);
								Delay(0x0FFFFF);
								GPIO_SetBits(GPIOC,GPIO_Pin_6);	
								USART_printf(USART3,"Push: 1\r\n");
								while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
								switch_flag = 1;
								Single_flag = 0;
								break;						

				case 11:
								GPIO_ResetBits(GPIOB,GPIO_Pin_13);			
									GPIO_ResetBits(GPIOB,GPIO_Pin_15);
									Delay(0x0FFFFF);
									GPIO_SetBits(GPIOB,GPIO_Pin_13);
									GPIO_SetBits(GPIOB,GPIO_Pin_15);	
									USART_printf(USART3,"Push: 3\r\n");
									USART_printf(USART3,"Push: 5\r\n");
									while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
									switch_flag = 1;
									Single_flag = 0;
									break;

				case 10:
											GPIO_ResetBits(GPIOB,GPIO_Pin_13);
									Delay(0x0FFFFF);
									GPIO_SetBits(GPIOB,GPIO_Pin_13);				
									USART_printf(USART3,"Push: 3\r\n");
									while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
									switch_flag = 1;
									Single_flag = 0;
									break;							

				case 1:
											GPIO_ResetBits(GPIOB,GPIO_Pin_15);
									Delay(0x0FFFFF);
									GPIO_SetBits(GPIOB,GPIO_Pin_15);	
									USART_printf(USART3,"Push: 5\r\n");
									while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
									switch_flag = 1;
									Single_flag = 0;
									break;							
		
				case 0:
											GPIO_SetBits(GPIOC,GPIO_Pin_6);
									GPIO_SetBits(GPIOB,GPIO_Pin_13);
									GPIO_SetBits(GPIOB,GPIO_Pin_15);
									USART_printf(USART3,"No Push\r\n");
									while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
									switch_flag = 1;
									Single_flag = 0;
								break;							


			}
			break;
		}
		else if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0 || Single_flag == 1) && switch_flag == 1)
		{
			USART_printf(USART3,"\rError touch of single.\r\n");			
			USART_printf(USART3,"\rError count is %d.\r\n",--count);
			Single_flag = 0;
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
			break;
		}

			if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0 || Double_flag == 1)&& switch_flag == 1)
			{
				switch(group2)
				{
					case 11:
								GPIO_ResetBits(GPIOB,GPIO_Pin_14);			
								GPIO_ResetBits(GPIOB,GPIO_Pin_12);
								Delay(0x0FFFFF);
								GPIO_SetBits(GPIOB,GPIO_Pin_14);
								GPIO_SetBits(GPIOB,GPIO_Pin_12);
								USART_printf(USART3,"Push: 2\r\n");
								USART_printf(USART3,"Push: 4\r\n");
								while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0);
								switch_flag=0;
								Double_flag	= 0;		
								break;

					case 10:
								GPIO_ResetBits(GPIOB,GPIO_Pin_12);
								Delay(0x0FFFFF);
								GPIO_SetBits(GPIOB,GPIO_Pin_12);	
								USART_printf(USART3,"Push: 2\r\n");
								while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0);
								switch_flag=0;
								Double_flag	= 0;		
								break;

					case 1:
								GPIO_ResetBits(GPIOB,GPIO_Pin_14);
								Delay(0x0FFFFF);
								GPIO_SetBits(GPIOB,GPIO_Pin_14);
								USART_printf(USART3,"Push: 4\r\n");
								while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0);
								switch_flag=0;
								Double_flag	= 0;		
								break;

					case 0:
								GPIO_SetBits(GPIOB,GPIO_Pin_12);		
								GPIO_SetBits(GPIOB,GPIO_Pin_14);	
								while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0);
								USART_printf(USART3,"No Push...\r\n");
								switch_flag=0;
								Double_flag	= 0;		
								break;						
						}
					break;
				}
		else if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0 || Double_flag == 1)&& switch_flag == 0)
				{
					count--;
					USART_printf(USART3,"\rError touch of double.\r\n");
					USART_printf(USART3,"\rError count is %d.\r\n",count);
					Double_flag	= 0;		
					while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0);
					break;
				}
			}
	}


void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
