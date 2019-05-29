#ifndef  __ST_FLASH_H
#define  __ST_FLASH_H
 
#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "Singleton.h"

const u8 BOARD_POWER_ON_RECORD_BUFFER[]={0x01,0xAA};
const u8 BOARD_POWER_OFF_RECORD_BUFFER[]={0x02,0xAA};
const u8 PC_POWER_ON_RECORD_BUFFER[]={0x01,0xBB};
const u8 PC_POWER_OFF_RECORD_BUFFER[]={0x00,0xBB};
#define SIZE sizeof(BOARD_POWER_ON_RECORD_BUFFER)		//数组长度

#define BOARD_POWER_STATE_FIRST_ADDRESS 0X08010400			//page 65
#define BOARD_POWER_STATE_END_ADDRESS 0X080157FF			//page 85

#define PC_POWER_STATE_FIRST_ADDRESS 0X08015800				//page 86
#define PC_POWER_STATE_END_ADDRESS 0x0801ABFF				//page 106




//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE  127	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1              //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif		 
//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值
 
class STFLASH
{
	private:
	bool mUseHalfWord;//
	uint32_t mStartAddress;//
	public:
		STFLASH(uint32_t startAddress=(0x08000000+1000),bool useHalfWord=true);
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
u16 ReadHalfWord(u32 faddr);
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite) ;
//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
void Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);
 
 
//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
void Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead) ;
 
 
};

typedef NormalSingleton<STFLASH> flasher;
 
 
#endif












