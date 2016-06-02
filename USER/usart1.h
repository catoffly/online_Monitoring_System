#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
void USART1_Config(void);
int fputc(int ch, FILE *f);
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void USART2_Config(void);
void USART3_Config(void);
extern char data_us,USART_RX_buf[50],USART_RX,data_us1,USART_RX_buf1[50],USART_RX1;
extern u8 cun,cun1;
int kmp_search( char const *src, char const *patn, int const *nextval,int pos);
void get_nextval(char const *ptn, int *nextval);
extern char data_cu[100],state_0x0a,data_cu1[100],state_0x0a1;
int bf( char *text,  char *find);
#endif /* __USART1_H */
