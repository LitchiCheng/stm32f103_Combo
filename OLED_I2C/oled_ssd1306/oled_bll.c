#include "SysTick.h"
#include "oled_i2c_driver.h"
#include "oled_bll.h"
//#include "oled_font.h"
#include "codetab.h"

void WriteCmd(unsigned char I2C_Command)//写命令
{
	I2C_WriteByte(0x00, I2C_Command);
}

void WriteDat(unsigned char I2C_Data)//写数据
{
	I2C_WriteByte(0x40, I2C_Data);
}


void OLED_Init(void)
{
	delay_ms(500); //这里的延时很重要
	
//	WriteCmd(0xAE); //开显示
//	WriteCmd(0x20);	//设置内存寻址模式	
//	WriteCmd(0x10);	//00，水平寻址模式;01，垂直寻址模式;10，页面寻址模式(重置);11，无效
//	WriteCmd(0xb0);	//为页面寻址模式设置页面开始地址，0-7
//	WriteCmd(0xc8);	//设置COM输出扫描方向
//	WriteCmd(0x00); //---设置低列地址
//	WriteCmd(0x10); //---设置高列地址
//	WriteCmd(0x40); //--设置起始行地址
//	WriteCmd(0x81); //--set contrast control register
//	WriteCmd(0xff); //亮度调节 0x00~0xff
//	WriteCmd(0xa1); //--设置段重新映射0到127
//	WriteCmd(0xa6); //--设置正常显示
//	WriteCmd(0xa8); //--设置复用比(1 ~ 64)
//	WriteCmd(0x3F); //
//	WriteCmd(0xa4); //0xa4,输出遵循RAM内容;0xa5,Output忽略RAM内容
//	WriteCmd(0xd3); //-设置显示抵消
//	WriteCmd(0x00); //-not offset
//	WriteCmd(0xd5); //--设置显示时钟分频/振荡器频率
//	WriteCmd(0xf0); //--设置分率
//	WriteCmd(0xd9); //--设置pre-charge时期
//	WriteCmd(0x22); //
//	WriteCmd(0xda); //--设置com大头针硬件配置
//	WriteCmd(0x12);
//	WriteCmd(0xdb); //--设置vcomh
//	WriteCmd(0x20); //0x20,0.77xVcc
//	WriteCmd(0x8d); //--设置DC-DC
//	WriteCmd(0x14); //
//	WriteCmd(0xaf); //--打开oled面板
	
	WriteCmd(0xae);//--turn off oled panel
	WriteCmd(0x00);//---set low column address
	WriteCmd(0x10);//---set high column address
	WriteCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	WriteCmd(0x81);//--set contrast control register
	WriteCmd(0xCF); // Set SEG Output Current Brightness
	WriteCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	WriteCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	WriteCmd(0xa6);//--set normal display
	WriteCmd(0xa8);//--set multiplex ratio(1 to 64)
	WriteCmd(0x3f);//--1/64 duty
	WriteCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	WriteCmd(0x00);//-not offset
	WriteCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	WriteCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	WriteCmd(0xd9);//--set pre-charge period
	WriteCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	WriteCmd(0xda);//--set com pins hardware configuration
	WriteCmd(0x12);
	WriteCmd(0xdb);//--set vcomh
	WriteCmd(0x40);//Set VCOM Deselect Level
	WriteCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	WriteCmd(0x02);//
	WriteCmd(0x8d);//--set Charge Pump enable/disable
	WriteCmd(0x14);//--set(0x10) disable
	WriteCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	WriteCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 
	WriteCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00); //初始清屏
	OLED_SetPos(0,0);

}

void OLED_SetPos(unsigned char x, unsigned char y) //设置起始点坐标
{ 
	WriteCmd(0xb0+y);
	WriteCmd(((x&0xf0)>>4)|0x10);
	WriteCmd((x&0x0f)|0x01);
}

void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		WriteCmd(0xb0+m);		//page0-page1
		WriteCmd(0x00);		//low column start address
		WriteCmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				WriteDat(fill_Data);
			}
	}
}

void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}

//--------------------------------------------------------------
void OLED_ON(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X14);  //开启电荷泵
	WriteCmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //设置电荷泵
	WriteCmd(0X10);  //关闭电荷泵 
	WriteCmd(0XAE);  //OLED休眠
}

/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
void OLED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>126){x=0;y++;}
		OLED_SetPos(x,y);
		for(i=0;i<6;i++)
		WriteDat(F6x8[c][i]);
		x+=6;
		j++;
	}
}

/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
void OLED_P8x16Str(unsigned char x,unsigned  char y,unsigned char ch[])
{
	unsigned char c=0,i=0,j=0;
	while (ch[j]!='\0')
	{
		c =ch[j]-32;
		if(x>120){x=0;y++;}
		OLED_SetPos(x,y);
		for(i=0;i<8;i++)
		WriteDat(F8X16[c*16+i]);
		OLED_SetPos(x,y+1);
		for(i=0;i<8;i++)
		WriteDat(F8X16[c*16+i+8]);
		x+=8;
		j++;
	}
}
/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
void OLED_P16x16Ch(unsigned char x,unsigned char y,unsigned int N)
{
	unsigned char wm=0;
	unsigned int adder=32*N;
	OLED_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	}
	OLED_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat(F16x16[adder]);
		adder += 1;
	} 	  	
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

	if(y1%8==0) y=y1/8;      
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_SetPos(x0,y);
		for(x=x0;x<x1;x++)
	    {      
	    	WriteDat(BMP[j++]);
	    }
	}
}

