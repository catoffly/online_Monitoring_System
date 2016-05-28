#ifndef _I2C_H_
  #define _I2C_H_

  #include "stm32f10x.h"
  #include "system_stm32f10x.h"

#define CLI()      __set_PRIMASK(1)  
#define SEI()      __set_PRIMASK(0)

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

#define true 1
#define false 0

#define TRUE  0
#define FALSE -1

  #define IIC_RCC       RCC_APB2Periph_GPIOB
  #define IIC_GPIO      GPIOB
  #define SCL_PIN       GPIO_Pin_6
  #define SDA_PIN       GPIO_Pin_7

#define SCL_H         GPIOB->BSRR = GPIO_Pin_6 /* GPIO_SetBits(GPIOB , GPIO_Pin_10)   */
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6 /* GPIO_ResetBits(GPIOB , GPIO_Pin_10) */

#define SDA_H         GPIOB->BSRR = GPIO_Pin_7 /* GPIO_SetBits(GPIOB , GPIO_Pin_11)   */
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7 /* GPIO_ResetBits(GPIOB , GPIO_Pin_11) */

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10) */
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) */


//0表示写
#define	I2C_Direction_Trans   0
//１表示读
#define	I2C_Direction_Rec      1	 
/*====================================================================================================*/
/*====================================================================================================*/
uint8_t i2cWriteBuffer(uint8_t addr_, uint8_t reg_, uint8_t len_, uint8_t *data);
uint8_t i2cWrite(uint8_t addr_, uint8_t reg_, uint8_t data);
uint8_t i2cRead(uint8_t addr_, uint8_t reg_, uint8_t len, uint8_t* buf);
void I2c_Init(void);
uint16_t i2cGetErrorCounter(void);
static void i2cUnstick(void);

int8_t i2cwrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data);
int8_t i2cread(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
/*====================================================================================================*/
/*====================================================================================================*/

#endif
