/*****************************Copyright (c)**********************************
**--------------File Info----------------------------------------------------
** File name:			    Main.c
** Created by:		    Alex Hsu
** Created date:		  2009-25-06
**
******************************************************************************
------------------------------Vervision---------------------------------------
**
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "hw_config.h"
#include "Delay.h"
#include "USART.h"
#include "ADC_Sample.h"
#include "fpga.h"
#include "XRS_Control.h"

/* Private variables ---------------------------------------------------------*/

extern volatile unsigned int ADC_DualConvertedValueTab[32];

extern unsigned char Power_Flag; //XRS power flag: 0 on; 1 off;

void sys_init(void); //ϵͳ��ʼ��

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    :  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

int main(void)
{
  /* Add your application code here
     */
  unsigned char i;
  //int j=0;
  //u8 tab[42];

  HW_SetSystem(); //RCC��GPIO��NVIC��EXTI����
  sys_init();     //ϵͳ��ʼ��

  Power_Flag = 1;
  printf("I want to leand git by hand not only by eye\n");
  /* Infinite loop */
  while (1)
  {

    // FPGA_TEST_REG=0x5555;
    //  FPGA_AFEBOARD_HORIZON_NUM=0x55;
    //Delay_NS(10);
    // FPGA_TEST_REG=0xaaaa;
    //  FPGA_AFEBOARD_HORIZON_NUM=0xaa;
    //Delay_NS(10);
    RS232_COM();

    //1�����������
    //�����������,����Ҫ��һ��timer��������յ�����һ���ַ������������timeout�ˣ���û���յ�6��char��
    //���ʾ����ͨ���д���Ҫ��λbuffer,�ȴ�������һ������

    /*if(com_buffer_cnt == 5) //buffer full
  	{
  	  COM_Process();
  	  com_buffer_cnt=0;  //reset
  	}
  	else if(com_buffer_cnt > 0) //�յ�����
  	{
  		//����timer����
  		i++;
  		
  		if(i=65000) //timeout
  		{
  			com_buffer_cnt=0; //��λ
  		}
  	}
  	else
  	{
  		i=0;
  	}*/

    Output_State(); //Output state of XRS to FPGA

    //2��USB�����

    //3��ϵͳ״̬���

    if ((Power_Flag == 1))
    {
      for (i = 0; i < 30; i++)
      {
        Send_PC_Cutoff();
        Delay_MS(1000);
      }
      Power_Cutoff;
      //EX0=1;
      //Power_Flag = 0;
      //Delay_s(1);
      //Power_On;
      Power_Flag = 0;
      Delay_MS(100);
    }
  }
}

void sys_init(void)
{
  XRS_Control_Init();

  FPGA_FSMCConfig();

  ADC_Config();

  //beep
  BUZZER;
  Delay_MS(200);
  BUZZER;
  Delay_MS(200);
  BUZZER;

  SYS_RESET;

  FPGA_AFEBOARD_HORIZON_NUM = 512;

  FPGA_AFEBOARD_VERTICAL_NUM = 512;

  FPGA_AFEBOARD_TOTALPIXEL_NUM = 2048;

  FPGA_INTG_TIME_REG = 140;

  FPGA_AFEBOARD_SCANPIXEL_NUM = 980;

  FPGA_ADC_CTRL_REG = 0x0001; // scan start
}

#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2009 MYTech *****END OF FILE****/
