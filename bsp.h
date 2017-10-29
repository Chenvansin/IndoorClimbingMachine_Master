#ifndef __BSP_H__
#define __BSP_H__

#include "stm32f10x.h"
#include "usart.h"
#include "sdio.h"
#include "ff.h"
#include "led.h"
#include "DataTrans.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "SysTick.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "valve.h"
#include "CommandTrans.h"
#include "tim.h"
#include "encoder.h"
#include "bsp_GeneralTim.h" 
#include "bsp_exti.h"

static uint16_t rx_buf[1024];
//static int32_t count = 0;
static uint8_t reset_flag = 0;
static uint8_t switch_flag = 0;

#endif
