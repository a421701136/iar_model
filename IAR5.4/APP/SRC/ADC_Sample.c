/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			ADC_Sample.c
** Created by:		    Alex Hsu
** Created date:		2009-25-05
**
********************************************************************************************************/
/* Includes ------------------------------------------------------------------*/
//#include "platform_config.h"
#include "stm32f10x.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC3_DR_Address    ((uint32_t)0x40013C4C)
#define SPI2_DR_Address    ((uint32_t)0x4000380C)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//#pragma location = ".mysection"
ADC_InitTypeDef           ADC_InitStructure;
DMA_InitTypeDef           DMA1Channel1,DMA2Channel5;
volatile unsigned int ADC_DualConvertedValueTab[32];
DMA_InitTypeDef  DMA1Channel4;
float fTemp;
/* Private function prototypes -----------------------------------------------*/
void SPI_Config(void);
/* Private functions ---------------------------------------------------------*/
/*
*******************************************************************************
** Function name:       M_ADC_Config		
** input parameters:		None
** Returned value:			None
*******************************************************************************
*/
void ADC_Config(void)
{
	/*
	 * ADC1 regular group channels using TIM1->CC1 external trigger
	 * ADC2 regular group channels using TIM1->CC2 external trigger
	 * ADC1 is transfering converted data using DMA
	 * ADC2 converted data are stored using End of conversion interrupt
	 * ADC1->channel 1 measures High_CH_ADC
	 * ADC2->channel 2 measures Low_CH_ADC
	 */
	/* Configure analog input */
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//  GPIO_InitStructure.GPIO_Pin = (coil_sig_adc0 | //PA1->ADC1->channel 1  comment zouxp
//                                 coil_sig_adc1 | //PA2->ADC1->channel 2
//                                 coil_sig_adc2 | //PA3->ADC1->channel 3
//                                 coil_sig_adc3 | //PA4->ADC1->channel 4
//                                 coil_sig_adc4 | //PA5->ADC1->channel 5
//                                 coil_sig_adc5   //PA6->ADC1->channel 6
//                                ); 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = (coil_sig_adc6 | //PC0->ADC2->channel 10 comment zouxp
//                                 coil_sig_adc7 | //PC1->ADC2->channel 11
//                                 coil_sig_adc8 | //PC2->ADC2->channel 12
//                                 coil_sig_adc9 | //PC3->ADC2->channel 13
//                                 coil_sig_adc10| //PC4->ADC2->channel 14
//                                 coil_sig_adc11  //PC5->ADC2->channel 15
//                                ); 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
   
  /* DMA1 Channel1 Configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA1Channel1.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA1Channel1.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;
  DMA1Channel1.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA1Channel1.DMA_BufferSize = 6;
  DMA1Channel1.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA1Channel1.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA1Channel1.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA1Channel1.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA1Channel1.DMA_Mode = DMA_Mode_Normal;
  DMA1Channel1.DMA_Priority = DMA_Priority_Medium;
  DMA1Channel1.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA1Channel1);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
  

  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//ADC_ExternalTrigConv_T3_TRGO;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 6;
  ADC_Init(ADC1, &ADC_InitStructure);
  
  
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_Init(ADC2, &ADC_InitStructure);
  
  /* ADC2 regular channel6 configuration,measure main current in B*/ 
  ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_28Cycles5);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_11, 2, ADC_SampleTime_28Cycles5);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_12, 3, ADC_SampleTime_28Cycles5);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_13, 4, ADC_SampleTime_28Cycles5);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_14, 5, ADC_SampleTime_28Cycles5);
  ADC_RegularChannelConfig(ADC2, ADC_Channel_15, 6, ADC_SampleTime_28Cycles5);
  
  /* ADC1 regular channel7 configuration,measure main current in C*/ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_28Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 2, ADC_SampleTime_28Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_28Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_28Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_28Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_28Cycles5);
  
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  
  
  /* Enable ADC1 external trigger */ 
  ADC_ExternalTrigConvCmd(ADC1, ENABLE);
  
  /* Enable ADC2 external trigger */ 
  ADC_ExternalTrigConvCmd(ADC2, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  
  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));
  
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
  
  /* Enable ADC1 */
  ADC_Cmd(ADC2, ENABLE);
  
  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC2);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC2));
  
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC2);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC2)); 
  
  /* Clear DMA1 channel1 transfer complete flag */
  DMA_ClearFlag(DMA1_FLAG_TC1);
}
/*
*********************************************************************************************************
** Function name:       Start_ADC_Convert	
** input parameters:		Main_Backup:Main or Backup sample
** Returned value:			None
*********************************************************************************************************
*/
void ADC_StartConvert(void)
{

	ADC1->SR = 0;
  
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  /* Wait on channel1 transfer complete flag */
  while(!DMA_GetFlagStatus(DMA1_FLAG_TC1));

	/* Clear DMA1 channel1 transfer complete flag */
  DMA_ClearFlag(DMA1_FLAG_TC1);
  
  DMA1_Channel1->CCR &= 0xfffffffe;
	DMA1_Channel1->CNDTR = 6;
	DMA1_Channel1->CCR |= 0x00000001;
}

void SPI_Config(void)
{
	
//	SPI_InitTypeDef  SPI_InitStructure;
//	//GPIO_Configuration1();
//	/* DMA1 channel4 configuration ---------------------------------------------*/
//  DMA_DeInit(DMA1_Channel4);
//  DMA1Channel4.DMA_PeripheralBaseAddr = (uint32_t)SPI2_DR_Address;
//  DMA1Channel4.DMA_MemoryBaseAddr = (uint32_t)SPI2_Buffer_Rx;
//  DMA1Channel4.DMA_DIR = DMA_DIR_PeripheralSRC;
//  DMA1Channel4.DMA_BufferSize = 15420;
//  DMA1Channel4.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  DMA1Channel4.DMA_MemoryInc = DMA_MemoryInc_Enable;
//  DMA1Channel4.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//  DMA1Channel4.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//  DMA1Channel4.DMA_Mode = DMA_Mode_Normal;
//  DMA1Channel4.DMA_Priority = DMA_Priority_VeryHigh;
//  DMA1Channel4.DMA_M2M = DMA_M2M_Disable;
//  DMA_Init(DMA1_Channel4, &DMA1Channel4);
//
//	 /* SPI2 configuration ------------------------------------------------------*/
//  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Rx;
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//  SPI_InitStructure.SPI_CRCPolynomial = 7;
//
//  SPI_Init(SPI2, &SPI_InitStructure);
//
//  /* Enable SPI2 Rx request */
//  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);
//
//	//SPI_SSOutputCmd(SPI2, ENABLE);
//
//  /* Enable SPI2 */
//  SPI_Cmd(SPI2, ENABLE);
//
//  /* Enable DMA1 Channel4 */
//  DMA_Cmd(DMA1_Channel4, ENABLE);
//
//	/* Clear DMA1 channel1 transfer complete flag */
//  //DMA_ClearFlag(DMA1_FLAG_TC4);
}


