#include "key_16.h"
#include "sys.h" 
#include "time.h"
#include "lcd.h"


_key_scan keyscan;  //����������Ҫ����


//u8 k;
								    
//������ʼ������
void KEY_16_Init(void) //IO��ʼ��
{ 
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
//	GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3); 
//	GPIO_ResetBits(GPIOC, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7); 
}

void KEY_16_Scan(void)
{
	
    u8 keycode,scancode,i;
	unsigned int temp;
//	extern u8 k;  //���°����Ĵ���
    keycode=16;
    scancode=0xfe;  // ����PC�ĵ���λ��Ҫ�����ֵ1110
    for(i=0;i<4;i++) //ѭ������ɨ��
    {
	      GPIOC->ODR&=0x00;
	      GPIOC->ODR|=scancode;
          temp=GPIOC->IDR;
		  delay_ms(10);
          if((temp&0xf0)!=0xf0)  //�а������£�����ѭ��
          {
              //keycode=(COL&0xf0)|scancode;
//              temp=GPIO_ReadInputData(GPIOC);
//			  temp=GPIOC->IDR;
 //             k++;
              keyscan.keytime++;
              delay_ms(20);
              while((GPIO_ReadInputData(GPIOC)&0xf0)!=0xf0);    //�ȴ������ͷ�      
              temp >>=4;
              temp=~temp;
              temp &=0x0f;
//              keyscan.keytime=keyscan.keytime-1; 
              switch(temp)
              {
                  case 0x01:
                    keycode=0+i*4;    //1��5��9��13
                    break;
                  case 0x02:        // 2��6��10��14
                    keycode=1+i*4; 
                    break;
                  case 0x04:    //3��7��11��15
                    keycode=2+i*4; 
                    break;
                  case 0x08:  //4��8��12��16
                    keycode=3+i*4; 
                    break;
              }
       //       return(keycode);
          }
          scancode <<=1;  //ǰһ��ɨ����󣬽�����һ��
          scancode++;

    }
//    return(keycode);  //���ذ��µĵڼ�������
    keyscan.keynum=keycode;
//	if(k==20)k=0;	  //������ʾ��λ��

//	keyscan.keytime=(k-1);
}
//������ʾ����
/*
void KEY_16_display(void)
{
	KEY_16_Scan();
	if(keyscan.keynum!=16)
	{	
		LCD_ShowNum((10+keyscan.keytime*8),60,keyscan.keynum,1,16);
	}
}

*/




// /***************************************

// ��  �ܣ�ͨ������¼��һ��С��2λ������

// ***************************************/
// u8 KEY_getnum(void)
// {
// 	u8 value=0;
// 	u8 temp=1;
// 	u8 i;
// 	
// 	static  u8 keybuf[2]; //���ڴ�ż���¼��ļ�ֵ
//     static  u8 bitnum=0;  //��¼��������ǵڼ�λ
// //	KEY_16_Scan(); //����ɨ��  ��ü�ֵ

// 	keybuf[bitnum]=keyscan.keynum;
// 	bitnum++;
// 	
// 			//�������е�������� 
// 			if(bitnum==2)
// 			{
// 				for(i=0;i<bitnum;i++)
// 				{
// 					value=keybuf[bitnum-i-1]*temp+value;  //�����ʮ����
// 					temp=temp*10;
// 				}
// 				temp=1;
// 				for(i=0;i<2;i++)   //��������
// 				keybuf[i]=0;
// 				bitnum=0;
// 			}

// 	return value;
// }

