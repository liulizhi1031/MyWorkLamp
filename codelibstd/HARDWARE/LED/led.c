#include "led.h"
#include "sys.h"
#include "usart.h"

/**
 * frequence 30kHz
 * LED1 PB6 T4_1
 * LED2 PB7 T4_2
 */
int led_init(void)
{
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;

	// 频率=时钟频率/(psc*arr)
	int psc=2;
	int arr=1200;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        			//复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				//速度50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);             
		
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  					//定时器分频
	TIM_TimeBaseStructure.TIM_Period = arr;   						//自动重装载值
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//设置分频因子
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				//选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  
        
	TIM_CtrlPWMOutputs(TIM4,ENABLE);  								//MOE 主输出使能
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);   			//预装载使能 
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);   			//预装载使能 
	TIM_ARRPreloadConfig(TIM4,ENABLE);								//ARPE使能 
	TIM_Cmd(TIM4, ENABLE);  
	
	TIM_SetCompare1(TIM4, 0);					
	TIM_SetCompare2(TIM4, 0);
	
	return 0;
}

/**
 * 
 */
int led_ctrl(short bright, short color)
{
	int ccr1 = (bright * 1200) / 100;
	int ccr2 = ccr1;
	int offset = 0;
	
	if((bright > 100)||(color > 100)||(bright < 0)||(color < 0))
		return -1;
	
	offset = (((color - 50) * 1200) / 100);
	
	ccr1 += offset;
	if(ccr1 > 1200)
		ccr1 = 1200;
	else if(ccr1 < 0)
		ccr1 = 0;
	
	ccr2 -= offset;
	if(ccr2 > 1200)
		ccr2 = 1200;
	else if(ccr2 < 0)
		ccr2 = 0;
	
	TIM_SetCompare1(TIM4, ccr2);	
	
	TIM_SetCompare2(TIM4, ccr1);
	
	return 0;
}






















