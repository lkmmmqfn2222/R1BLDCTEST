#include "delay.h"
#include "bsp.h"

void delay_init()
{

#ifdef OS_CRITICAL_METHOD 	//if OS_CRITICAL_METHOD id defined 
	u32 reload; 				//it means that ucosII will be used
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 	//  HCLK/8
	fac_us = SystemCoreClock / 8000000; 	//1/8 of the system clock  

#ifdef OS_CRITICAL_METHOD 	//���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	reload = SystemCoreClock / 8000000; 		//ÿ���ӵļ������� ��λΪK	   
	reload *= 1000000 / OS_TICKS_PER_SEC; //����OS_TICKS_PER_SEC�趨���ʱ��
	//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	fac_ms = 1000 / OS_TICKS_PER_SEC; //����ucos������ʱ�����ٵ�λ	   
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;    	//����SYSTICK�ж�
	SysTick->LOAD = reload;  	//ÿ1/OS_TICKS_PER_SEC���ж�һ��	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;    	//����SYSTICK    
#else
	fac_ms = (u16)fac_us * 1000; // if not use ucos, every ms comprise of systick   
#endif
}


void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD = nus*fac_us;  //load time to delay	  		 
	SysTick->VAL = 0x00;         //clear the counter
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;           //begin
	do
	{
		temp = SysTick->CTRL;
	} while (temp & 0x01 && !(temp&(1 << 16)))
		;//waiting
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;        //disable the counter
	SysTick->VAL = 0X00;        //clear the counter	 
}

void delay_ms(u16 nms)
{
	u32 temp;
	SysTick->LOAD = (u32)nms*fac_ms; //load time (SysTick->LOADΪ24bit)
	SysTick->VAL = 0x00;            //clear the counter
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;           //begin  
	do
	{
		temp = SysTick->CTRL;
	} while (temp & 0x01 && !(temp&(1 << 16)))
		;//waiting   
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;        //disable the counter
	SysTick->VAL = 0X00;        //clear the counter  	    
}