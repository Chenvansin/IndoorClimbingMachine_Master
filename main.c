#include "bsp.h"

#define GET_ARRAY_LEN(array,len) {len = (sizeof(array) / sizeof(array[0]));}
#define CMD_BUFFER_LEN 255
#define test_mode 1
// 0 --- test
// 1 --- normal 
#if !test_mode
static uint8_t debug_flag1 = 0;
static uint8_t debug_flag2 = 0;
static uint8_t debug_flag3 = 0;
static uint8_t debug_flag4 = 0;
static uint8_t debug_flag5 = 0;
static uint8_t check_flag = 0;
#endif

FATFS fs;
FIL fnew;

FRESULT res_sd;
UINT fnum;
BYTE ReadBuffer[2048];
int Speed_Buf[6] = {7296,3749,2430,1823,1499,1140};
char rx_buf2[256];	
char* Tempbuff[256];
int i;
int j = 0;
u16 tim ;
u16 delta_tim;
u16 tim_ls;
int amplitude_pulse;
double amplitude_angle;
uint8_t overcount = 0;
static char a_dir = 0;
static float a_value = 0;
uint8_t pause_flag = 0;
uint8_t restart_flag = 0;
uint8_t resume_flag = 0;
uint8_t Single_flag = 0;
uint8_t Double_flag = 0;
static uint8_t motor_flag = 0;
static uint8_t prepare_flag = 0;
static uint8_t connect_flag = 0;
static uint8_t file_send_flag = 0;
uint8_t enter_flag = 0;
 uint8_t End_flag = 0;
static uint8_t buf_size = 2;
int num;
int num2 = 0;
uint16_t lenRx;
int page;
int tCount;
extern int32_t count;
extern uint8_t reset_done;
uint16_t ARR;
uint8_t  PSC;
char ch;
//static uint8_t motor_dir = 0;
char* substring(char * ch, long pos , long length);
void SoftReset(void);
void Set_direction(void);
void Enter_stop(void);
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
//static uint8_t Pulse_count=  0;
void Delay(__IO u32 nCount); 
extern void SysTick_Delay_Us( __IO uint32_t us);
extern void SysTick_Delay_Ms( __IO uint32_t ms);
extern void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
uint16_t encoder_cnt_p = 0;
uint16_t encoder_cnt = 0;
static int16_t encoder_roll = 0;
static int16_t encoder_roll_cnt = 0;
uint16_t encoder_current_value = 0;
double climb_height = 0;
volatile uint32_t pulse_tim_cnt;
volatile uint32_t pulse_cnt;
uint8_t reset_push = 2;
u16 encoder_count;
u16 encoder_count_0;
u16 delta_count;
int16_t speed;
u16 encoder_count_ls;
int16_t speed_ls;
//float max_speed;
uint8_t speed_flag = 0;
uint32_t time = 0; // ms 计时变量 
void bsp_RunPer10ms(void);

/* DEBUG */
//uint8_t bit1,bit2,bit3;
/*---------------------------*/

