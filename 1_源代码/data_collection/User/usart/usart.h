#ifndef __USART_H
#define __USART_H
#include "stdio.h"	


//如果想串口中断接收，请不要注释以下宏定义
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口2接收

void USART3_printf(char *fmt, ...);

//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);
void uart3_init(u32 bound);
int USART1_Determine_data(char AT_data[]);
int USART3_Determine_data(char AT_data[]);
#endif


