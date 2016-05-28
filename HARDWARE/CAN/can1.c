#include "stm32f10x_can.h"
#include "sys.h"
#include "can1.h"
//void GPIO_Configuration(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;	


//	//端口映射
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

   //关闭时间触发模式
   CAN_InitStructure.CAN_TTCM=DISABLE;
   //关闭自动离线管理
   CAN_InitStructure.CAN_ABOM=DISABLE;
   //关闭自动唤醒模式
   CAN_InitStructure.CAN_AWUM=DISABLE;
   //禁止报文自动重传
   CAN_InitStructure.CAN_NART=DISABLE;
   //FIFO 溢出时报文覆盖源文件
   CAN_InitStructure.CAN_RFLM=DISABLE;
   //报文发送优先级取决于ID号
   CAN_InitStructure.CAN_TXFP=DISABLE;
   //正常的工作模式
   CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;

   // 设置CAN 波特率  125 KBPS 500k

   CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
   CAN_InitStructure.CAN_BS1=CAN_BS1_3tq;
   CAN_InitStructure.CAN_BS2=CAN_BS2_2tq;
   CAN_InitStructure.CAN_Prescaler = 48;


   //初始化CAN
   CAN_Init(CAN1,&CAN_InitStructure);
	
   //屏蔽滤波
   CAN_FilterInitStructure.CAN_FilterNumber=0;
   //屏蔽模式
   CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
   //32位寄存器
   CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
   //高16位
   CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
   //低16位
   CAN_FilterInitStructure.CAN_FilterIdLow=0;
   //屏蔽位高16位
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
   //屏蔽位低16位
   CAN_FilterInitStructure.CAN_FilterMaskIdLow=0;
   //过滤器 0 关联到 FIFO0
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
   //使能过滤器
   CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
   //初始化过滤器
   CAN_FilterInit(&CAN_FilterInitStructure);

   //使能接收中断
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

	//CAN接收
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
	//CAN 发送数据
	CAN_Transmit(CAN1,&TxMessage);
}



////TxMessage.StdId=0xFE00>>5;
//		//TxMessage.ExtId=0;
//		//TxMessage.IDE=CAN_ID_STD;
//   	TxMessage.StdId=0;
//		TxMessage.ExtId=0xFFFFFFFF>>3;

//		TxMessage.IDE=CAN_ID_EXT;
//		//数据帧
//		TxMessage.RTR=CAN_RTR_DATA;
//		//远程帧
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
//		//CAN 发送数据
//		//CAN_Transmit(CAN1,&TxMessage);