int main(void)
{
	USART1_Config(); //USART1
	
	USART3_Config(); //USART3
	
	SysTick_Init(); //Delay_us(nTime)
	
	TIM4_Init();		//Encoder_Timer
	
	Set_System();		//MAL_Config();
	
	Set_USBClock();	//USBClock = 72/1.5 = 48MHz
	
	USB_Interrupts_Config();	//NVIC Config (include SDIO.EXTI
	
	valve_ctrl_IO_config();
	
	IO_Config(); //led exti(button)

#if test_mode
	USB_Init(); //USB_Init();
	USART_printf(USART3,"\rwelcome ...\r\n");
	USART_printf(USART3,"\rNow Loading ...\r\n");
	Delay_10us(1000);
	USART_printf(USART3,"\r0x81 to check config...\r\n");
#endif		

#if !test_mode
	printf("test_mode\r\n"); // normal running will not reach here

//Debug test code.

	TIM_DeInit(TIM3);

while(1)
{
	if(debug_flag1 == 1)
	{
		valve2_off();
					GPIO_ResetBits(GPIOB,GPIO_Pin_0); 							//Close EN config.(or reset GPIO as Reset.)

		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
		{			
			printf("\r\nvalve2_off\r\n");
			valve1_on();
			printf("\r\nvalve1_on\r\n");

			debug_flag1 = 0;
		}
	}
	
	if(debug_flag2 == 1)
	{		
		valve1_off();
			GPIO_ResetBits(GPIOB,GPIO_Pin_0); 							//Close EN config.(or reset GPIO as Reset.)
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
		{
			printf("\r\nvalve1_off\r\n");
			valve2_on();
			printf("\r\nvalve2_on\r\n");
			debug_flag2 = 0;
		}	
	}		
	
	if(debug_flag3 == 1)
	{						
			printf("\r\nReset Motor Runnig...\r\n");
			GPIO_SetBits(GPIOB,GPIO_Pin_0);
//			
//			TIM3_C2_Mode_config(9120,35);									//startup motor2 for reset action in defalut speed.
//			TIM_SetCompare2(TIM3,ARR-200);								//open TIM3_C2 PWM---PA.7
			for(i = 0;i < 5;i++)
			{
				ARR = Speed_Buf[i];
				TIM3_C2_Mode_config(ARR,PSC);
				TIM_SetCompare2(TIM3,ARR-ARR/10);				//open TIM3_C2 PWM
				Delay_10us(50);
			}
			while(1);

		
//			TIM4_Init();		//Encoder_Timer
//			while(1)
//			{
//				count = TIM4->CNT/4;//获取计数值
//				Delay_10us(100000);//每隔1s打印一次编码器角度,用手去拨动编码器  使其慢速旋转
//				printf("count = %d\n",count);		
//				printf("roll = %d\n",encoder_roll);
//			}
		
		//		
//				while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8) == 1);
				check_flag = 1;
//				printf("\r\nCheck First BOARD...\r\n");

			if(check_flag)
			{
	      while(1)
				{				
					
//						count = TIM4->CNT/4;//获取计数值
//						Delay_10us(100000);//每隔1s打印一次编码器角度,用手去拨动编码器  使其慢速旋转
//						printf("count = %d\n",count);		
//						printf("roll = %d\n",encoder_roll);

					
							if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0)
						{
							GPIO_ResetBits(GPIOB,GPIO_Pin_14);			
							GPIO_ResetBits(GPIOB,GPIO_Pin_12);
							Delay(0x0FFFFF);
							GPIO_SetBits(GPIOB,GPIO_Pin_14);
							GPIO_SetBits(GPIOB,GPIO_Pin_12);
							printf("\r\nMagnet_double on\r\n");
							while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0);
						}
				
						if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
						{
							GPIO_ResetBits(GPIOC,GPIO_Pin_6);
							GPIO_ResetBits(GPIOB,GPIO_Pin_13);
			//				GPIO_ResetBits(GPIOB,GPIO_Pin_15);
							Delay(0x0FFFFF);
							GPIO_SetBits(GPIOC,GPIO_Pin_6);
							GPIO_SetBits(GPIOB,GPIO_Pin_13);
							GPIO_ResetBits(GPIOB,GPIO_Pin_15);
							Delay(0x0FFFFF);							
							GPIO_SetBits(GPIOB,GPIO_Pin_15);
							printf("\r\nMagnet_Single on\r\n");
							while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
						}
						
						if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)	
						{
							printf("\r\nCheck Second BOARD...\r\n");
//							debug_flag3 = 0;
							TIM_DeInit(TIM3);
							GPIO_ResetBits(GPIOB,GPIO_Pin_0); 			//Enable EN config.
							valve1_off();
							SysTick_Delay_Us(100000);
							valve2_on();
							
//							while(1)
//							{
								
//							count = TIM4->CNT/4;//获取计数值
//							Delay_10us(100000);//每隔1s打印一次编码器角度,用手去拨动编码器  使其慢速旋转
//							printf("count = %d\n",count);		
//							printf("roll = %d\n",encoder_roll);

								
							if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0)
								{
									valve2_off();
//									printf("%02X",0x8A);
									printf("\rUser Position is too low.\r\n");
//									printf("%02X",0x87);
									printf("\rpause now.\r\n");
									while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == 0);
									valve2_on();
									printf("\rresume now.\r\n");
								}	
								
							}
