#ifndef __USART_H
#define __USART_H
#include "stdio.h"	


//����봮���жϽ��գ��벻Ҫע�����º궨��
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������2����

void USART3_printf(char *fmt, ...);

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart1_init(u32 bound);
void uart3_init(u32 bound);
int USART1_Determine_data(char AT_data[]);
int USART3_Determine_data(char AT_data[]);
#endif


