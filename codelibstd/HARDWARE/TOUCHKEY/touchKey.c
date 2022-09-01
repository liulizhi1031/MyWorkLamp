#include "touchKey.h"
#include "delay.h"

int touchkey_Init(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;							// PB8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 				// 设置成浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					// 速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);								// 初始化GPIO
	
	return 0;
} 










