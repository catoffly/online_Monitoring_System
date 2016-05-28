#include "key_16.h"
#include "sys.h" 
#include "time.h"
#include "lcd.h"


_key_scan keyscan;  //管理按键的重要参数


//u8 k;
								    
//按键初始化函数
void KEY_16_Init(void) //IO初始化
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
//	extern u8 k;  //记下按键的次数
    keycode=16;
    scancode=0xfe;  // 设置PC的低四位将要输出的值1110
    for(i=0;i<4;i++) //循环的行扫描
    {
	      GPIOC->ODR&=0x00;
	      GPIOC->ODR|=scancode;
          temp=GPIOC->IDR;
		  delay_ms(10);
          if((temp&0xf0)!=0xf0)  //有按键按下，进入循环
          {
              //keycode=(COL&0xf0)|scancode;
//              temp=GPIO_ReadInputData(GPIOC);
//			  temp=GPIOC->IDR;
 //             k++;
              keyscan.keytime++;
              delay_ms(20);
              while((GPIO_ReadInputData(GPIOC)&0xf0)!=0xf0);    //等待按键释放      
              temp >>=4;
              temp=~temp;
              temp &=0x0f;
//              keyscan.keytime=keyscan.keytime-1; 
              switch(temp)
              {
                  case 0x01:
                    keycode=0+i*4;    //1、5、9、13
                    break;
                  case 0x02:        // 2、6、10、14
                    keycode=1+i*4; 
                    break;
                  case 0x04:    //3、7、11、15
                    keycode=2+i*4; 
                    break;
                  case 0x08:  //4、8、12、16
                    keycode=3+i*4; 
                    break;
              }
       //       return(keycode);
          }
          scancode <<=1;  //前一行扫描完后，接着下一行
          scancode++;

    }
//    return(keycode);  //返回按下的第几个按键
    keyscan.keynum=keycode;
//	if(k==20)k=0;	  //用于显示的位置

//	keyscan.keytime=(k-1);
}
//依次显示数字
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

// 功  能：通过键盘录入一个小于2位的数字

// ***************************************/
// u8 KEY_getnum(void)
// {
// 	u8 value=0;
// 	u8 temp=1;
// 	u8 i;
// 	
// 	static  u8 keybuf[2]; //用于存放键盘录入的键值
//     static  u8 bitnum=0;  //记录放入的数是第几位
// //	KEY_16_Scan(); //按键扫描  获得键值

// 	keybuf[bitnum]=keyscan.keynum;
// 	bitnum++;
// 	
// 			//将数组中的数据算出 
// 			if(bitnum==2)
// 			{
// 				for(i=0;i<bitnum;i++)
// 				{
// 					value=keybuf[bitnum-i-1]*temp+value;  //换算成十进制
// 					temp=temp*10;
// 				}
// 				temp=1;
// 				for(i=0;i<2;i++)   //清零数组
// 				keybuf[i]=0;
// 				bitnum=0;
// 			}

// 	return value;
// }

