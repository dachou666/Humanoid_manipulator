#include "stm32f10x.h"   
#include "usart.h"	
#include "delay.h"
#include "adc.h"
#include "led.h"
#include "key.h"  
#include "iwdg.h"	

/***拇指最大值，与调试控制舵机的值相关***/
/***根据实际调试改写***/
#define THUMB_MAX 120 
#define THUMB_MIN 10

#define INDEX_FINGER_MAX 125
#define INDEX_FINGER_MIN 20

#define MIDDLE_FINGER_MAX 125
#define MIDDLE_FINGER_MIN 0

#define RING_FINGER_MAX 117
#define RING_FINGER_MIN 20

#define LITTLE_FINGER_MAX 117
#define LITTLE_FINGER_MIN 30

float temp;	//调试用的临时值
u16 adc[5]={0};	//adc存储数组

/***校准时所用的最大最小变量***/
u16 thumb_max,thumb_min;
u16	index_finger_max,index_finger_min;
u16	middle_finger_max,middle_finger_min;
u16	ring_finger_max,ring_finger_min;
u16	little_finger_max,little_finger_min;

/***数据采集函数***/
/***用于采集adc数据***/
void DATA_COLLECTION(void)
{
	adc[0]=Get_Adc_Average(ADC_Channel_0,10);
	printf("\r\n CH0 = %d \r\n",adc[0]);
	
	adc[1]=Get_Adc_Average(ADC_Channel_1,10);
	printf("\r\n CH1 = %d \r\n",adc[1]);
	
	adc[2]=Get_Adc_Average(ADC_Channel_2,10);
	printf("\r\n CH2 = %d \r\n",adc[2]);
	
	adc[3]=Get_Adc_Average(ADC_Channel_3,10);
	printf("\r\n CH3 = %d \r\n",adc[3]);
	
	adc[4]=Get_Adc_Average(ADC_Channel_4,10);
	printf("\r\n CH4 = %d \r\n",adc[4]);
}

/***数据转换函数***/
/***把读取的adc值转换成控制舵机的值通过串口3发给esp8266***/
/***公式：（（校准最大值-采集值）/（调校准最大值-校准最小值）*（调试时的舵机最大值-调试时的舵机最小值））+调试时的舵机最小值））***/
void DATA_TRANSFORMATION(void)
{	
	adc[0]=Get_Adc_Average(ADC_Channel_0,10);
	printf("\r\n CH0 = %d \r\n",adc[0]);
	USART3_printf("4,%3d",(int)(((little_finger_max-(float)adc[0])/(little_finger_max-little_finger_min))*(LITTLE_FINGER_MAX-LITTLE_FINGER_MIN))+LITTLE_FINGER_MIN);
//	delay_ms(100);
	
	adc[1]=Get_Adc_Average(ADC_Channel_1,10);
	printf("\r\n CH1 = %d \r\n",adc[1]);
	USART3_printf("3,%3d",(int)(((ring_finger_max-(float)adc[1])/(ring_finger_max-ring_finger_min))*(RING_FINGER_MAX-RING_FINGER_MIN))+RING_FINGER_MIN);
//	delay_ms(100);
	
	adc[2]=Get_Adc_Average(ADC_Channel_2,10);
	printf("\r\n CH2 = %d \r\n",adc[2]);
	USART3_printf("2,%3d",(int)(((middle_finger_max-(float)adc[2])/(middle_finger_max-middle_finger_min))*(MIDDLE_FINGER_MAX-MIDDLE_FINGER_MIN))+MIDDLE_FINGER_MIN);
//	delay_ms(100);
	
	adc[3]=Get_Adc_Average(ADC_Channel_3,10);
	printf("\r\n CH3 = %d \r\n",adc[3]);
	USART3_printf("1,%3d",(int)(((index_finger_max-(float)adc[3])/(index_finger_max-index_finger_min))*(INDEX_FINGER_MAX-INDEX_FINGER_MIN))+INDEX_FINGER_MIN);
//	delay_ms(100);
	
	adc[4]=Get_Adc_Average(ADC_Channel_4,10);
	printf("\r\n CH4 = %d \r\n",adc[4]);
	USART3_printf("0,%3d",(int)((((thumb_max-(float)adc[4])/(thumb_max-thumb_min))*(THUMB_MAX-THUMB_MIN))+THUMB_MIN));
	printf("0,%3d",(int)((((float)(adc[4]-thumb_min)/(thumb_max-thumb_min))*(THUMB_MAX-THUMB_MIN))+THUMB_MIN));
//	delay_ms(100);
}

int main(void)
{
	/***初始化***/
	LED_Init();
	delay_init();
	Key_GPIO_Config();
	uart1_init(115200);
	uart3_init(115200);
	printf("串口1打印出来的\r\n");
	USART3_printf("串口3打印出来的\r\n");
	Adc_Init();
	IWDG_Config(IWDG_Prescaler_64 ,625*5);  // IWDG 5s 超时溢出
	 
	/***进行最大值校准***/
	LED0=!LED0;
	while(Key_Scan(GPIOB,GPIO_Pin_0))
	{
		IWDG_Feed();
		DATA_COLLECTION();
	}	
	LED0=!LED0;
	thumb_max = adc[4]+50;
	index_finger_max = adc[3];
	middle_finger_max = adc[2];
	ring_finger_max = adc[1];
	little_finger_max = adc[0];
	USART3_printf("MAX_finish");

	/***进行最小值校准***/
	while(Key_Scan(GPIOB,GPIO_Pin_0))
	{
		IWDG_Feed();	
		DATA_COLLECTION();
	}
	thumb_min = adc[4];
	index_finger_min = adc[3];
	middle_finger_min = adc[2];
	ring_finger_min = adc[1];
	little_finger_min = adc[0];
	USART3_printf("MIN_finish");
	
	while(1)
	{
		IWDG_Feed();
		DATA_TRANSFORMATION();
//		temp=(float)adc[4]*(3.3/4096);
//		printf("\r\n CH4 value = %f V \r\n",temp);
	
		LED0=!LED0;
//		delay_ms(10);
	}
}


