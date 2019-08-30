/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			    I2c_stm32.c
** Created by:		    Alex Hsu
** Created date:		  2009-05-07
**
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Delay.h"
#include <String.h>
//#include "EEPROM.h"
#include <String.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define EEPROM_Speed              400000
#define EEPROM_SLAVE_ADDRESS7     0xA0
#define EEPROM_PageSize           64
#define EEPROM                    I2C1
/* Private macro -------------------------------------------------------------*/
#define I2C1_SCL                  GPIO_Pin_8  // PB8
#define I2C1_SDA                  GPIO_Pin_9  // PB9
#define SDA_H GPIOB->BSRR = I2C1_SDA
#define SDA_L GPIOB->BRR  = I2C1_SDA
#define SCL_H GPIOB->BSRR = I2C1_SCL
#define SCL_L GPIOB->BRR  = I2C1_SCL
#define SDA_I {GPIOB->CRH  &= 0xffffff0f,\
               GPIOB->CRH  |= 0x00000040;}
#define SDA_O {GPIOB->CRH  &= 0xffffff0f,\
               GPIOB->CRH  |= 0x00000070;}
#define NOP() asm("nop")
/* Private variables ---------------------------------------------------------*/
//unsigned char CPLDDataBuffer[256];
u8 ack;	            	/*应答标志位*/
const unsigned char DefaultConfigData[256]=
{
//0   1     2     3     4     5     6     7     8     9     a     b     c     d     e     f
0x01, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0x00
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0x10
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,//0x20
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00,//0x30
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0x40
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0x50
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0x60
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0x70
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0x80
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0x90
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0xa0
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0xb0
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0xc0
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0xd0
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,//0xe0
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64,//0xf0
};
unsigned char ConfigDataBuffer[256];
/* Extern variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : EEPROM_Configuration
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EEPROM_Configuration(void)
{
  /* Configure IO out */
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Pin = I2C1_SCL | I2C1_SDA;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  SDA_H;
  SDA_H;
}
/*******************************************************************************
* Function Name  : EEPROM_Configuration
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_GenerateSTART(void)
{
	SDA_H;   	  /*发送起始条件的数据信号*/
  NOP();
	NOP();
  SCL_H;
  Delay_NS(2); /*起始条件建立时间大于4.7us,延时*/    
  SDA_L;   	   /*发送起始信号*/
  Delay_NS(2); /* 起始条件锁定时间大于4μs*/       
  SCL_L;   	   /*钳住I2C总线，准备发送或接收数据 */
  NOP();
	NOP();
}
void I2C_GenerateSTOP(void)
{
	SDA_L;  	/*发送结束条件的数据信号*/
  NOP();   
	NOP(); 
  SCL_H;  	/*结束条件建立时间大于4μs*/
  Delay_NS(2);
  SDA_H;  	/*发送I2C总线结束信号*/
  NOP();   
	NOP(); 
	NOP();   
	NOP(); 
}
void I2C_SendData(u8 Data) 
{
	u8 BitCnt;
  
  for(BitCnt = 0; BitCnt < 8; BitCnt++)  /*要传送的数据长度为8位*/
  {
    if((Data << BitCnt) & 0x80)
      SDA_H;   /*判断发送位*/
    else
    {
      SDA_L;
			NOP();   
  		NOP(); 
  		NOP();   
  		NOP(); 
      NOP();   
  		NOP(); 
  		NOP();   
  		NOP(); 
    }
    //Delay_NS(1);
    SCL_H;               /*置时钟线为高，通知被控器开始接收数据位*/
    Delay_NS(10);         /*保证时钟高电平周期大于4μs*/  
    SCL_L;
    Delay_NS(6);
  }
  SDA_H;                /*8位发送完后释放数据线，准备接收应答位*/
  NOP();   
  NOP(); 
	SDA_I;
  SCL_H;
  Delay_NS(6);
  if((GPIOB->IDR & I2C1_SDA) == I2C1_SDA)
    ack=0;     
  else 
    ack=1;        /*判断是否接收到应答信号*/
  SCL_L;
  SDA_O;
  NOP();   
  NOP(); 
  NOP();   
  NOP(); 
}
u8 I2C_ReceiveData(void)
{
	u8 retc;
  u8 BitCnt;
  
  retc=0; 
  SDA_I;             		/*置数据线为输入方式*/
  for(BitCnt=0;BitCnt<8;BitCnt++)
  {
    NOP();           
    SCL_L;                  /*置时钟线为低，准备接收数据位*/
    Delay_NS(9);                 /*时钟低电平周期大于4.7μs*/
    SCL_H;                  /*置时钟线为高使数据线上数据有效*/
    //Delay_NS(1);
    NOP();   
  	NOP(); 
  	NOP();   
  	NOP(); 
    NOP();   
  	NOP(); 
  	NOP();   
  	NOP(); 
    retc=retc<<1;
    if((GPIOB->IDR & I2C1_SDA) == I2C1_SDA)
      retc = retc + 1;  /*读数据位,接收的数据位放入retc中 */
    Delay_NS(7);
  }
  SCL_L;    
  NOP();   
	NOP(); 
	NOP();   
	NOP(); 
  NOP();   
	NOP(); 
	NOP();   
	NOP(); 
  NOP();   
	NOP(); 
	NOP();   
	NOP(); 
  NOP();   
	NOP(); 
	NOP();   
	NOP(); 
	SDA_O;
  return(retc);
}
void I2C_ACK(u8 a)
{
  
  if(a==0)
    SDA_L;     	     /*在此发出应答或非应答信号 */
  else 
    SDA_H;
  Delay_NS(2);    
  SCL_H;
  Delay_NS(5);                    /*时钟低电平周期大于4μs*/ 
  SCL_L;                     /*清时钟线，钳住I2C总线以便继续接收*/
  NOP();   
	NOP(); 
	NOP();   
	NOP();  
  NOP();   
	NOP(); 
	NOP();   
	NOP(); 
}
/*******************************************************************************
* Function Name  : EEPROM_BufferWrite
* Description    :  
* Input          : pBuffer : pointer to the buffer  containing the data to be written to the DEVICE.
**                 WriteAddr : Device's internal address to write to.
**                 NumByteToWrite : number of bytes to write to the Device.
**                 DeviceAddr : Device address to write
**                 DoubleAddr : bytes of device internal address 
* Output         : None
* Return         : None
*******************************************************************************/
void EEPROM_BufferWrite(u8* pBuffer, 
										    u16 WriteAddr, 
										    u16 NumByteToWrite,
										    u8  DeviceAddr,
										    u8  DoubleAddr
										   )
{
  u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % EEPROM_PageSize;
  count = EEPROM_PageSize - Addr;
  NumOfPage =  NumByteToWrite / EEPROM_PageSize;
  NumOfSingle = NumByteToWrite % EEPROM_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      EEPROM_PageWrite(pBuffer, WriteAddr, NumOfSingle, DeviceAddr, DoubleAddr);
      //EEPROM_WaitDEVICEStandbyState(DeviceAddr);
      Delay_MS(6);
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        EEPROM_PageWrite(pBuffer, WriteAddr, EEPROM_PageSize, DeviceAddr, DoubleAddr); 
    		//EEPROM_WaitDEVICEStandbyState(DeviceAddr);
    		Delay_MS(6);
        WriteAddr +=  EEPROM_PageSize;
        pBuffer += EEPROM_PageSize;
      }

      if(NumOfSingle!=0)
      {
        EEPROM_PageWrite(pBuffer, WriteAddr, NumOfSingle, DeviceAddr, DoubleAddr);
        //EEPROM_WaitDEVICEStandbyState(DeviceAddr);
        Delay_MS(6);
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {

			if(count != 0)
      { 
      	if(NumByteToWrite > count)  
      	{
	        EEPROM_PageWrite(pBuffer, WriteAddr, count, DeviceAddr, DoubleAddr);
	        //EEPROM_WaitDEVICEStandbyState(DeviceAddr);
	        Delay_MS(6);
	        WriteAddr += count;
	        pBuffer += count;

					EEPROM_PageWrite(pBuffer, WriteAddr, (NumByteToWrite - count), DeviceAddr, DoubleAddr);
					//EEPROM_WaitDEVICEStandbyState(DeviceAddr);
      		Delay_MS(6);
      	}
        else
        {
          EEPROM_PageWrite(pBuffer, WriteAddr, NumByteToWrite, DeviceAddr, DoubleAddr);
          //EEPROM_WaitDEVICEStandbyState(DeviceAddr);
          Delay_MS(6);
        }
      } 
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / EEPROM_PageSize;
      NumOfSingle = NumByteToWrite % EEPROM_PageSize;	
      
      if(count != 0)
      {  
        EEPROM_PageWrite(pBuffer, WriteAddr, count, DeviceAddr, DoubleAddr);
        //EEPROM_WaitDEVICEStandbyState(DeviceAddr);
        Delay_MS(6);
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        EEPROM_PageWrite(pBuffer, WriteAddr, EEPROM_PageSize, DeviceAddr, DoubleAddr);
        //EEPROM_WaitDEVICEStandbyState(DeviceAddr);
        Delay_MS(6);
        WriteAddr +=  EEPROM_PageSize;
        pBuffer += EEPROM_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        EEPROM_PageWrite(pBuffer, WriteAddr, NumOfSingle, DeviceAddr, DoubleAddr); 
        //EEPROM_WaitDEVICEStandbyState(DeviceAddr);
        Delay_MS(6);
      }
    }
  }  
}
/*******************************************************************************
* Function Name  : EEPROM_ByteWrite
* Description    :  
* Input          : pBuffer : pointer to the buffer  containing the data to be written to the DEVICE.
**                 WriteAddr : DEVICE's internal address to write to.
**                 DeviceAddr : Device address to write
**                 DoubleAddr : bytes of device internal address 
* Output         : None
* Return         : None
*******************************************************************************/
u8 EEPROM_ByteWrite(u8* pBuffer, 
									  u16 WriteAddr,
									  u8  DeviceAddr,
									  u8  DoubleAddr
									 )
{	
  /* Send STRAT condition */
  I2C_GenerateSTART(); 

  /* Send DEVICE address for write */
  I2C_SendData(DeviceAddr);

	if(ack == 0)
		return 0;

	if(DoubleAddr == 2)
	{
  	/* Send the DEVICE's internal address to write to */
  	I2C_SendData((unsigned char)(WriteAddr>>8));
		if(ack == 0)
			return 0;
	}
	
	I2C_SendData((unsigned char)WriteAddr); 

	if(ack == 0)
		return 0;
  
  /* Send the byte to be written */
  I2C_SendData(*pBuffer);
	if(ack == 0)
		return 0;
  
  /* Send STOP condition */
  I2C_GenerateSTOP();
	return 1;
}
/*******************************************************************************
* Function Name  : EEPROM_PageWrite
* Description    :  
* Input          : pBuffer : pointer to the buffer  containing the data to be written to the DEVICE.
**                 WriteAddr : DEVICE's internal address to write to.
**                 NumByteToWrite : number of bytes to write to the DEVICE.
**                 DeviceAddr : Device address to write
**                 DoubleAddr : bytes of device internal address 
* Output         : None
* Return         : None
*******************************************************************************/
u8 EEPROM_PageWrite(u8* pBuffer, 
									  u16 WriteAddr, 
									  u16 NumByteToWrite,
									  u8  DeviceAddr,
									  u8 DoubleAddr
									 )
{
  
  /* Send START condition */
  I2C_GenerateSTART();
  
  /* Send DEVICE address for write */
  I2C_SendData(DeviceAddr);

	if(ack == 0)
		return 0;

	if(DoubleAddr == 2)
	{
  	/* Send the DEVICE's internal address to write to */
  	I2C_SendData((unsigned char)(WriteAddr>>8));
		if(ack == 0)
			return 0;
	}

  /* Send the DEVICE's internal address to write to */
	I2C_SendData((unsigned char)WriteAddr);
	
	if(ack == 0)
		return 0;

  /* While there is data to be written */
  while(NumByteToWrite--)  
  {
    /* Send the current byte */
    I2C_SendData(*pBuffer); 

		if(ack == 0)
		return 0;

    /* Point to the next byte to be written */
    pBuffer++; 
  
  }

  /* Send STOP condition */
  I2C_GenerateSTOP();
  
  return 1;
}
/*******************************************************************************
* Function Name  : EEPROM_BufferRead
* Description    :  
* Input          : pBuffer : pointer to the buffer that receives the data read.
**                 ReadAddr : DEVICE's internal address to read from.
**                 NumByteToRead : number of bytes to read from the DEVICE.
**                 DeviceAddr : Device address to write
**                 DoubleAddr : bytes of device internal address 
* Output         : None
* Return         : None
*******************************************************************************/
u8 EEPROM_BufferRead(u8* pBuffer, 
										 u16 ReadAddr, 
										 u16 NumByteToRead,
										 u8  DeviceAddr,
										 u8  DoubleAddr
									  )
{  
	int i;
  /* Send START condition */
  I2C_GenerateSTART();
   
  /* Send DEVICE address for write */
  I2C_SendData(DeviceAddr);

	if(ack == 0)
		return 0;

	if(DoubleAddr == 2)
	{
  	/* Send the DEVICE's internal address to write to */
  	I2C_SendData((unsigned char)(ReadAddr>>8));

		if(ack == 0)
			return 0;
	}
	
	I2C_SendData((unsigned char)ReadAddr);

	if(ack == 0)
		return 0;
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART();
  
  
  /* Send DEVICE address for read */
  I2C_SendData((DeviceAddr + 1));

	if(ack == 0)
		return 0;
  
  for(i = 0; i < (NumByteToRead - 1); i++)
  {
		*pBuffer = I2C_ReceiveData();
		I2C_ACK(0); // send a ack
		pBuffer++;
  }

	*pBuffer = I2C_ReceiveData();
	I2C_ACK(1);

  I2C_GenerateSTOP();

	return 1;
}
/*******************************************************************************
* Function Name  : EEPROM_BufferRead
* Description    :  
* Input          : pBuffer : pointer to the buffer that receives the data read.
**                 ReadAddr : DEVICE's internal address to read from.
**                 NumByteToRead : number of bytes to read from the DEVICE.
**                 DeviceAddr : Device address to write
**                 DoubleAddr : bytes of device internal address 
* Output         : None
* Return         : None
*******************************************************************************/
u8 EEPROM_ByteRead(u8* pBuffer, 
									 u16 ReadAddr, 
									 u8  DeviceAddr,
									 u8  DoubleAddr
									)
{  
  /* Send START condition */
  I2C_GenerateSTART();
   
  /* Send DEVICE address for write */
  I2C_SendData(DeviceAddr);

	if(ack == 0)
		return 0;

	if(DoubleAddr == 2)
	{
  	/* Send the DEVICE's internal address to write to */
  	I2C_SendData((unsigned char)(ReadAddr>>8));

		if(ack == 0)
			return 0;
	}
	
	I2C_SendData((unsigned char)ReadAddr); 

	if(ack == 0)
		return 0;
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART();
  
  /* Send DEVICE address for read */
  I2C_SendData((DeviceAddr + 1));

	if(ack == 0)
		return 0;

	/* Read a byte from the DEVICE */
  *pBuffer = I2C_ReceiveData(); 
	
	I2C_ACK(1); //send a no ack

  /* Send STOP Condition */
	I2C_GenerateSTOP();

	return 1;

}

