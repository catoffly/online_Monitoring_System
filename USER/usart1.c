/******************** 尚学科技 **************************
 * 实验平台：开拓者STM32开发板
 * 库版本  ：ST3.5.0
 * 作者    ：尚学科技团队 
 * 淘宝    ：http://shop102218275.taobao.com/
 * 本程序只供学习使用，未经作者许可，不得用于其它任何用途
 *版权所有，盗版必究。
**********************************************************************************/
#include "usart1.h"
#include <stdarg.h>
#include "string.h"
#include "esp8266.h"
#include "stdio.h"

#define MAX(x, y) (x) > (y) ? (x) : (y)
char data_us=0,USART_RX_buf[50],USART_RX=0,data_transfer=0,data_us1=0,USART_RX_buf1[50],USART_RX1;
char data_cu[100],state_0x0a,data_cu1[100],state_0x0a1;
u8 cun=0,cun1=0;
/*
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置,工作模式配置。115200 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_DeInit(USART1);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);    
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ClearFlag(USART1, USART_IT_RXNE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);  
}

/*******************************************************************************
* Function Name  : None.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
					data_transfer=USART_ReceiveData(USART1);	//读取接收到的数据
					USART_SendData(USART2, data_transfer);
	 
     } 
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
} 

/*******************************************************************************
* Function Name  : None.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int fputc(int ch, FILE *f)
{
		USART_SendData(USART1, (u8)ch);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	  
	return ch;
}

/*******************************************************************************
* Function Name  : None.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int GetKey (void)  
{ 
	while (!(USART1->SR & USART_FLAG_RXNE));
	return ((int)(USART1->DR & 0x1FF));
}

/*
 * 函数名：USART1_Config
 * 描述  ：USART1 GPIO 配置,工作模式配置。115200 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_DeInit(USART2);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);    
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ClearFlag(USART2, USART_IT_RXNE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE);  
}
void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	USART_DeInit(USART3);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);    
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ClearFlag(USART3, USART_IT_RXNE);
	USART_ITConfig(USART3, USART_IT_RXNE , ENABLE);
	USART_Cmd(USART3, ENABLE);  
}

/*
* 函数名：USART_printf
* 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
* 输入  ：-USARTx 串口通道
*         -Data   要发送到串口的内容的指针
*         -...    其他参数
* 输出  ：无
* 返回  ：无 
* 调用  ：外部调用
* 典型应用：USART_printf( USART3, "\r\n this is a demo \r\n" );
* USART_printf( USART1, "\r\n %d \r\n", i );
* USART_printf( USART2, "\r\n %s \r\n", j );
*/
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
  const char *s;
  int d;   
  char buf[16];
  va_list ap;
  va_start(ap, Data);
  while ( *Data != 0)     // 判断是否到达字符串结束符
	{                                                          
	if ( *Data == 0x5c )  //'\'
	{                                                                          
				switch ( *++Data )
				{
								case 'r':                                                                  //回车符
												USART_SendData(USARTx, 0x0d);
												Data ++;
												break;

								case 'n':                                                                  //换行符
												USART_SendData(USARTx, 0x0a);        
												Data ++;
												break;
								
								default:
												Data ++;
										break;
				}                         
	}
	else if ( *Data == '%')//start else if
	{                        
					switch ( *++Data )
					{                                
									case 's'://字符串
										s = va_arg(ap, const char *);
										for ( ; *s; s++) 
										{
														USART_SendData(USARTx,*s);
														while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
										}
										Data++;
									break;

									case 'd': //十进制
										d = va_arg(ap, int);
										//itoa(d, buf, 10);
										sprintf(buf,"%d",d);
										for (s = buf; *s; s++) 
										{
											USART_SendData(USARTx,*s);
											while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
										}
										Data++;
									break;
												 default:
																				Data++;
														break;
								}                 
				} /* end of else if */
				else USART_SendData(USARTx, *Data++);
				while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
 }
}
void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			
					data_us =USART_ReceiveData(USART2);	//读取接收到的数据
					//USART_SendData(USART1, data_us);
					USART_RX_buf[USART_RX]=data_us;//一级数组
					data_cu[cun]=USART_RX_buf[USART_RX];//二级数组
					cun++;
					USART_RX++;
					if(USART_RX>45)//防止一次数据太多,将最后一位刷成其他数值，最后一位必须是0x00才不会出错
					{
						USART_RX=0;
					}
					if(cun>95)
					{
						cun=0;
					}
					if(data_us==0x0a)
					{
						USART_RX=0;
						state_0x0a=1;
					}
					
	 
     } 
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}


void USART3_IRQHandler(void)                	//串口1中断服务程序
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			
					data_us1 =USART_ReceiveData(USART3);	//读取接收到的数据
					//USART_SendData(USART1, data_us);
					USART_RX_buf1[USART_RX1]=data_us1;//一级数组
					data_cu1[cun1]=USART_RX_buf1[USART_RX1];//二级数组
					cun1++;
					USART_RX1++;
					if(USART_RX1>45)//防止一次数据太多,将最后一位刷成其他数值，最后一位必须是0x00才不会出错
					{
						USART_RX1=0;
					}
					if(cun1>95)
					{
						cun1=0;
					}
					if(data_us1==0x0a)
					{
						USART_RX1=0;
						state_0x0a1=1;
					}
					
	 
     } 
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
}
int bf( char *text,  char *find)  
{  
		int find_len,text_len;
		char *s,*p,*q;
    if (text == '\0' || find == '\0')  
        return -1;  
    find_len = strlen(find);  
    text_len = strlen(text);  
    if (text_len < find_len)  
        return -1;  
     s = text;  
     p = s;  
     q = find;  
    while (*p != '\0')  
    {  
        if (*p == *q)  
        {  
            p++;  
            q++;  
        }  
        else  
        {  
            s++;  
            p = s;  
            q = find;  
        }  
        if (*q == '\0')  
        {  
            return (p - text) - (q - find);  
        }  
    }  
    return -1;  
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
