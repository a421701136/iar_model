/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			SPI.c
** Created by:		    Alex Hsu
** Created date:		2009-25-06
**
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "PROJ_Config.h"
#include "stdio.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/* Private define ------------------------------------------------------------*/
#define SPI2_DR_Address  0x4000380C
#define BufferSize       32
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t SPI1_Buffer_Tx[BufferSize] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
                                 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12,
                                 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B,
                                 0x1C, 0x1D, 0x1E, 0x1F, 0x20};
uint8_t SPI2_Buffer_Rx[BufferSize];
volatile TestStatus TransferStatus = FAILED;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void RCC_Configuration1(void)
{
   /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
     initialize the PLL and update the SystemFrequency variable. */
  //SystemInit();

  /* Enable peripheral clocks --------------------------------------------------*/
  /* DMA1 clock enable */
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  /* GPIOA, GPIOB and SPI1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_SPI1, ENABLE);

  /* SPI2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}
void GPIO_Configuration1(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure SPI1 pins: NSS, SCK, MISO and MOSI ----------------------------*/
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  //GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure SPI2 pins: NSS, SCK, MISO and MOSI ----------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
}
void SPI_Config(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	//GPIO_Configuration1();
	/* DMA1 channel4 configuration ---------------------------------------------*/
//  DMA_DeInit(DMA1_Channel4);
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI2_DR_Address;
//  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI2_Buffer_Rx;
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//  DMA_InitStructure.DMA_BufferSize = BufferSize;
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//  DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	 /* SPI1 configuration ------------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);

  /* SPI2 configuration ------------------------------------------------------*/
//  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Rx;
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
//  SPI_Init(SPI2, &SPI_InitStructure);

  /* Enable SPI2 NSS output for master mode */
  SPI_SSOutputCmd(SPI2, ENABLE);

//  /* Enable SPI2 Rx request */
//  SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);

  /* Enable SPI2 */
  SPI_Cmd(SPI2, ENABLE);
//  /* Enable SPI1 */
//  SPI_Cmd(SPI1, ENABLE);

//  /* Enable DMA1 Channel4 */
//  DMA_Cmd(DMA1_Channel4, ENABLE);

//  /* Wait for DMA1 channel4 transfer complete */
//  while (!DMA_GetFlagStatus(DMA1_FLAG_TC4));

//  /* Check the corectness of written data */
//  TransferStatus = Buffercmp(SPI2_Buffer_Rx, SPI1_Buffer_Tx, BufferSize);
//  /* TransferStatus = PASSED, if the transmitted and received data
//     are equal */
//  /* TransferStatus = FAILED, if the transmitted and received data
//     are different */

//  while (1)
//  {}
}
