#include"RS232.H"
#include "stdio.h"

int main()
{
	RS232_Init(9600);
	printf("功能：串口打印可以像printf一样方便！\n");
	printf("前提是你要重写putc函数\n");
	while(1);
}
