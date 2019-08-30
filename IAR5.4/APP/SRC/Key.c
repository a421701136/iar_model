/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			Key.c
** Created by:		    Alex Hsu
** Created date:		2009-25-06
**
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "Key.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "platform_config.h"
#if defined (USBHID)
	#include "usb_hid.h"
#elif defined (USBCDC)
	#include "usb_cdc.h"
#elif defined (USBCDC)
	#include "usb_cdchid.h"
#elif defined (RS485)
	#include "USART.h"
#endif
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned short XBuffer[256],YBuffer[256];
unsigned char XY_in,XY_out;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*
*******************************************************************************
* Function Name  : KEY_ADS7843Write
* Description    :  
* Input          : Control: AD7843 control command
* Output         : None
* Return         : None
*******************************************************************************/
void KEY_ADS7843Write(unsigned char Control) 
{
	unsigned char i=0;
	for(i = 0; i < 8; i++)
	{
		GPIOB->BRR = GPIO_AD7843_DCLK;
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		if(Control & 0x80)
		{
			GPIOB->BSRR = GPIO_AD7843_DIN;
		}
		else
		{
			GPIOB->BRR = GPIO_AD7843_DIN;
		}
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		GPIOB->BSRR = GPIO_AD7843_DCLK;
		Control = Control << 1;
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
	}
	//GPIO_AD7843->BRR = GPIO_AD7843_DCLK;
}
/*
*******************************************************************************
* Function Name  : KEY_ADS7843Read
* Description    :  
* Input          : None
* Output         : None
* Return         : AD7843 data
*******************************************************************************/
unsigned short KEY_ADS7843Read(void) 
{
	unsigned char i=0;
	unsigned short temp = 0;
	for(i = 0; i < 12; i++)
	{
		GPIOB->BRR = GPIO_AD7843_DCLK;
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		if(GPIOB->IDR & GPIO_AD7843_DOUT)
		{
			temp = (temp << 1) | 0x01;
		}
		else
		{
			temp = (temp << 1);
		}
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		GPIOB->BSRR = GPIO_AD7843_DCLK;
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
	}
	for(i = 0; i < 4; i++)
	{
		GPIOB->BRR = GPIO_AD7843_DCLK;
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		GPIOB->BSRR = GPIO_AD7843_DCLK;
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
		asm("NOP");
	}
	return temp;
}
/*
*******************************************************************************
* Function Name  : KEY_ADS7843Read
* Description    :  
* Input          : None
* Output         : None
* Return         : AD7843 data
*******************************************************************************/
void KEY_SendData(unsigned char *pSendData, unsigned char nLength)
{
#if defined (USBHID)
	USBHID_SendData(pSendData, nLength);
#elif defined (USBCDC)
	USBCDC_SendData(pSendData, nLength);
#elif defined (USBCDC)
	USBCDCHID_SendData(pSendData, nLength);
#elif defined (RS485)
	UART_SendData(pSendData, nLength);
#endif
}