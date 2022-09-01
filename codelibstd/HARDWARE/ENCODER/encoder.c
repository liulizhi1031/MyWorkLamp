#include "encoder.h"
#include "delay.h"
#include "usart.h"

/// A -> PA0 -> TIM2_1
/// B -> PA1 -> TIM2_2
/// K -> PA2

int encoder_Init(void)
{
 	GPIO_InitTypeDef 			GPIO_InitStructure;
	NVIC_InitTypeDef  			NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_ICInitTypeDef 			TIM_ICInitStructure; 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;	// PA0 PA1 PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 				// 设置成浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					// 速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);								// 初始化GPIO

	TIM_TimeBaseStructure.TIM_Prescaler = 0;							// 定时器分频
	TIM_TimeBaseStructure.TIM_Period = 2000;							// 自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;				// 设置时间分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			// 向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;      				// TIM2 通道 1    			      
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	      	// 输入捕获上升沿
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   	// 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	          	// 每探测到一个边沿执行一次
	TIM_ICInitStructure.TIM_ICFilter = 5;                            	// 配置输入滤波器
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;                  	// TIM2 通道 1  
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	     	// 输入捕获上升沿
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;   	// 
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         	// 每探测到一个边沿执行一次    
	TIM_ICInitStructure.TIM_ICFilter = 5;                             	// 配置输入滤波器
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
		
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;                  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;     
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;            
	NVIC_Init(&NVIC_InitStructure);                              
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                       
	TIM_Cmd(TIM2, ENABLE);
	
	/// 初始化编码器计数器
	TIM_SetCounter(TIM2, 1000);
	
	return 0;
}


void TIM2_IRQHandler(void)                         
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)      
	{  
			
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);       
}











