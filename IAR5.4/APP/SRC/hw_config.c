/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V3.0.1
* Date               : 04/27/2009
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
//#include "platform_config.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ADC_ConvertedValueX = 0;
uint32_t ADC_ConvertedValueX_1 = 0;
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : HW_RCCConfiguration
* Description    : System init 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HW_RCCConfiguration(void)
{
	ErrorStatus HSEStartUpStatus;
/* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
	
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* ADCCLK = PCLK2/4 */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
  
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }

	
  
	/* Enable peripheral clocks --------------------------------------------------*/
  /* Enable AHBPeriph clock */
  RCC_AHBPeriphClockCmd((RCC_AHBPeriph_DMA1 | //DMA1 clock
                         //RCC_AHBPeriph_DMA2
                         RCC_AHBPeriph_FSMC
                         ),
                        ENABLE
                        );
  
	/* Enable APB2Periph clock */
	RCC_APB1PeriphClockCmd((RCC_APB1Periph_TIM2 |
	                        RCC_APB1Periph_TIM3 |
	                        //RCC_APB1Periph_I2C2 |
	                        //RCC_APB1Periph_PWR |
	                        //RCC_APB1Periph_BKP |
	                        //RCC_APB1Periph_SPI2  |
	                        RCC_APB1Periph_USART2   //USART2 clock
	                        ), 
	                       ENABLE
	                       );

  /* Enable APB2Periph clock */
  RCC_APB2PeriphClockCmd((RCC_APB2Periph_GPIOA | //GPIOA clock
                          RCC_APB2Periph_GPIOB | //GPIOB clock
												  RCC_APB2Periph_GPIOC | //GPIOC clock
                          RCC_APB2Periph_GPIOD | //GPIOD clock
                          RCC_APB2Periph_GPIOE | //GPIOE clock
                          RCC_APB2Periph_GPIOF | //GPIOF clock
                          RCC_APB2Periph_GPIOG | //GPIOG clock
                          RCC_APB2Periph_AFIO |
                          RCC_APB2Periph_ADC1 |  //ADC1 clock
												  RCC_APB2Periph_ADC2 |  //ADC2 clock
												  //RCC_APB2Periph_ADC3  //ADC3 clock
												  RCC_APB2Periph_TIM1   //TIM1 clock
												  //RCC_APB2Periph_USART1 |  //USART1 clock
												  //RCC_APB2Periph_SPI1
												  ),
												 ENABLE
												 );
}



/*******************************************************************************
* Function Name  : HW_GPIOConfiguration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HW_GPIOConfiguration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure IOs as AF_PP   A0-A9  */
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |
                                 GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
                                );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      // 外部总线
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  //config A10-A15 & FSMC_nE2
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |
                                 GPIO_Pin_5 | GPIO_Pin_9
                                );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      // 外部总线
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  

  //config A16-A18 & FSMC_nOE, nWE, nWAIT, nE1, D0-3, D13-15
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 |
                                 GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
                                 GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
                                );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      // 外部总线
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  //config D4-12
	GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 |
                                 GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
                                );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      // 外部总线
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  
  //set as input status
	GPIO_InitStructure.GPIO_Pin = (X_ray_status | over_vol_status | over_curr_status | over_heat_status 
                                );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      // input
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  
  
  //set as input status
	GPIO_InitStructure.GPIO_Pin = (SYS_STATUS0 | SYS_STATUS1 | SYS_STATUS2 | SYS_STATUS3 
                                );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      // input
  GPIO_Init(GPIOC, &GPIO_InitStructure);


  //set as input status
	GPIO_InitStructure.GPIO_Pin = (PC5V_Detect_STATUS | GZ_IN1_STATUS | GZ_IN2_STATUS | CUTOFF_IN_STATUS 
                                 | FPGA_nINT0 | FPGA_nINT1
                                );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      // input
  GPIO_Init(GPIOA, &GPIO_InitStructure);


  //set as output status
	GPIO_InitStructure.GPIO_Pin = (PC_Power_ON_ctrl
                                );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      // output
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIOA->BRR = PC_Power_ON_ctrl;
  
  //set as output status
	GPIO_InitStructure.GPIO_Pin = (Alarm_LED_CTRL | MOT_DIR_CTRL | MOT_EN_CTRL | CUTOFF_OUT | SYS_nRST
                                );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      // output
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIOB->BRR = MOT_DIR_CTRL | MOT_EN_CTRL | CUTOFF_OUT;
  GPIOB->BSRR  = Alarm_LED_CTRL | SYS_nRST;

  


  //set as output status
	GPIO_InitStructure.GPIO_Pin = (Buzzer_CTRL
                                );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      // output
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIOD->BRR = Buzzer_CTRL;

  //set as output status
	GPIO_InitStructure.GPIO_Pin = (XRAY_ON_CTRL | GZ_EN1_CTRL | GZ_EN2_CTRL
                                );
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      // output
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIOG->BRR=GZ_EN1_CTRL | GZ_EN2_CTRL;
  GPIOG->BSRR  = XRAY_ON_CTRL; 
}


/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures NVIC and Vector Table base location
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HW_NVICConfiguration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
	
#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
//  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//  
//  /* Enable the EXTI9_5 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);  
    
  /* Enable the EXTI15_10 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
    /* Enable the DMA1 Channel1 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the DMA1 Channel1 Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure); 

  
  
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
* Function Name : HW_EXTIConfiguration.
* Description   : Configure the EXTI lines for Key and Tamper push buttons.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
void HW_EXTIConfiguration(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
   
  /* Enable the AFIO Clock */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  
    
  /* Connect Tamper puch button to the correspondent EXTI line */
//  GPIO_EXTILineConfig(GPIO_TAMPER_PORTSOURCE, GPIO_TAMPER_PINSOURCE);
//  
//  /* Configure Tamper EXTI Line to generate an interrupt rising & falling edges */  
//  EXTI_InitStructure.EXTI_Line = GPIO_TAMPER_EXTI_Line;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//
//  /* Clear the Tamper EXTI line pending bit */
//  EXTI_ClearITPendingBit(GPIO_TAMPER_EXTI_Line);
  

  
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource10);  
  /* Configure Tamper EXTI Line to generate an interrupt rising & falling edges */  
  EXTI_InitStructure.EXTI_Line = EXTI_Line10;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);  
  /* Clear the Tamper EXTI line pending bit */
  EXTI_ClearITPendingBit(GPIO_PinSource10);
}

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void HW_SetSystem(void)
{
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
     initialize the PLL and update the SystemFrequency variable. */
  //SystemInit();
  HW_RCCConfiguration();
  
  /* Configure the used GPIOs*/
  HW_GPIOConfiguration();
  
  /* Configure the EXTI lines for Key and Tamper push buttons*/
  HW_EXTIConfiguration();

  /* Configure the NVIC*/
  HW_NVICConfiguration();

}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
