/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			USART.c
** Created by:		    Alex Hsu
** Created date:		2009-25-06
**
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "platform_config.h"
#include "stm32f10x.h"
//#include "stdio.h"
#include "USART.h"
#include "hw_config.h"
#include "Delay.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//unsigned int BaudRate = 38400;
//unsigned short StopBit = USART_StopBits_1;
//unsigned short ParityBit = USART_Parity_No;
//unsigned short DataBit = USART_WordLength_8b;
//extern unsigned char Buf_Size_Out;
unsigned char Com_Buf[256] = {0xFF,0x00,0x00,0x00,0x00,0x00};//State was sent to PC	
unsigned char com_buffer_cnt = 0;  //´æ´¢Ê±¼ÆÊý

extern  unsigned char Com_Buf_In[256];
extern volatile u8 RS232ReceiveIn2;
u8 RS232ReceiveOut2;
u8 cmd_end_flag = 0;
extern void COM_Process(u8 CMD_Buf[]);

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void UART_Init(void)
{
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
	/* Configure UART5 Tx(PC.12) as alternate function open-drain */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = TXD_MCU;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Configure UART5_Rx(PD.02) as input floating */
  GPIO_InitStructure.GPIO_Pin = RXD_MCU;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 38400;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  /* Configure the USARTx */ 
  USART_Init(USART2, &USART_InitStructure);
  /* Enable the USARTx */
  USART_Cmd(USART2, ENABLE);
  /* Enable the USARTx Receive interrupt*/
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
}

/*******************************************************************************
* Function Name  : UART1_SendByte
* Description    : 
* Input          : Data:the data to transmit. 
* Output         : None
* Return         : None
*******************************************************************************/
void UART2_SendByte(unsigned char Data)
{
	/* Send one byte */
	USART_SendData(USART2, Data);
    
  /* Loop until USART1 DR register is empty */ 
  while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {
  }
}



void  UART_SendData(unsigned char* pSendData, unsigned char nLength)
{
	unsigned char i;
	for(i = 0; i < nLength; i++)
	{
		/* Loop until USART1 DR register is empty */ 
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {
    }
    USART_SendData(USART2, *pSendData);
    
    pSendData++;
    
    /* Loop until USART1 DR register is empty */ 
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
}
/*********************************************************************************/
/*                   com_rbuflen                                                 */
/*********************************************************************************/
/*In  :-                                                                         */
/*Out :-                                                                         */
/*********************************************************************************/
int com2_rbuflen(void)
{
	if(RS232ReceiveIn2 == RS232ReceiveOut2)
  {
    return 0;
  }
  else
    return 1;
}

/*********************************************************************************/
/*                   com_getchar                                                 */
/*********************************************************************************/
/*In  :-                                                                         */
/*Out :-                                                                         */
/*********************************************************************************/
int com2_getchar (void)
{
	int c;
	if(com2_rbuflen()==0)
		return (-1);
	c=Com_Buf_In[RS232ReceiveOut2];
  Com_Buf_In[RS232ReceiveOut2] = 0;
  RS232ReceiveOut2++;
	return (c);
}

//*----------------------------------------------------------------------------
//* Function Name     : com_waitchar
//* Object            : 
//* Input Parameters  : 
//* Output Parameters : 
//*----------------------------------------------------------------------------
int com2_waitchar(void)
{
	int c,i = 0;
	for(;;)
	{
		c = com2_getchar();
		
		if(c != (-1))
		{
			return c;
		}
		i++;
		if(i > 5000000)
		{
			return (-1);
		}
	}
}


void RS232_COM(void)
{
  int m = com2_getchar();
  if(m != (-1))
  {
    Com_Buf[com_buffer_cnt] = (u8)m;
    if(Com_Buf[com_buffer_cnt] == 0xff) 
    {
      cmd_end_flag = 1;
    }
    com_buffer_cnt++;
  }
  if(com_buffer_cnt > 5)
  {
    if((cmd_end_flag == 1)&&(Com_Buf[com_buffer_cnt - 1] == 0xff))
    {
      cmd_end_flag = 0;
      COM_Process(&(Com_Buf[com_buffer_cnt - 6]));
      for(m = 0; m < com_buffer_cnt; m++)
      {
        Com_Buf[m] = 0;
      }
      com_buffer_cnt = 0;
    }
  }
}




