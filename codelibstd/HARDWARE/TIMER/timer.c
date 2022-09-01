#include "timer.h"
#include "usart.h"

unsigned char g_timerstatus = 0;

///< 主定时器，频率为100Hz
void TIM3_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 		

	TIM_TimeBaseStructure.TIM_Prescaler = 72; 
	TIM_TimeBaseStructure.TIM_Period = 10000; 												 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				
 
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  				
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  		
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 				
	NVIC_Init(&NVIC_InitStructure);  		
	
	TIM_Cmd(TIM3, ENABLE);  														 
}

/// TIM3中断处理函数
void TIM3_IRQHandler(void)   										
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 				
	{
		g_timerstatus = 1;	
			
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  			
	}
}












