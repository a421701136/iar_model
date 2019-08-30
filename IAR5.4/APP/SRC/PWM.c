/*******************************Copyright (c)***********************************
**--------------File Info-------------------------------------------------------
** File name:			    lcd.c
** Created by:		    Alex Hsu
** Created date:		  2011-06-13
**
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "Delay.h"
#include "PWM.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/                              
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures NVIC and Vector Table base location
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PWM_IRConfiguration(void)
{
  TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
  TIM_OCInitTypeDef         TIM_OCInitStructure;
  GPIO_InitTypeDef          GPIO_InitStructure;
  
  /* Configure IOs as AF Output PP */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure the KEY IO as Input pullup */
  GPIO_InitStructure.GPIO_Pin = (IR_status_in_A | 
                                 IR_status_in_B
                                );
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* TIM3 configuration ------------------------------------------------------*/
	/*TIM3 Frequency = 72MHz/(ARR + 1)/(PSC + 1) = 38400*/
  /* Time Base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = 1874;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* TIM1 channel1 configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
  TIM_OCInitStructure.TIM_Pulse = 937; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;         
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  
  TIM_CtrlPWMOutputs(TIM3, ENABLE);
  
  TIM_Cmd(TIM3, ENABLE);
}