//						}								
	// Encoder Test		
	//			count = TIM4->CNT/4;//获取计数值
	//			Delay_10us(10000);//每隔1s打印一次编码器角度,用手去拨动编码器  使其慢速旋转
	//			printf("count = %d\n",count);		
	//			printf("roll = %d\n",encoder_roll);
			}
		}
	}	
	if(debug_flag4 == 1)
	{
//		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
//		{
			printf("\r\nAmplitude Motor Runnig...\r\n");
			TIM3_C1_Mode_config(9120,35);
			TIM_SetCompare1(TIM3,ARR-100);
			GPIO_SetBits(GPIOB,GPIO_Pin_1); // select direction
			while(1);
	}
	
	if(debug_flag5 == 1)
	{
//		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 1)
//		{
//			valve1_off();	
//			printf("\r\nvalve1_off...\r\n");
		while(1)
		{
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0)
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_14);			
//			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
			Delay(0x1FFFFF);
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
//			GPIO_SetBits(GPIOB,GPIO_Pin_12);
			printf("\r\nMagnet_4 on\r\n");
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 0);
			break;
		}
		else
			GPIO_SetBits(GPIOB,GPIO_Pin_14);
//		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0)
//		{
////			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
//			GPIO_ResetBits(GPIOB,GPIO_Pin_15);
//			Delay(0x1FFFFF);
////			GPIO_SetBits(GPIOB,GPIO_Pin_13);	
//			GPIO_SetBits(GPIOB,GPIO_Pin_15);
//			printf("\r\nMagnet_5 on\r\n");
////			GPIO_ResetBits(GPIOB,GPIO_Pin_14);			
////			GPIO_ResetBits(GPIOB,GPIO_Pin_12);
////			Delay(0x0FFFFF);
////			GPIO_SetBits(GPIOB,GPIO_Pin_14);
////			GPIO_SetBits(GPIOB,GPIO_Pin_12);
////			printf("\r\nMagnet_4 on\r\n");
//			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9) == 0);
//			break;
//		}
		
	}

