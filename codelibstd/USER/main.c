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

extern unsigned char g_timerstatus;						///< ����ʱ������״̬

unsigned short encoderkeycnt = 0;						///< ��������������
unsigned char encoderkeystatus = 0;						///< ����������״̬
short encoderchange = 0;								///< �����������ı�ֵ

unsigned short touchkeycnt = 0;							///< ������������
unsigned char touchkeystatus = 0;						///< ��������״̬

unsigned short ledstatus = 0;							///< LED����״̬
short ledbright = 50;									///< LED����ֵ
short ledcolor = 50;									///< LEDɫ��

/// ����������״̬����
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

/// ��������״̬����
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
	delay_init();	    			    				///< ��ʼ����ʱ����	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		///< �����ж����ȼ�����2
	uart_init(115200);     			    				///< ��ʼ������1�����ڵ���
	TIM3_Init();										///< ����ʱ��������100Hz
	led_init();											///< LED��ʼ��
	OLED_Init();										///< OLED��ʼ��
	touchkey_Init();									///< ����������ʼ��
	encoder_Init();										///< ��������ʼ��

	/// ��ʾLogo
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
			/// ������������ȡ�봦��
			EncoderKeyRrefresh();
			
			/// ����������ȡ�봦��
			TouchKeyRrefresh();
			
			/// ����̧������LED����
			if(touchkeystatus == 2)
			{
				if(ledstatus == 0)
					ledstatus = 1;
				else 
					ledstatus = 0;
			}
			
			/// ���������ݴ���
			encoderchange = (TIM_GetCounter(TIM2) - 1000);
			TIM_SetCounter(TIM2, 1000);
			
			if(encoderkeystatus == 1)
				ledcolor -= encoderchange;
			else
				ledbright -= encoderchange;
			
			/// ����LED����
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
						
			/// ���¶�ʱ��״̬
			g_timerstatus = 0;
			
			/// ����OLED��ʾ
			OLED_Refresh_Gram();
		}
	}		 
}
 








