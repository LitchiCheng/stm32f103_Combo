#include "test.h"
#include "bsp_esp8266.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>



volatile uint8_t ucTcpClosedFlag = 0;

char cStr [ 1500 ] = { 0 };



/**
  * @brief  ESP8266 （Sta Tcp Client）透传
  * @param  无
  * @retval 无
  */
void ESP8266_StaTcpClient_UnvarnishTest ( void )
{
	uint8_t ucStatus;
	
	char cStr [ 100 ] = { 0 };
	
		
  printf ( "\r\n正在配置 ESP8266 ......1\r\n" );

	macESP8266_CH_ENABLE();
	printf ( "\r\n正在配置 ESP8266 ......2\r\n" );
	
	ESP8266_AT_Test ();
	printf ( "\r\n正在配置 ESP8266 ......3\r\n" );
	
	ESP8266_Net_Mode_Choose ( STA );
	printf ( "\r\n正在配置 ESP8266 ......4\r\n" );

  while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	
	printf ( "\r\n正在配置 ESP8266 ......5\r\n" );
	ESP8266_Enable_MultipleId ( DISABLE );
	
	while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port, Single_ID_0 ) );
	
	while ( ! ESP8266_UnvarnishSend () );
	
	printf ( "\r\n配置 ESP8266 完毕\r\n" );
	
		
}


