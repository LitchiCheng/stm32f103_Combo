#include"RS232.H"
#include "stdio.h"

int main()
{
	RS232_Init(9600);
	printf("���ܣ����ڴ�ӡ������printfһ�����㣡\n");
	printf("ǰ������Ҫ��дputc����\n");
	while(1);
}
