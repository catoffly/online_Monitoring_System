#include "esp8266.h"
#include "usart1.h"
#include "time.h"

short data_exist=0,data_exist1=0,data_exist2=0,data_exist3=0;
u8 data_length=0,data_length1=0,data_length2=0,data_ok=0;
int *nextval;
char data_wifi[30];
char *ATCommandsArray[21] = {
	"AT\r\n",
	"AT+CIPSTATUS\r\n",
	"AT+CWLAP\r\n",
	"AT+GMR\r\n",
	"AT+CWMODE?\r\n",
	"AT+CWMODE=2\r\n",//apģʽ
	"AT+CWJAP=\"SSID\",\"PASSWORD\"\r\n",
	"AT+CWJAP?\r\n",
	"AT+RST\r\n",//����
	"AT+CIPMUX=1\r\n",//����������
	"AT+CIOBAUD=115200\r\n",
	"AT+CIPSERVER=1,8080\r\n",
	"AT+CIFSR\r\n",
	"AT+CIPSTART=?\r\n",
	"AT+CWLIF\r\n",
	"AT+CWQAP\r\n",
	"AT+CWSAP=\"llj123\",\"0123456789\",11,3\r\n",
	"ATE0\r\n",
	"AT+CIPCLOSE=",
	"AT+CWMODE=1\r\n",
	"ATE1\r\n"};


char *ESP_Responses[11] =
{
		"ready",
		"Link",
		"Unlink",
		"OK\r\n",
		"SEND OK",
		"+IPD",
		"ERROR",
		"wrong syntax",
		"busy p...",
		"busy inet...",
		":"
};
/*���ܣ�esp8266 apģʽserver
 *������void  
 *����ֵ��void
 *����:���־�
 */
void esp8266_ap_server_init(void)
{

		USART_printf(USART2,ATCommandsArray[20] );//��������
		delay_ms(1000);
		USART_printf(USART2,ATCommandsArray[5] );//apģʽ
		delay_ms(1000);
		USART_printf(USART2,ATCommandsArray[16] );//AT+CWSAP="ESP8266","12345678",11,0 ����·��
		delay_ms(1000);
		USART_printf(USART2,ATCommandsArray[8] );//����
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		USART_printf(USART2,ATCommandsArray[9] );//AT+CIPMUX=1 ���óɶ�����
		delay_ms(1000);
		USART_printf(USART2,ATCommandsArray[11] );//AT+CIPSERVER=1,8080 ����TCP����˿�
		delay_ms(1000);
}