//		}
//		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) == 1)	
//		{
//			valve2_off();
//			printf("\r\nvalve2_off...\r\n");
//			debug_flag5 = 0;
//		}
	}
}
#endif
//
	
	while(1)
	{
		if(connect_flag == 1)
		{
			while (bDeviceState != CONFIGURED);	 //usb config completed
				USART_printf(USART3,"\rUSB Config OK!\r\n");
			
			res_sd = f_mount(&fs,"0:",1); 			 //f_mount is necessary 
						
			if(res_sd == FR_NO_FILESYSTEM)
			{
				USART_printf(USART3,"\rSD card has no fatfs...\r\n");
				res_sd = f_mkfs("0:",0,0);
				
				if(res_sd == FR_OK)
				{
					USART_printf(USART3,"\rSD card fatfs create succeeded.\r\n");
					res_sd = f_mount(NULL,"0:",1);
					res_sd = f_mount(&fs,"0:",1);
				}
				else
				{
					USART_printf(USART3,"\rSD card fatfs create fail: %d\r\n",res_sd);
					while(1);
				}
			}
			else if(res_sd != FR_OK)
			{
				USART_printf(USART3,"\r\nSD card mount fail:%d\r\n",res_sd);
				while(1);
			}
			else
				USART_printf(USART3,"\r\nSD card mount succeeded\r\n");
			
			USART_SendData(USART1,0x81);	
			break;
		}
	};
	
	USART_printf(USART3,"\r0x82 to check file...\r\n");
	
	while(1)
	{
		if(file_send_flag == 1)
		{
			res_sd = f_open(&fnew,"0:Roraima.txt",FA_OPEN_EXISTING | FA_READ);
			if(res_sd == FR_OK)
			{			
				USART_SendData(USART1,0x82);	
				f_close(&fnew);
			}
//				printf("%02X",0x82);
			else
			{
				USART_SendData(USART1,0x03);	
//				printf("%02X",0x03);
				USART_printf(USART3,"\r\nSD card mount fail:%d\r\n",res_sd);
				while(1);
			}
			break;
		}
	};

	USART_printf(USART3,"\rEnter 01 for amplitude action.\r\n");
	
	while(1)						// waiting al motion instruction.
	{
		if(motor_flag == 1)
			break;
	}
	//-----v = 72000000 * 60 / ((psc + 1) * pluse * (arr + 1)) r/min -----------
	
	while(1)
	{
//		if(USART_RX_STA)
		if(rx_buf2[0] != 0)
		{
			PSC = 35;
			ARR = 3749; 			//20 r/min --- 533Hz
//			USART_printf(USART2,"Speed is :%f r/min.\r\n",72000000 * 60.0 / ((PSC + 1) * 400.0 * (ARR + 1)));
			USART_printf(USART3,"\r\n equivalent Angle is : %d\r\n",a_value);
//			USART_printf(USART3,"\r\n equivalent Angle is : %d\r\n",a_value);
			a_value = a_value * 0.1; 						//每个变幅单位0.1度
//			amplitude_angle = a_value * 40; 		//Alarm: const based on machine
			amplitude_pulse = a_value * 195;
			
			
			USART_printf(USART3,"Amplitude_angle is %d\r\n",amplitude_angle);

			if(a_dir)
				USART_printf(USART3,"Direction is clockwise\r\n");
			else	
				USART_printf(USART3,"Direction is anti-clockwise\r\n");
			
			Set_direction();			// set DIR 
			TIM3_C1_Mode_config(ARR,PSC);	//OPEN TIM3
			TIM_SetCompare1(TIM3,ARR-200);
	//-------------------------This Part Exist Bug-----------------------------------		
			for(i = amplitude_pulse;i > 0; i --)
			{
				SysTick_Delay_Us(1875); //0.001875s/pulse
				USART_printf(USART3,"Pulse rest: %d\r\n",i);
			}
	//------------------------------------------------------------------------------
			//motor working until pulse_cnt == amplitude_pulse	
			Enter_stop	();
			motor_flag = 0; //swtich rx_buf2 to rx_buf1
			USART_SendData(USART1,0x8C);	
			break;
		}
	}
	valve1_on();
	USART_printf(USART3,"\rEnter 0x83 for Pre-action.\r\n");