/*******************************************************************************
* Function Name  : InitDefaultDataS
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EEPROM_DefaultData(void)
{
	EEPROM_BufferRead(ConfigDataBuffer, 0, 256, EEPROM_ADDRESS, 2);
	while(ConfigDataBuffer[255] != 0x64)
	{
		EEPROM_BufferWrite((u8*)DefaultConfigData, 0, 256, EEPROM_ADDRESS, 2);
    EEPROM_BufferRead(ConfigDataBuffer, 0, 256, EEPROM_ADDRESS, 2);
	}
}
void DAC5578_Init(void)
{
  u32 i;
  u8 tab[2] = {0xff,0xff};
  u8 temp[2] = {0x80,0x80};
  u8 CAByte;
  EEPROM_BufferWrite(tab, 
									  0x60, 
                    2,
									  DAC5578,
									  1
									);
  for(i = 0; i < 6; i++)
  {
    temp[0] = ConfigDataBuffer[i + 1];
    CAByte = 0x30 + i;
    EEPROM_BufferWrite(temp, 
                       CAByte, 
                       2,
                       DAC5578,
                       1
                      );
  }
  for(i = 0; i < 6; i++)
  {
    CAByte = i;
    EEPROM_BufferRead(tab, 
                      CAByte, 
                      2,
                      DAC5578,
                      1
                      );
    CAByte = 0x10 + i;
    EEPROM_BufferRead(tab, 
                      CAByte, 
                      2,
                      DAC5578,
                      1
                      );
  }
//  EEPROM_BufferWrite(tab, 
//									  0x10, 
//                    2,
//									  DAC5578,
//									  1
//									);
//  EEPROM_BufferWrite(tab, 
//									  0x40, 
//                    2,
//									  DAC5578,
//									  1
//									);
//  EEPROM_BufferWrite(tab, 
//									  0x50, 
//                    2,
//									  DAC5578,
//									  1
//									);
//  EEPROM_BufferWrite(tab, 
//									  0x60, 
//                    2,
//									  DAC5578,
//									  1
//									);
//  
//  
//  EEPROM_BufferRead(tab, 
//									  0x10, 
//                    2,
//									  DAC5578,
//									  1
//									);
//  EEPROM_BufferRead(tab, 
//									  0x40, 
//                    2,
//									  DAC5578,
//									  1
//									);
//  EEPROM_BufferRead(tab, 
//									  0x50, 
//                    2,
//									  DAC5578,
//									  1
//									);
//  EEPROM_BufferRead(tab, 
//									  0x60, 
//                    2,
//									  DAC5578,
//									  1
//									);
}
/******************* (C) COPYRIGHT 2009 MYTech *****END OF FILE****************/
