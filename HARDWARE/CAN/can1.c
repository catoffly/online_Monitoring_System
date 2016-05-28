#include "stm32f10x_can.h"
#include "sys.h"
#include "can1.h"
//void GPIO_Configuration(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;	


//	//�˿�ӳ��
//	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);

//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//}

//void NVIC_Configuration(void)
//{
//   	NVIC_InitTypeDef NVIC_InitStructure; 

//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

//	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
//	NVIC_Init(&NVIC_InitStructure);
//}

void CAN_Configuration(void)
{
   CAN_InitTypeDef CAN_InitStructure;
   CAN_FilterInitTypeDef CAN_FilterInitStructure;
		NVIC_InitTypeDef NVIC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;	
	
   CAN_DeInit(CAN1);
   CAN_StructInit(&CAN_InitStructure);

   //�ر�ʱ�䴥��ģʽ
   CAN_InitStructure.CAN_TTCM=DISABLE;
   //�ر��Զ����߹���
   CAN_InitStructure.CAN_ABOM=DISABLE;
   //�ر��Զ�����ģʽ
   CAN_InitStructure.CAN_AWUM=DISABLE;
   //��ֹ�����Զ��ش�
   CAN_InitStructure.CAN_NART=DISABLE;
   //FIFO ���ʱ���ĸ���Դ�ļ�
   CAN_InitStructure.CAN_RFLM=DISABLE;
   //���ķ������ȼ�ȡ����ID��
   CAN_InitStructure.CAN_TXFP=DISABLE;
   //�����Ĺ���ģʽ
   CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;

   // ����CAN ������  125 KBPS 500k

   CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
   CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
   CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
   CAN_InitStructure.CAN_Prescaler = 48;


   //��ʼ��CAN
   CAN_Init(CAN1,&CAN_InitStructure);
	
   //�����˲�
   CAN_FilterInitStructure.CAN_FilterNumber=0;
   //����ģʽ
   CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
   //32λ�Ĵ���
   CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
   //��16λ
   CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
   //��16λ
   CAN_FilterInitStructure.CAN_FilterIdLow=0;
   //����λ��16λ
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
   //����λ��16λ
   CAN_FilterInitStructure.CAN_FilterMaskIdLow=0;
   //������ 0 ������ FIFO0
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
   //ʹ�ܹ�����
   CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
   //��ʼ��������
   CAN_FilterInit(&CAN_FilterInitStructure);

   //ʹ�ܽ����ж�
   CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
	 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
		GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
 }
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	CanTxMsg TxMessage;

	//CAN����
	CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);

	TxMessage.StdId=RxMessage.StdId;
	TxMessage.ExtId=RxMessage.ExtId;
	TxMessage.IDE=RxMessage.IDE;
	TxMessage.RTR=RxMessage.RTR;
	TxMessage.DLC=RxMessage.DLC;

	TxMessage.Data[0]=RxMessage.Data[0];
	TxMessage.Data[1]=RxMessage.Data[1];
	TxMessage.Data[2]=RxMessage.Data[2];
	TxMessage.Data[3]=RxMessage.Data[3];
	TxMessage.Data[4]=RxMessage.Data[4];
	TxMessage.Data[5]=RxMessage.Data[5];
	TxMessage.Data[6]=RxMessage.Data[6];
	TxMessage.Data[7]=RxMessage.Data[7];
	//CAN ��������
	CAN_Transmit(CAN1,&TxMessage);
}



////TxMessage.StdId=0xFE00>>5;
//		//TxMessage.ExtId=0;
//		//TxMessage.IDE=CAN_ID_STD;
//   	TxMessage.StdId=0;
//		TxMessage.ExtId=0xFFFFFFFF>>3;

//		TxMessage.IDE=CAN_ID_EXT;
//		//����֡
//		TxMessage.RTR=CAN_RTR_DATA;
//		//Զ��֡
//		//TxMessage.RTR=CAN_RTR_REMOTE;
//		TxMessage.DLC=8;
//		
//		TxMessage.Data[0]=0x11;
//		TxMessage.Data[1]=0x22;
//		TxMessage.Data[2]=0x33;
//		TxMessage.Data[3]=0x44;
//		TxMessage.Data[4]=0x55;
//		TxMessage.Data[5]=0x66;
//		TxMessage.Data[6]=0x77;
//		TxMessage.Data[7]=0x88;
//		//CAN ��������
//		//CAN_Transmit(CAN1,&TxMessage);
