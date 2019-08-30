/**
  ******************************************************************************
  * @file    Project/Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.0.0
  * @date    04/06/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
//#include "platform_config.h"
#include "ADC_Sample.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//static unsigned char bdata Combdata;

unsigned char Com_Buf_In[256];
volatile u8 RS232ReceiveIn2;

extern unsigned char Power_Flag;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval : None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval : None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval : None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval : None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval : None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval : None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles ADC1 and ADC2 global interrupts requests.
  * @param  None
  * @retval : None
  */
void ADC1_2_IRQHandler(void)
{
	
//	if((ADC1->SR) && ADC_FLAG_EOC)
//	{
//		ADC1_CurrentRegularConvertedValueTab[ADC1_CurrentCount] = ADC1->DR;
//		ADC1_CurrentCount++;
//		/* Clear ADC1 JEOC pending interrupt bit */
//  	ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
//	}
//	if((ADC1->SR) && ADC_FLAG_JEOC)
//	{
//		ADC1_VoltageRegularConvertedValueTab[ADC1_VoltageCount] = ADC1->JDR1;
//		ADC1_VoltageCount++;
//		/* Clear ADC1 JEOC pending interrupt bit */
//		ADC_ClearITPendingBit(ADC1, ADC_IT_JEOC);
//	}
//	if((ADC2->SR) && ADC_FLAG_EOC)
//	{
//		ADC2_CurrentRegularConvertedValueTab[ADC2_CurrentCount] = ADC2->DR;
//		ADC2_CurrentCount++;
//		/* Clear ADC1 JEOC pending interrupt bit */
//  	ADC_ClearITPendingBit(ADC2, ADC_IT_EOC);
//	}
//	if((ADC2->SR) && ADC_FLAG_JEOC)
//	{
//		ADC2_VoltageRegularConvertedValueTab[ADC2_VoltageCount] = ADC2->JDR1;
//		ADC2_VoltageCount++;
//		/* Clear ADC1 JEOC pending interrupt bit */
//		ADC_ClearITPendingBit(ADC2, ADC_IT_JEOC);
//	}
}
/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles ADC1 and ADC2 global interrupts requests.
  * @param  None
  * @retval : None
  */

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval : None
  */
void RTC_IRQHandler(void)
{
//	unsigned int Time;
//  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
//  {
//    Time = (((unsigned int)RTC->CNTH << 16 ) | RTC->CNTL);
//    //SecondCounter++;
//    if((Time % 60 == 0) && (Time > 0))
//		{
//			RTC_AddOneMinute();
//		}
//		if((Time % 15 == 0))
//		{
//			DisplayFlag = 1;
//		}
//    /* Clear the RTC Second interrupt */
//    RTC_ClearITPendingBit(RTC_IT_SEC);
//
//
//    /* Enable time update */
//    //TimeDisplay = 1;
//
//    /* Wait until last write operation on RTC registers has finished */
//    RTC_WaitForLastTask();
//		//Time = RTC_GetCounter();
//		
////    /* Reset RTC Counter when Time is 23:59:59 */
////    if (Time == 0x00015180)
////    {
////      RTC_SetCounter(0x0);
////      /* Wait until last write operation on RTC registers has finished */
////      RTC_WaitForLastTask();
////    }
//  }
}
/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval : None
  */


void USART2_IRQHandler(void)
{	
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //收到数据 处理
  {
//    USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
    Com_Buf_In[RS232ReceiveIn2++] = USART_ReceiveData(USART2);
//    for(i=1;i<6;i++)
//    {
//      OutTime_Com = 0;
//      while((!(USART_GetFlagStatus(USART2,USART_FLAG_RXNE))) && (OutTime_Com<65000))
//        OutTime_Com++;
//      if(OutTime_Com==65000)
//      { 
//        Buf_Size_In = Buf_Size_In - i;
//        i = 9;
//      }
//      else
//        Com_Buf_In[Buf_Size_In++] = USART_ReceiveData(USART2);
//      if((i==5) && (Buf_Size_In<6))
//        BufIn_OverFlow_Flag = 1;
//    }
//    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  }
}




void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
  {
    Power_Flag = 1;
    EXTI_ClearFlag(EXTI_Line10);
    EXTI_ClearITPendingBit(EXTI_Line10);
  }  
}
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