//--------------------------This Part of Reset Action-------------------------	
	while(1)
	{
		if(prepare_flag == 1)
		{	
			break;
		}
	};
	

	res_sd = f_open(&fnew,"0:Roraima.txt",FA_OPEN_EXISTING | FA_READ);
	if(res_sd == FR_OK)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
		GPIO_SetBits(GPIOB,GPIO_Pin_1); 						// select direction			

		for(i = 0;i < 6;i++)
		{
			ARR = Speed_Buf[i];
			TIM3_C2_Mode_config(ARR,PSC);
			TIM_SetCompare2(TIM3,ARR-ARR/10);				//open TIM3_C2 PWM
			Delay_10us(50);
		}
		
		res_sd = f_read(&fnew,ReadBuffer,sizeof(ReadBuffer),&fnum);		
		if(res_sd == FR_OK)
		{	
			// 8细分 (1600pulse/v) 
			USART_printf(USART3,"\r\nReset Motor Runnig...\r\n");
					
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4) == 1);
			USART_printf(USART3,"\r\nCheck First BOARD...\r\n");
			
			Tempbuff[0] = substring(ReadBuffer,0,8);
			num = atoi(Tempbuff[0]);		
			while(1)
			{			
					if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0)
					{
						USART_printf(USART3,"\r\nCheck Second BOARD...\r\n");
						USART_printf(USART3,"\r\nSwitch Now...\r\n");
						USART_SendData(USART1,0x84);	
						TIM_DeInit(TIM3);
						GPIO_ResetBits(GPIOB,GPIO_Pin_0); 			//Enable EN config.
						valve1_off();
						SysTick_Delay_Ms(500);
						valve2_on();
						break;
					}
			
					Tempbuff[count] = substring(ReadBuffer,(8*count),8);
					num = atoi(Tempbuff[count++]);	
					Magnet_WriteByte(num);
					USART_printf(USART3,"\rCOUNT IS : %d\r\n",count);
					
			}	

			USART_printf(USART3,"\rUser can start climbing...\r\n");						
			reset_done = 1;					
			count--;	
			
			TIM4_Init();
			SysTick_Delay_Ms(3000);			
		//	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == 0);		
			while(1)
			{	
				encoder_count = TIM4->CNT / 4;
				encoder_count_ls = encoder_count;
				SysTick_Delay_Ms(100);
				encoder_count = TIM4->CNT / 4;	

				if((encoder_count_ls != encoder_count) && End_flag != 1)
				{
					USART_SendData(USART1,0x85);
					USART_printf(USART3,"\rStart now.\r\n");
					break;
				}
			}

			TIM_DeInit(TIM4);					//Init TIM4
			lenRx = strlen(ReadBuffer)-2;
			while(1)
			{				
				TIM4_Init();
				for(;count < (lenRx/8);)
				{
					if(speed_flag == 0)
						bsp_RunPer10ms();
					
					Tempbuff[count] = substring(ReadBuffer,(8*count),8);
					num = atoi(Tempbuff[count++]);
		
					Magnet_WriteByte(num);
					USART_printf(USART3,"\rCOUNT IS : %d\r\n",count);
				}	
			}		
		}
		else
			USART_printf(USART3,"Reading file failed.\r\n");
	}
	else
		USART_printf(USART3,"Open file failed...\r\n");
	
	f_close(&fnew);
	f_mount(NULL,"0:",1);
	while(1);
}

void onReceivedEnd(void){
		valve2_off();
		USART_SendData(USART1,0x87);
		pause_flag = 1;	
		USART_SendData(USART1,0x8B);
		USART_printf(USART3,"\rClimbing has been ended.\r\n");
}

char* substring(char* ch, long pos , long length)
{
	char* pch = ch;
	char* subch = (char*)calloc(sizeof(char),length);
	int i;
	pch = pch + pos;
	for(i=0;i<length;i++)
	{
		subch[i] =  *(pch++);
	}
	subch[length] = '\0';
	return subch;
}

void Set_direction(void)
{
	if(a_dir)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_1);	
	}
	else
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);	
	}
}

void Enter_stop(void)
{
		TIM_DeInit(TIM3);
		USART_printf(USART3,"Stop now...\r\n");
		USART_printf(USART3,"-------------------------------------------------\r\n");
}


