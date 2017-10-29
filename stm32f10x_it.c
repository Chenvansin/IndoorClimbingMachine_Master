/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
//#include "sdio.h"
//#include "led.h"
//#include "SysTick.h"
//#include "usb_lib.h"
#include "usb_istr.h"
//#include "RS485.h"
#include "bsp.h"
#include "tim.h"
#include "bsp_GeneralTim.h" 
#define ERR_INFO "\r\nEnter HardFault_Handler, System Halt.\r\n"
extern void TimingDelay_Decrement(void);
extern void ChangeFlag(void);
extern void Start(void);
extern volatile uint32_t pulse_tim_cnt;
extern volatile uint32_t pulse_cnt;
extern void bsp_RunPer10ms(void);
extern uint8_t speed_flag;
extern volatile uint32_t time;
extern uint8_t	overcount;

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
	const char *pError = ERR_INFO;
  uint8_t i;

  for (i = 0; i < sizeof(ERR_INFO); i++)
  {
     USART1->DR = pError[i];
     while ((USART1->SR & USART_FLAG_TC) == (uint16_t)RESET);
  }
	
	__set_FAULTMASK(1);    //soft_reset.
	NVIC_SystemReset();
	
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
//	static uint32_t s_count = 0;
	TimingDelay_Decrement();
	
//	if(++s_count >= 10000 && speed_flag == 1)
//	{
//		s_count = 0;

//		bsp_RunPer10ms();	/* 每隔10ms调用一次此函数，此函数在 bsp.c */
//	}

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/
void SDIO_IRQHandler(void)
{
	SD_ProcessIRQSrc();
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	USB_Istr();
}

void USB_HP_CAN1_TX0_IRQHandler(void)
{
	CTR_HP();
}

//void TIM6_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET)
//	{
//		pulse_tim_cnt++;
//		if(pulse_tim_cnt == 74)
//			pulse_cnt++;
//		TIM_ClearITPendingBit(TIM6,TIM_FLAG_Update);
//	}
//}

void  GENERAL_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( GENERAL_TIM, TIM_IT_Update) != RESET ) 
	{	
		TIM_ClearITPendingBit(GENERAL_TIM , TIM_FLAG_Update);  		 
		overcount++; //ms
	}	


}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
