#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "timer.h"
#include "led.h"
#include "oled.h"
#include "touchKey.h"
#include "encoder.h"
#include "beep.h"
#include "eeprom.h"

extern unsigned char g_timerstatus;						///< 主定时器触发状态

unsigned short encoderkeycnt = 0;						///< 编码器按键计数
unsigned char encoderkeystatus = 0;						///< 编码器按键状态
short encoderchange = 0;								///< 编码器计数改变值

unsigned short touchkeycnt = 0;							///< 触摸按键计数
unsigned char touchkeystatus = 0;						///< 触摸按键状态

unsigned short ledstatus = 0;							///< LED开关状态
short ledbright = 50;									///< LED亮度值
short ledcolor = 50;									///< LED色温

/// 编码器按键状态更新
void EncoderKeyRrefresh(void)							
{
	if(KEY_ENCODER == 1)
	{
		encoderkeycnt = 0;
		if(encoderkeystatus == 1)
			encoderkeystatus = 2;
		else if(encoderkeystatus == 2)
			encoderkeystatus = 0;
	}
	else
		encoderkeycnt++;
	
	if(encoderkeycnt > 10)
	{
		encoderkeystatus = 1;
	}	
}

/// 触摸按键状态更新
void TouchKeyRrefresh(void)								
{
	if(KEY_TOUCH == 1)
	{
		touchkeycnt = 0;
		if(touchkeystatus == 1)
			touchkeystatus = 2;
		else if(touchkeystatus == 2)
			touchkeystatus = 0;
	}
	else
		touchkeycnt++;
	
	if(touchkeycnt > 10)
	{
		touchkeystatus = 1;
	}
}

int main(void)
{	  	 
	delay_init();	    			    				///< 初始化延时函数	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		///< 设置中断优先级分组2
	uart_init(115200);     			    				///< 初始化串口1，用于调试
	TIM3_Init();										///< 主定时器开启，100Hz
	led_init();											///< LED初始化
	OLED_Init();										///< OLED初始化
	touchkey_Init();									///< 触摸按键初始化
	encoder_Init();										///< 编码器初始化

	/// 显示Logo
	OLED_ShowString(10,10,"WorkLamp V0.1");
	OLED_ShowString(5,25,"My_Tech Studio");
	OLED_ShowString(20,45,"By LiuLizhi");
	OLED_Refresh_Gram();
	delay_ms(3000);
	OLED_Clear();
	OLED_Refresh_Gram();

	while(1) 
	{	
		if(g_timerstatus == 1)
		{		
			/// 编码器按键读取与处理
			EncoderKeyRrefresh();
			
			/// 触摸按键读取与处理
			TouchKeyRrefresh();
			
			/// 按键抬起后操作LED开关
			if(touchkeystatus == 2)
			{
				if(ledstatus == 0)
					ledstatus = 1;
				else 
					ledstatus = 0;
			}
			
			/// 编码器数据处理
			encoderchange = (TIM_GetCounter(TIM2) - 1000);
			TIM_SetCounter(TIM2, 1000);
			
			if(encoderkeystatus == 1)
				ledcolor -= encoderchange;
			else
				ledbright -= encoderchange;
			
			/// 更新LED控制
			if(ledbright > 100)
				ledbright = 100;
			else if(ledbright < 0)
				ledbright = 0;
			
			if(ledcolor > 100)
				ledcolor = 100;
			else if(ledcolor < 0)
				ledcolor = 0;
			
			if(ledstatus == 1)
			{
				led_ctrl(ledbright, ledcolor);
				OLED_ShowString(20,20,"Bright:");
				OLED_ShowString(20,40,"Color:");
				OLED_ShowNum(84,20,(unsigned int)ledbright,3);
				OLED_ShowNum(84,40,(unsigned int)ledcolor,3);
			}
			else
			{
				led_ctrl(0, 50);
				OLED_Clear();
			}
						
			/// 更新定时器状态
			g_timerstatus = 0;
			
			/// 更新OLED显示
			OLED_Refresh_Gram();
		}
	}		 
}
 








