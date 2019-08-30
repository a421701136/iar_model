/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			I2c_ee.h
** Created by:		    Alex Hsu
** Created date:		2009-05-07
**
********************************************************************************************************/
/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __I2C_EE_H
#define __I2C_EE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
void InitConfigData(void);
/* Exported variables --------------------------------------------------------*/
extern unsigned char ConfigDataBuffer[256];
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* EEPROM Addresses defines */
#define EEPROM_ADDRESS                  0xA0   /* A2A1A0 = 0 */
#define ConfigDataAddr                         0
#define ConfigDataSize                         256
//#define I2C1_SCL                        GPIO_Pin_8 //PB8
//#define I2C1_SDA                        GPIO_Pin_9 //PB9
#define DAC5578                         0x90
#define ZONE1_SENS                      ConfigDataBuffer[1]
#define ZONE2_SENS                      ConfigDataBuffer[2]
#define ZONE3_SENS                      ConfigDataBuffer[3]
#define ZONE4_SENS                      ConfigDataBuffer[4]
#define ZONE5_SENS                      ConfigDataBuffer[5]
#define ZONE6_SENS                      ConfigDataBuffer[6]
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//void EEPROM_Init(void);
void EEPROM_Configuration(void);
u8 EEPROM_ByteWrite(u8* pBuffer, 
                    u16 WriteAddr,
                    u8 DeviceAddr,
                    u8 DoubleAddr);

u8 EEPROM_PageWrite(u8* pBuffer, 
                    u16 WriteAddr, 
                    u16 NumByteToWrite,
                    u8 DeviceAddr,
                    u8 DoubleAddr);

void EEPROM_BufferWrite(u8* pBuffer, 
                        u16 WriteAddr, 
                        u16 NumByteToWrite,
                        u8 DeviceAddr,
                        u8 DoubleAddr);

u8 EEPROM_BufferRead(u8* pBuffer, 
                     u16 ReadAddr, 
                     u16 NumByteToRead,
                     u8 DeviceAddr,
                     u8 DoubleAddr);

u8 EEPROM_ByteRead(u8* pBuffer, 
                   u16 ReadAddr, 
                   u8 DeviceAddr,
                   u8 DoubleAddr);
void DAC5578_Init(void);
void EEPROM_DefaultData(void);

#endif /* __I2C_EE_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/