void USART1_IRQHandler(void) //DEBUG
{
	uint16_t num = 0;
	uint8_t flag_value;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)
	{
		if(motor_flag == 0)
		{
			num = 0;
			rx_buf[num] = USART_ReceiveData(USART1);
			flag_value = rx_buf[num++]; 
			switch(flag_value)
			{
				case 0x01:
					USART_printf(USART3,"Start Motor Operation...\r\n");
					motor_flag = 1;
				break;
				
				case 0x81: 		
					connect_flag = 1;
				break;
								
				case 0x82:
					file_send_flag = 1;
				break;
				
				case 0x83:
					prepare_flag = 1;
					USART_SendData(USART1,0x83);	
				break;
				
				case 0x86:
					pause_flag = 1;
				break;

				case 0x88:
					resume_flag = 1;
				break;
				
				case 0x89:
					End_flag = 1;
					USART_printf(USART3,"Receive End ...\r\n");
					onReceivedEnd();
				break;
								
				case 0x06:
					restart_flag = 1;
				break;
				
				case 0xFF:
					SoftReset();
				break;
//Debug instruct	
#if !test_mode				
				case 0xaa:
					debug_flag1 = 1;
					printf("\r\nValve_1 Test\r\n");
				break;
				
				case 0xbb:
					debug_flag2 = 1;
					printf("\r\nValve_2 Test\r\n");
				break;
				
				case 0xcc:
					debug_flag3 = 1;
					printf("\r\nReset Motor Test\r\n");
				break;
				
				case 0xdd:
					debug_flag4 = 1;
					printf("\r\namplitude Motor Test\r\n");
				break;
				
				case 0xee:
					debug_flag5 = 1;
					printf("\r\nvalve_mid_pos\r\n");
#endif
				
				default:
					break;
			}
		}
		else if(motor_flag == 1)
		{
			ch = USART_ReceiveData(USART1);
			
			rx_buf2[num2] = ch;
			if(num2 >= buf_size)
			{
				num2 = 0;
			}
			if(num2 < buf_size)
			{
				num2++;
			}
			
			a_dir = (rx_buf2[0] & 0x80) >> 7;
			a_value =(int)(rx_buf2[0] & 0x7f); 
		}
	}
}

void TIM4_IRQHandler(void)
{ 	
	if(TIM4->SR&0x0001)//溢出中断
	{
		encoder_roll++;
	}	
	TIM4->SR&=~(1<<0);//清除中断标志位 	
}

void SoftReset(void)
{
	__set_FAULTMASK(1);    //soft_reset.
	NVIC_SystemReset();
}

static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
        const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

        while ( *Data != 0)     // ????????????
        {                                                          
                if ( *Data == 0x5c )  //'\'
                {                                                                          
                        switch ( *++Data )
                        {
                                case 'r':                                                                  //???
                                        USART_SendData(USARTx, 0x0d);
                                        Data ++;
                                        break;

                                case 'n':                                                                  //???
                                        USART_SendData(USARTx, 0x0a);        
                                        Data ++;
                                        break;
                                
                                default:
                                        Data ++;
                                    break;
                        }                         
                }
                else if ( *Data == '%')
                {                                                                          //
                        switch ( *++Data )
                        {                                
                                case 's':                                                                                  //???
                                        s = va_arg(ap, const char *);
          for ( ; *s; s++) 
                                        {
                                                USART_SendData(USARTx,*s);
                                                while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
                                        Data++;
          break;

        case 'd':                                                                                //???
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
                                        {
                                                USART_SendData(USARTx,*s);
                                                while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
                                        Data++;
          break;
                                 default:
                                                Data++;
                                    break;
                        }                 
                } /* end of else if */
                else USART_SendData(USARTx, *Data++);
                while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
        }
}

void bsp_RunPer10ms(void)
{	
		encoder_count = TIM4->CNT / 4;
		encoder_count_ls = encoder_count;
//		USART_printf(USART3,"encoder_count_ls is : %d\n",encoder_count_ls);
		SysTick_Delay_Ms(500);
		encoder_count = TIM4->CNT / 4;
//		USART_printf(USART3,"encoder_count is : %d\n",encoder_count);
		delta_count = encoder_count - encoder_count_ls;
//		USART_printf(USART3,"encoder increment count is : %d\n",encoder_count - encoder_count_ls);
//		USART_printf(USART3,"Receive Encoder value :%d ...\r\n",encoder_count);

		if(delta_count < 0)
		{
			delta_count = 600 + delta_count;
			Usart_SendByte(USART1,delta_count);
		}
		else if(delta_count > 0)
			Usart_SendByte(USART1,delta_count);
}
