/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			    USB_CDC.c
** Created by:		    Alex Hsu
** Created date:		  2009-09-01
**
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*!< Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source) */
//#define SYSCLK_FREQ_HSE    HSE_Value
//#define SYSCLK_FREQ_20MHz  20000000
//#define SYSCLK_FREQ_36MHz  36000000
//#define SYSCLK_FREQ_48MHz  48000000
//#define SYSCLK_FREQ_56MHz  56000000
#define SYSCLK_FREQ_72MHz  72000000
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Delay_NS(unsigned int nNS)
{
	unsigned int j;
  for(j = 0; j < nNS; j++)
  {
    asm("nop");
  }
}
#if defined SYSCLK_FREQ_72MHz
/*******************************************************************************
* Function Name  : Delay_MS
* Description    : 1mS Delay at 72M 
* Input          : the number of delay in mS
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_MS(unsigned int nMS)
{
  unsigned int i,j;
  for(i = 0; i < nMS; i++)
  {
    for(j = 0; j < 9000; j++)
    {
      asm("nop");
    }
  }
}
#elif defined SYSCLK_FREQ_56MHz
/*******************************************************************************
* Function Name  : Delay_MS
* Description    : 1mS Delay at 56M 
* Input          : the number of delay in mS
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_MS(unsigned int nMS)
{
  unsigned int i,j;
  for(i = 0; i < nMS; i++)
  {
    for(j = 0; j < 6048; j++)
    {
      asm("nop");
    }
  }
}
#else
	#error "SYSCLK no define"
#endif

/******************* (C) COPYRIGHT 2009 MYTech *****END OF FILE****/


