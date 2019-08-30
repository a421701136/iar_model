/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			USART.h
** Created by:		    Alex Hsu
** Created date:		2009-25-06
**
********************************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H
/* Includes ------------------------------------------------------------------*/
//#include "PROJ_Config.h"
/* Exported types ------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define RS485_TXD5               GPIO_Pin_12 // PC12
#define RS485_RXD5               GPIO_Pin_2  // PD2
#define RS485_CTRL               GPIO_Pin_3  // PD3
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UART_Init(void);
void UART2_SendByte(unsigned char Data);
void UART_SendData(unsigned char* pSendData, unsigned char nLength);
void RS232_COM(void);
//void USART_SendString(unsigned short *pDataA, unsigned short *pDataB,unsigned short *pDataC,unsigned int num);
#endif
