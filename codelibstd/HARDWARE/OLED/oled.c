#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"

///   SCK  	PA5
///   SDA  	PA7
///   RES  	PB0
///   DC   	PB1
///   CS   	PA4

//OLED的显存
unsigned char OLED_GRAM[8][128];			   

//初始化SSD1306					    
void OLED_Init(void)
{ 	
 	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	/// 时钟初始化
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/// SPI GPIO初始化
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_5|GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	 
	
	/// GPIO初始化
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_0|GPIO_Pin_1;	 
 	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	
 	GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);	
	
	GPIO_InitStructure.GPIO_Pin 		= GPIO_Pin_4;	 
 	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 		= GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	
 	GPIO_SetBits(GPIOA,GPIO_Pin_4);	
	
	/// SPI初始化
	SPI_InitStructure.SPI_Direction 		= SPI_Direction_1Line_Tx; 
	SPI_InitStructure.SPI_Mode 				= SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize 			= SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL 				= SPI_CPOL_Low; 		
	SPI_InitStructure.SPI_CPHA 				= SPI_CPHA_1Edge; 		
	SPI_InitStructure.SPI_NSS 				= SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit 			= SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial 	= 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);

	/// OLED初始化
	OLED_RST_Set();
	delay_ms(100);
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set(); 
	OLED_CS_Clr();
			
	/// OLED寄存器设置
	OLED_WR_Byte(0xAE,OLED_CMD); 	///< 关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD);	///< 设置时钟分频因子,震荡频率
	OLED_WR_Byte(0x50,OLED_CMD);	///< [3:0],分频因子;[7:4],震荡频率 
	OLED_WR_Byte(0xA8,OLED_CMD);	///< 设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD);	///< 默认0X3F(1/64)
	OLED_WR_Byte(0xD3,OLED_CMD);	///< 设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD);	///< 默认为0
	OLED_WR_Byte(0x40,OLED_CMD);	///< 设置显示开始行 [5:0],行数.									    
	OLED_WR_Byte(0x8D,OLED_CMD);	///< 电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD);	///< bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD);	///< 设置内存地址模式
	OLED_WR_Byte(0x00,OLED_CMD);	///< 默认为0
	OLED_WR_Byte(0xA0,OLED_CMD);	///< 段重定义设置,bit0:0,0->0;1,0->127;   0xA0<-->0xA1
	OLED_WR_Byte(0xC8,OLED_CMD);	///< 设置COM扫描方向; 0xC0<-->0xC8:垂直反向
	OLED_WR_Byte(0xDA,OLED_CMD);	///< 设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD);	///< [5:4]配置
	OLED_WR_Byte(0x81,OLED_CMD);	///< 对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD);	///< 1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD);	///< 设置预充电周期
	OLED_WR_Byte(0xF1,OLED_CMD);	///< [3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD);	///< 设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD);	///< [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_WR_Byte(0xA4,OLED_CMD);	///< 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD);	///< 设置显示方式;bit0:1,反相显示;0,正常显示						   
	OLED_WR_Byte(0xAF,OLED_CMD);	///< 开启显示
	OLED_Clear();
	OLED_DC_Set();
	
	/// DMA初始化
	//DMA_Config();			
}  

/// DMA初始化
void DMA_Config(void)
{
	DMA_InitTypeDef	DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel3);
            
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32) &SPI1->DR;  
	DMA_InitStructure.DMA_MemoryBaseAddr 		= (u32) &OLED_GRAM[0][0]; 
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize 			= 1024;
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_Medium;
    DMA_InitStructure.DMA_M2M		         	= DMA_M2M_Disable;  
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);

	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	DMA_SetCurrDataCounter(DMA1_Channel3, 1024);
    DMA_Cmd(DMA1_Channel3, ENABLE);
}

//向SSD1106写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{			  	
	if(cmd)
		OLED_DC_Set();
	else 
		OLED_DC_Clr();	
	
	SPI_I2S_SendData(SPI1, dat);	 
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	OLED_DC_Set();   
} 

//硬件SPI刷屏
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[i][n],OLED_DATA); 
	}   
}

//清屏
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[i][n]=0X00;  
}

//画点函数
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	pos=7-(y>>3); 
	bx=y%8;
	temp=1<<(7-bx);
	if(t)
		OLED_GRAM[pos][x]|=temp;
	else 
		OLED_GRAM[pos][x]&=~temp;	
}

//填充方形区域
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)
			OLED_DrawPoint(x,y,dot);
	}										
}

//显示字符
//x:0~127
//y:0~63
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{   
	y=6-y/8;
	chr=chr-' ';
	OLED_GRAM[y][x]		= asc2_1608[chr][1];
	OLED_GRAM[y][x+1]	= asc2_1608[chr][3];
	OLED_GRAM[y][x+2]	= asc2_1608[chr][5];
	OLED_GRAM[y][x+3]	= asc2_1608[chr][7];
	OLED_GRAM[y][x+4]	= asc2_1608[chr][9];
	OLED_GRAM[y][x+5]	= asc2_1608[chr][11];
	OLED_GRAM[y][x+6]	= asc2_1608[chr][13];
	OLED_GRAM[y][x+7]	= asc2_1608[chr][15];
	
	OLED_GRAM[y+1][x]	= asc2_1608[chr][0];
	OLED_GRAM[y+1][x+1]= asc2_1608[chr][2];
	OLED_GRAM[y+1][x+2]= asc2_1608[chr][4];
	OLED_GRAM[y+1][x+3]= asc2_1608[chr][6];
	OLED_GRAM[y+1][x+4]= asc2_1608[chr][8];
	OLED_GRAM[y+1][x+5]= asc2_1608[chr][10];
	OLED_GRAM[y+1][x+6]= asc2_1608[chr][12];
	OLED_GRAM[y+1][x+7]= asc2_1608[chr][14];      
}

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;			
	u8 size=16;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1;  
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
		OLED_ShowChar(x,y,chr[j]);
		x+=8;
		if(x>120)
		{
			x=0;
			y+=2;
		}
		j++;
	}
}





























