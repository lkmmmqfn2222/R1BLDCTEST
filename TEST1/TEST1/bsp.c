#include "bsp.h"

uint8_t receive_dma[CIRCUL_BUF_LEN];
uint16_t adc_dma[ADC_CHANNEL_SIZE];

void uart1_init(u32 baudrate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);

	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

	// 	����DMA,TDR�Ĵ�����д.
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->TDR);  //����Ĵ�����ַ
	//DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SRC_Const_Buffer;//�����ͻ�����
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //������ΪĿ���ַ
	DMA_InitStructure.DMA_BufferSize = 0; //�����С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //Դ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //ÿ�η��Ͷ���Byte
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�ֽ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //���δ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //�е����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //�����ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel2, &DMA_InitStructure); //��ʼ��

	// ʹ��DMA USART����
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_Cmd(DMA1_Channel2, DISABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// 	����DMA,RDR�Ĵ�����д.
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->RDR);  //����Ĵ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&receive_dma); //�����ͻ�����
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //�ڴ���ΪĿ���ַ
	DMA_InitStructure.DMA_BufferSize = CIRCUL_BUF_LEN; //�����С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //Դ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //ÿ�η��Ͷ���Byte
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�ֽ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //ѭ������
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; //�е����ȼ�
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //�����ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel3, &DMA_InitStructure); //��ʼ��

	// ʹ��DMA USART����
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	DMA_Cmd(DMA1_Channel3, ENABLE);
}

void pwm_tim_init()
{
	
}