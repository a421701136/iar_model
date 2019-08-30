/******************** (C) COPYRIGHT 2009 STMicroelectronics ********************
* File Name          : hw_config.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H


/* Includes ------------------------------------------------------------------*/
//#include "usb_type.h"
#include "stm32f10x.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define	ON			                           0
#define	OFF			                           1

#define FPGA_BASE_ADDR                     *((volatile unsigned char * )0x64000000)

//FPGA pin define
#define FSMC_nOE                           GPIO_Pin_4//PD4
#define FSMC_nWE                           GPIO_Pin_5//PD5
#define FSMC_nE2                           GPIO_Pin_9//PG9
#define FSMC_nE1                           GPIO_Pin_7//PD7
#define FSMC_nWait                         GPIO_Pin_6//PD6

#define FSMC_A0                            GPIO_Pin_0//PF0
#define FSMC_A1                            GPIO_Pin_1//PF1
#define FSMC_A2                            GPIO_Pin_2//PF2
#define FSMC_A3                            GPIO_Pin_3//PF3
#define FSMC_A4                            GPIO_Pin_4//PF4
#define FSMC_A5                            GPIO_Pin_5//PF5
#define FSMC_A6                            GPIO_Pin_12//PF12
#define FSMC_A7                            GPIO_Pin_13//PF13
#define FSMC_A8                            GPIO_Pin_14//PF14
#define FSMC_A9                            GPIO_Pin_15//PF15
#define FSMC_A10                           GPIO_Pin_0//PG0
#define FSMC_A11                           GPIO_Pin_1//PG1
#define FSMC_A12                           GPIO_Pin_2//PG2
#define FSMC_A13                           GPIO_Pin_3//PG3
#define FSMC_A14                           GPIO_Pin_4//PG4
#define FSMC_A15                           GPIO_Pin_5//PG5
#define FSMC_A16                           GPIO_Pin_11//PD11
#define FSMC_A17                           GPIO_Pin_12//PD12
#define FSMC_A18                           GPIO_Pin_13//PD13

//#define SYS_nRST                           GPIO_Pin_1//PB10
#define FPGA_nCS                           FSMC_nE2
#define FSMC_D0                            GPIO_Pin_14//PD14
#define FSMC_D1                            GPIO_Pin_15//PD15
#define FSMC_D2                            GPIO_Pin_0 //PD0
#define FSMC_D3                            GPIO_Pin_1 //PD1
#define FSMC_D4                            GPIO_Pin_7 //PE7
#define FSMC_D5                            GPIO_Pin_8 //PE8
#define FSMC_D6                            GPIO_Pin_9 //PE9
#define FSMC_D7                            GPIO_Pin_10//PE10
#define FSMC_D8                            GPIO_Pin_10//PE10
#define FSMC_D9                            GPIO_Pin_10//PE10
#define FSMC_D10                           GPIO_Pin_10//PE10
#define FSMC_D11                           GPIO_Pin_10//PE10
#define FSMC_D12                           GPIO_Pin_10//PE10
#define FSMC_D13                           GPIO_Pin_10//PE10
#define FSMC_D14                           GPIO_Pin_10//PE10
#define FSMC_D15                           GPIO_Pin_10//PE10


// TO PC IO set
#define TXD_MCU                            GPIO_Pin_2 //PA2   -> PC
#define RXD_MCU                            GPIO_Pin_3 //PA3   <- PC
#define PC5V_Detect_STATUS                 GPIO_Pin_4 //PA4  当PC上电后。电平变0, 否则为1
#define PC_Power_ON_ctrl                   GPIO_Pin_7 //PA7  PC电源控制

// I2C
#define sda                                GPIO_Pin_5 //PA5
#define scl                                GPIO_Pin_6 //PA6

//状态输入
#define GZ_IN1_STATUS                      GPIO_Pin_8 // PA8
#define GZ_IN2_STATUS                      GPIO_Pin_9 // PA9
#define CUTOFF_IN_STATUS                   GPIO_Pin_10 //PA10  锁开关输入
#define FPGA_nINT0                         GPIO_Pin_11 //PA11
#define FPGA_nINT1                         GPIO_Pin_12 //PA12
#define SYS_STATUS0                        GPIO_Pin_6  //PC6
#define SYS_STATUS1                        GPIO_Pin_7  //PC7
#define SYS_STATUS2                        GPIO_Pin_8  //PC8
#define SYS_STATUS3                        GPIO_Pin_9  //PC9


// X-ray Power control
#define XRAY_ON_CTRL                       GPIO_Pin_15  //PG15
#define GZ_EN1_CTRL                        GPIO_Pin_10  //PG10
#define GZ_EN2_CTRL                        GPIO_Pin_11  //PG11
#define X_ray_status                       GPIO_Pin_2   //PE2
#define over_vol_status                    GPIO_Pin_3   //PE3
#define over_curr_status                   GPIO_Pin_4   //PE4
#define over_heat_status                   GPIO_Pin_5   //PE5

#define Buzzer_CTRL                        GPIO_Pin_3   //PD3
#define Alarm_LED_CTRL                     GPIO_Pin_5   //PB5
#define MOT_DIR_CTRL                       GPIO_Pin_7   //PB7
#define MOT_EN_CTRL                        GPIO_Pin_8   //PB8
#define CUTOFF_OUT                         GPIO_Pin_9   //PB9
#define SYS_nRST                           GPIO_Pin_10  //PB10

//FPGA registers
#define FPGA_ADC_CTRL_REG                  *((volatile unsigned short * )0x64000002)   //ADC采样与前端积分控制器


#define FPGA_AFEBOARD_HORIZON_NUM          *((volatile unsigned short * )0x64000004)   //水平探测点数量 0-1024
#define FPGA_AFEBOARD_VERTICAL_NUM         *((volatile unsigned short * )0x64000006)   //垂直探测点数量 0-1024
#define FPGA_AFEBOARD_TOTALPIXEL_NUM       *((volatile unsigned short * )0x64000008)   //总共探测点(高能+低能) = ( 水平点_垂直点)*2
#define FPGA_INTG_TIME_REG                 *((volatile unsigned short * )0x6400000a)   //探测板积分时间 20M clolk数
#define FPGA_AFEBOARD_SCANPIXEL_NUM        *((volatile unsigned short * )0x6400000c)   //每个方向上积分扫描点数，不少于水平点数

#define FPGA_USB_CONTROL_REG               *((volatile unsigned short * )0x64000010)   //USB控制寄存器
//#define FPGA_USB_CONTROL_REG               *((volatile unsigned short * )0x64000012)   //USB控制寄存器

#define FPGA_TEST_REG                      *((volatile unsigned short * )0x64000030)   //测试总线用
#define SYS_STATUS_ADDR                    *((volatile unsigned short * )0x6400002a)   //系统状态寄存器

#define BUZZER                             {GPIOD->BSRR = Buzzer_CTRL;\
                                            Delay_MS(200);\
                                            GPIOD->BRR = Buzzer_CTRL;}

#define SYS_RESET                          {GPIOB->BRR = SYS_nRST;\
                                            Delay_MS(200);\
                                            GPIOB->BSRR = SYS_nRST;}       
/* Exported functions ------------------------------------------------------- */
void HW_SetSystem(void);

#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
