/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			    I2c_stm32.c
** Created by:		    Alex Hsu
** Created date:		  2009-05-07
**
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "i2c_stm32.h"
#include "stm32f10x.h"
#include "platform_config.h"
#include "hw_config.h"
#include <String.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define I2C_Speed              100000
#define I2C2_SLAVE_ADDRESS7    0xA0
#define I2C_PageSize           8
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned char CPLDDataBuffer[256];
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

	/* Enable I2C2 reset state */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
  /* Release I2C2 from reset state */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);

  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C2_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
  /* I2C Peripheral Enable */
  I2C_Cmd(I2C2, ENABLE);
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C2, &I2C_InitStructure);
}
/*******************************************************************************
* Function Name  : I2C_BufferWrite
* Description    :  
* Input          : pBuffer : pointer to the buffer  containing the data to be written to the DEVICE.
**                 WriteAddr : Device's internal address to write to.
**                 NumByteToWrite : number of bytes to write to the Device.
**                 DeviceAddr : Device address to write
**                 DoubleAddr : bytes of device internal address 
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_BufferWrite(uint8_t* pBuffer, 
										 uint16_t WriteAddr, 
										 uint16_t NumByteToWrite,
										 uint8_t  DeviceAddr,
										 uint8_t DoubleAddr
										)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle, DeviceAddr, DoubleAddr);
      I2C_WaitDEVICEStandbyState(DeviceAddr);
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_PageWrite(pBuffer, WriteAddr, I2C_PageSize, DeviceAddr, DoubleAddr); 
    		I2C_WaitDEVICEStandbyState(DeviceAddr);
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle, DeviceAddr, DoubleAddr);
        I2C_WaitDEVICEStandbyState(DeviceAddr);
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle, DeviceAddr, DoubleAddr);
      I2C_WaitDEVICEStandbyState(DeviceAddr);
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_PageWrite(pBuffer, WriteAddr, count, DeviceAddr, DoubleAddr);
        I2C_WaitDEVICEStandbyState(DeviceAddr);
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_PageWrite(pBuffer, WriteAddr, I2C_PageSize, DeviceAddr, DoubleAddr);
        I2C_WaitDEVICEStandbyState(DeviceAddr);
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle, DeviceAddr, DoubleAddr); 
        I2C_WaitDEVICEStandbyState(DeviceAddr);
      }
    }
  }  
}
/*******************************************************************************
* Function Name  : I2C_ByteWrite
* Description    :  
* Input          : pBuffer : pointer to the buffer  containing the data to be written to the DEVICE.
**                 WriteAddr : DEVICE's internal address to write to.
**                 DeviceAddr : Device address to write
**                 DoubleAddr : bytes of device internal address 
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_ByteWrite(uint8_t* pBuffer, 
									 uint16_t WriteAddr,
									 uint8_t  DeviceAddr,
									 uint8_t DoubleAddr
									)
{	
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C2, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));  

  /* Send DEVICE address for write */
  I2C_Send7bitAddress(I2C2, DeviceAddr, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	if(DoubleAddr == 2)
	{
  	/* Send the DEVICE's internal address to write to */
  	I2C_SendData(I2C2, (unsigned char)(WriteAddr>>8));	
		/* Test on EV8 and clear it */
  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	
	I2C_SendData(I2C2, (unsigned char)WriteAddr); 
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* Send the byte to be written */
  I2C_SendData(I2C2, *pBuffer); 
   
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C2, ENABLE);
  
  ///* Test on EV8 and clear it */
  //while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  Delay_MS(1);
}
/*******************************************************************************
* Function Name  : I2C_PageWrite
* Description    :  
* Input          : pBuffer : pointer to the buffer  containing the data to be written to the DEVICE.
**                 WriteAddr : DEVICE's internal address to write to.
**                 NumByteToWrite : number of bytes to write to the DEVICE.
**                 DeviceAddr : Device address to write
**                 DoubleAddr : bytes of device internal address 
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_PageWrite(uint8_t* pBuffer, 
									 uint16_t WriteAddr, 
									 uint16_t NumByteToWrite,
									 uint8_t  DeviceAddr,
									 uint8_t DoubleAddr
									)
{
  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
  
  /* Send START condition */
  I2C_GenerateSTART(I2C2, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)); 
  
  /* Send DEVICE address for write */
  I2C_Send7bitAddress(I2C2, DeviceAddr, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	if(DoubleAddr == 2)
	{
  	/* Send the DEVICE's internal address to write to */
  	I2C_SendData(I2C2, (unsigned char)(WriteAddr>>8));	
		/* Test on EV8 and clear it */
  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}

  /* Send the DEVICE's internal address to write to */
	I2C_SendData(I2C2, (unsigned char)WriteAddr); 

  /* Test on EV8 and clear it */
  while(! I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(I2C2, *pBuffer); 

    /* Point to the next byte to be written */
    pBuffer++; 
  
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(I2C2, ENABLE);
}
/*******************************************************************************
* Function Name  : I2C_BufferRead
* Description    :  
* Input          : pBuffer : pointer to the buffer that receives the data read.
**                 ReadAddr : DEVICE's internal address to read from.
**                 NumByteToRead : number of bytes to read from the DEVICE.
**                 DeviceAddr : Device address to write
**                 DoubleAddr : bytes of device internal address 
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_BufferRead(uint8_t* pBuffer, 
										uint16_t ReadAddr, 
										uint16_t NumByteToRead,
										uint8_t  DeviceAddr,
										uint8_t DoubleAddr
									)
{  
    /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
  
  /* Send START condition */
  I2C_GenerateSTART(I2C2, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
   
  /* Send DEVICE address for write */
  I2C_Send7bitAddress(I2C2, DeviceAddr, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C2, ENABLE);

	if(DoubleAddr == 2)
	{
  	/* Send the DEVICE's internal address to write to */
  	I2C_SendData(I2C2, (unsigned char)(ReadAddr>>8));
		/* Test on EV8 and clear it */
  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	
	I2C_SendData(I2C2, (unsigned char)ReadAddr); 

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C2, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* Send DEVICE address for read */
  I2C_Send7bitAddress(I2C2, DeviceAddr, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C2, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C2, ENABLE);
    }

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))  
    {      
      /* Read a byte from the DEVICE */
      *pBuffer = I2C_ReceiveData(I2C2);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C2, ENABLE);

	I2C_WaitDEVICEStandbyState(DeviceAddr);
}
/*******************************************************************************
* Function Name  : I2C_BufferRead
* Description    :  
* Input          : pBuffer : pointer to the buffer that receives the data read.
**                 ReadAddr : DEVICE's internal address to read from.
**                 NumByteToRead : number of bytes to read from the DEVICE.
**                 DeviceAddr : Device address to write
**                 DoubleAddr : bytes of device internal address 
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_ByteRead(uint8_t* pBuffer, 
										uint16_t ReadAddr, 
										uint16_t NumByteToRead,
										uint8_t  DeviceAddr,
										uint8_t DoubleAddr
									)
{  
    /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
  
  /* Send START condition */
  I2C_GenerateSTART(I2C2, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
   
  /* Send DEVICE address for write */
  I2C_Send7bitAddress(I2C2, DeviceAddr, I2C_Direction_Transmitter);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C2, ENABLE);

	if(DoubleAddr == 2)
	{
  	/* Send the DEVICE's internal address to write to */
  	I2C_SendData(I2C2, (unsigned char)(ReadAddr>>8));
		/* Test on EV8 and clear it */
  	while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}
	
	I2C_SendData(I2C2, (unsigned char)ReadAddr); 

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C2, ENABLE);
  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* Send DEVICE address for read */
  I2C_Send7bitAddress(I2C2, DeviceAddr, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
  /* While there is data to be read */
  while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))  
  {
    ;  
  }

	/* Read a byte from the DEVICE */
  *pBuffer = I2C_ReceiveData(I2C2);  

  /* Send STOP Condition */
	I2C_GenerateSTOP(I2C2, ENABLE);

	Delay_MS(10);

}
/*******************************************************************************
* Function Name  : I2C_WaitDEVICEStandbyState
* Description    :  
* Input          : DeviceAddr : Device address to write
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_WaitDEVICEStandbyState(uint8_t DeviceAddr)      
{
  __IO uint16_t SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C2, ENABLE);
    /* Read I2C2 SR1 register */
    SR1_Tmp = I2C_ReadRegister(I2C2, I2C_Register_SR1);
    /* Send DEVICE address for write */
    I2C_Send7bitAddress(I2C2, DeviceAddr, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(I2C2, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C2, I2C_FLAG_AF);
  
  /* STOP condition */    
  I2C_GenerateSTOP(I2C2, ENABLE);  
}
/*******************************************************************************
* Function Name  : InitDefaultDataS
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void InitDefaultData(void)
{
	/* Wait for DEVICE standby state */
	//unsigned char CPLDDataBuffer[10];
  //I2C_CPLD_WaitDEVICEStandbyState();
  //unsigned int i;
	//for(i = 0; i < 10; i++)
	//{
		//I2C_ByteWrite(&(CPLDDataBuffer[i]), i, EEROM_ADDRESS, 2);
	//}
	//I2C_PageWrite((CPLDDataBuffer), 0, 10, EEROM_ADDRESS, 2);
	//I2C_CPLD_WaitDEVICEStandbyState();
	I2C_BufferRead(CPLDDataBuffer, 0, 256, T101A_ADDRESS, 1);
}

/******************* (C) COPYRIGHT 2009 MYTech *****END OF FILE****************/
