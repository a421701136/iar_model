/****************************************Copyright (c)**************************************************
**--------------File Info-------------------------------------------------------------------------------
** File name:			Dot.c
** Created by:		    Alex Hsu
** Created date:		2009-25-06
**
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "PROJ_Config.h"
#include "stdio.h"
#include "KEY.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const unsigned char MonthDaysTable[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
Timer_Typedef DateTime;
unsigned char MinuteFlag;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void RTC_SetYear(void)
{
	char Data[6];
	sprintf(Data,"%d",DateTime.Year);
	Data[0] -= 0x30;
	Data[1] -= 0x30;
	Data[2] -= 0x30;
	Data[3] -= 0x30;
	I2C_EE_ByteWrite((unsigned char*)(&Data[0]), DateTimeYear1Addr);
	//Delay_MS(10);
	I2C_EE_ByteWrite((unsigned char*)(&Data[1]), DateTimeYear2Addr);
	//Delay_MS(10);
	I2C_EE_ByteWrite((unsigned char*)(&Data[2]), DateTimeYear3Addr);
	//Delay_MS(10);
	I2C_EE_ByteWrite((unsigned char*)(&Data[3]), DateTimeYear4Addr);
	//Delay_MS(10);
}
void RTC_SetDateTime(void)
{
	
	unsigned int temp = DateTime.Hour * 3600 + DateTime.Minute * 60;
	/* Enable PWR and BKP clocks */
  
						
  RTC_WaitForLastTask();
  //RTC_EnterConfigMode();
  //RTC_ITConfig(RTC_IT_SEC, DISABLE);
  //RTC_WaitForLastTask();
  RTC_SetCounter(temp);
  RTC_WaitForLastTask();
  //RTC_DisplayDateTime();
  //RTC_ITConfig(RTC_IT_SEC, ENABLE);
  //RTC_WaitForLastTask();
  //RTC_ExitConfigMode();

	RTC_SetYear();
 
	//Delay_MS(10);
	I2C_EE_ByteWrite(&DateTime.Month, DateTimeMonthAddr);
	//Delay_MS(10);
	I2C_EE_ByteWrite(&DateTime.Day, DateTimeDayAddr);
}
void RTC_ChangeYear(unsigned int Flag)
{
	if(Flag)
	{
		DateTime.Year++;
	}
	else
	{
		if(DateTime.Year > 2009)
			DateTime.Year--;
	}
}

void RTC_ChangeMonth(unsigned int Flag)
{
	if(Flag)
	{
		DateTime.Month++;
		if(DateTime.Month == 13)
			DateTime.Month = 1;
	}
	else
	{
		DateTime.Month--;
		if(DateTime.Month == 0)
			DateTime.Month = 12;
	}
}
void RTC_ChangeDay(unsigned int Flag)
{
	if(Flag)
	{
		DateTime.Day++;
		if(DateTime.Day == 32)
			DateTime.Day = 1;
	}
	else
	{
		DateTime.Day--;
		if(DateTime.Day == 0)
			DateTime.Day = 31;
	}
}
void RTC_ChangeHour(unsigned int Flag)
{
	if(Flag)
	{
		DateTime.Hour++;
		if(DateTime.Hour == 24)
			DateTime.Hour = 0;
	}
	else
	{
		//if(DateTime.Hour > 0)
			DateTime.Hour--;
		if(DateTime.Hour == 0xff)
			DateTime.Hour = 23;
	}
}
void RTC_ChangeMinute(unsigned int Flag)
{
	if(Flag)
	{
		DateTime.Minute++;
		if(DateTime.Minute == 60)
			DateTime.Minute = 0;
	}
	else
	{
		DateTime.Minute--;
		if(DateTime.Minute == 0xff)
			DateTime.Minute = 59;
	}
}
void RTC_DisplayMinute(unsigned char X, unsigned char Y)
{
	unsigned char x = X;
	LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(DateTime.Minute / 10) * 16]));
	x++;
  LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(DateTime.Minute % 10) * 16]));
}

void RTC_DisplayHour(unsigned char X, unsigned char Y)
{
	unsigned char x = X;
	LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(DateTime.Hour / 10) * 16]));
	x++;
  LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(DateTime.Hour % 10) * 16]));
}

void RTC_DisplayDay(unsigned char X, unsigned char Y)
{
	unsigned char x = X;
	LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(DateTime.Day / 10) * 16]));
	x++;
  LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(DateTime.Day % 10) * 16]));
}

void RTC_DisplayMonth(unsigned char X, unsigned char Y)
{
	unsigned char x = X;
	LCD_DisplayCharacter(x, Y, 1, 16, BlankCharacter);
	//Delay_MS(100);
	LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(DateTime.Month / 10) * 16]));
	x++;
	LCD_DisplayCharacter(x, Y, 1, 16, BlankCharacter);
	//Delay_MS(100);
  LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(DateTime.Month % 10) * 16]));
}

void RTC_DisplayYear(unsigned char X, unsigned char Y)
{
	char Data[6];
	unsigned char x = X;
	sprintf(Data,"%d",DateTime.Year);
	LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(Data[0] - 0x30) * 16]));
	x++;
  LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(Data[1] - 0x30) * 16]));
	x++;
  LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(Data[2] - 0x30) * 16]));
	x++;
  LCD_DisplayCharacter(x, Y, 1, 16, &(chardot[(Data[3] - 0x30) * 16]));	
}

void RTC_DisplaySettingDateTime(void)
{
	char Data[6];
	sprintf(Data,"%d",DateTime.Year);
	//LCD_DisplayCharacter(1, 8, 1, 16, &(chardot[DOT1]));
  //LCD_DisplayCharacter(2, 8, 1, 16, &(chardot[DOTDOT]));
	//LCD_DisplayCharacter(3, 8, 2, 16, &(HZ16Dot[DOTSHE]));
	//LCD_DisplayCharacter(5, 8, 2, 16, &(HZ16Dot[DOTZHI4]));
	
	RTC_DisplayYear(1, 11);
	LCD_DisplayCharacter(5, 11, 2, 16, &(HZ16Dot[DOTNIAN]));
	KEY_BlockYAddr = 11; 
	KEY_BlockWidth = 4;
	KEY_BlockXAddr = 1;
	KEY_BlockHight = 16;
	KEY_DisplayBlock(KEY_BlockXAddr, KEY_BlockYAddr, KEY_BlockWidth, KEY_BlockHight,0xff);

	RTC_DisplayMonth(7, 11);
	//LCD_DisplayCharacter(9, 11, 1, 16, &(chardot[(DateTime.Month / 10) * 16]));
  //LCD_DisplayCharacter(10, 11, 1, 16, &(chardot[(DateTime.Month % 10) * 16]));
	LCD_DisplayCharacter(9, 11, 2, 16, &(HZ16Dot[DOTYUE]));

	RTC_DisplayDay(11, 11);
	//LCD_DisplayCharacter(13, 11, 1, 16, &(chardot[(DateTime.Day / 10) * 16]));
  //LCD_DisplayCharacter(14, 11, 1, 16, &(chardot[(DateTime.Day % 10) * 16]));
	LCD_DisplayCharacter(13, 11, 2, 16, &(HZ16Dot[DOTRI]));

	RTC_DisplayHour(15, 11);
	//LCD_DisplayCharacter(17, 11, 1, 16, &(chardot[(DateTime.Hour / 10) * 16]));
  //LCD_DisplayCharacter(18, 11, 1, 16, &(chardot[(DateTime.Hour % 10) * 16]));
	LCD_DisplayCharacter(17, 11, 1, 16, &(chardot[DOTCOLON]));

	RTC_DisplayMinute(18, 11);
	//LCD_DisplayCharacter(20, 11, 1, 16, &(chardot[(DateTime.Minute / 10) * 16]));
  //LCD_DisplayCharacter(21, 11, 1, 16, &(chardot[(DateTime.Minute % 10) * 16]));

	//LCD_DisplayCharacter(1, 14, 1, 16, &(chardot[DOT2]));
  //LCD_DisplayCharacter(2, 14, 1, 16, &(chardot[DOTDOT]));
	//LCD_DisplayCharacter(3, 14, 2, 16, &(HZ16Dot[DOTWAN]));
	//LCD_DisplayCharacter(5, 14, 2, 16, &(HZ16Dot[DOTCHENG]));
}
void RTC_DisplayDateTime()
{
	char Data[6];
	sprintf(Data,"%d",DateTime.Year);
	RTC_DisplayYear(21, 28);
	//LCD_DisplayCharacter(21, 28, 1, 16, &(chardot[(Data[0] - 0x30) * 16]));
  //LCD_DisplayCharacter(22, 28, 1, 16, &(chardot[(Data[1] - 0x30) * 16]));
  //LCD_DisplayCharacter(23, 28, 1, 16, &(chardot[(Data[2] - 0x30) * 16]));
  //LCD_DisplayCharacter(24, 28, 1, 16, &(chardot[(Data[3] - 0x30) * 16]));

	RTC_DisplayMonth(27, 28);
	//LCD_DisplayCharacter(27, 28, 1, 16, &(chardot[(DateTime.Month / 10) * 16]));
  //LCD_DisplayCharacter(28, 28, 1, 16, &(chardot[(DateTime.Month % 10) * 16]));

	RTC_DisplayDay(31, 28);
	//LCD_DisplayCharacter(31, 28, 1, 16, &(chardot[(DateTime.Day / 10) * 16]));
  //LCD_DisplayCharacter(32, 28, 1, 16, &(chardot[(DateTime.Day % 10) * 16]));

	RTC_DisplayHour(35, 28);
	//LCD_DisplayCharacter(35, 28, 1, 16, &(chardot[(DateTime.Hour / 10) * 16]));
  //LCD_DisplayCharacter(36, 28, 1, 16, &(chardot[(DateTime.Hour % 10) * 16]));

	RTC_DisplayMinute(38, 28);
	//LCD_DisplayCharacter(38, 28, 1, 16, &(chardot[(DateTime.Minute / 10) * 16]));
  //LCD_DisplayCharacter(39, 28, 1, 16, &(chardot[(DateTime.Minute % 10) * 16]));
}
unsigned int Sum_Leap_Year(unsigned short Year)
{
	if((Year % 400) == 0)
	{
		return 1;
	}
	else if((Year % 4) == 0)
	{
		if((Year % 100) == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
  else
    return 0;
}
void RTC_DateTimeCalculate(void)
{
  DateTime.Minute = 0;
  DateTime.Hour++;
  if(DateTime.Hour == 24)
  {
    DateTime.Hour = 0;
    DateTime.Day++;
  }
  if(DateTime.Month == 2)
  {
    if(Sum_Leap_Year(DateTime.Year))
    {
      if(DateTime.Day == 30)
      {
        DateTime.Day = 1;
        DateTime.Month++;
      }
    }
    else
    {
      if(DateTime.Day == 29)
      {
        DateTime.Day = 1;
        DateTime.Month++;
      }
    }
  }
  else
  {
    if(DateTime.Day == (MonthDaysTable[DateTime.Month] + 1))
    {
      DateTime.Day = 1;
      DateTime.Month++;
    }
  }
  if(DateTime.Month == 13)
  {
    DateTime.Month = 1;
    DateTime.Year++;
  }
}
void RTC_DateParse(unsigned int Second)
{
	//unsigned int day,hour,i;
	//unsigned int flag;
	DateTime.Hour = 0;
	DateTime.Minute = 0;
	while(1)
	{
		if(Second < 60)
		{
			break;
		}
		Second -= 60;
		DateTime.Minute++;
		if(DateTime.Minute == 60)
		{
			DateTime.Minute = 0;
      DateTime.Hour++;
      if(DateTime.Hour == 24)
      {
        DateTime.Hour = 0;
        DateTime.Day++;
      }
      if(DateTime.Month == 2)
      {
        if(Sum_Leap_Year(DateTime.Year))
        {
          if(DateTime.Day == 30)
          {
            DateTime.Day = 1;
            DateTime.Month++;
          }
        }
        else
        {
          if(DateTime.Day == 29)
          {
            DateTime.Day = 1;
            DateTime.Month++;
          }
        }
      }
      else
      {
        if(DateTime.Day == (MonthDaysTable[DateTime.Month] + 1))
        {
          DateTime.Day = 1;
          DateTime.Month++;
        }
      }
      if(DateTime.Month == 13)
      {
        DateTime.Month = 1;
        DateTime.Year++;
      }
		}
	}
}
void RTC_AddOneMinute()
{
	DateTime.Minute++;
	MinuteFlag = 1;
	if(DateTime.Minute == 60)
	{
		DateTime.Minute = 0;
    DateTime.Hour++;
    if(DateTime.Hour == 24)
    {
      DateTime.Hour = 0;
      DateTime.Day++;
			if(DateTime.Month == 2)
    	{
      	if(Sum_Leap_Year(DateTime.Year))
      	{
        	if(DateTime.Day == 30)
        	{
          	DateTime.Day = 1;
          	DateTime.Month++;
						if(DateTime.Month == 13)
    				{
      				DateTime.Month = 1;
      				DateTime.Year++;
							RTC_SetYear();
    				}
						I2C_EE_ByteWrite(&DateTime.Month, DateTimeMonthAddr);
        	}
      	}
      	else
      	{
        	if(DateTime.Day == 29)
        	{
          	DateTime.Day = 1;
          	DateTime.Month++;
						if(DateTime.Month == 13)
    				{
      				DateTime.Month = 1;
      				DateTime.Year++;
							RTC_SetYear();
    				}
						I2C_EE_ByteWrite(&DateTime.Month, DateTimeMonthAddr);
        	}
      	}
			}
			else
    	{
      	if(DateTime.Day == (MonthDaysTable[DateTime.Month] + 1))
      	{
        	DateTime.Day = 1;
        	DateTime.Month++;
					if(DateTime.Month == 13)
    			{
      			DateTime.Month = 1;
      			DateTime.Year++;
						RTC_SetYear();
    			}
					I2C_EE_ByteWrite(&DateTime.Month, DateTimeMonthAddr);
      	}
    	}
			I2C_EE_ByteWrite(&DateTime.Day, DateTimeDayAddr);
			RTC_SetCounter(0);
  		RTC_WaitForLastTask();
    }
		RTC_DisplayDateTime();
	}
	else
	{
		RTC_DisplayMinute(38, 28);
	}
  //RTC_DisplayDateTime();
}
/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval : None
  */
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

void RTC_Init()
{
  unsigned int Counter;
  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);
	
	if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */
    DateTime.Day = 1;
		DateTime.Hour = 0;
		DateTime.Minute = 0;
		DateTime.Month = 1;
		DateTime.Year = 2009;

    /* RTC Configuration */
    RTC_Configuration();

    /* Adjust time by values entred by the user on the hyperterminal */
    /* Wait until last write operation on RTC registers has finished */
  	RTC_WaitForLastTask();
  	/* Change the current time */
  	RTC_SetCounter(0);
  	/* Wait until last write operation on RTC registers has finished */
  	RTC_WaitForLastTask();

    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    

		RTC_DateParse(0);
  }
  else
  {
  	DateTime.Day = DateTimeDay;
		DateTime.Month = DateTimeMonth;
		DateTime.Year = DateTimeYear1 * 1000 + 
			              DateTimeYear2 * 100 +
			              DateTimeYear3 * 10 +
			              DateTimeYear4;
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    
    Counter = (RTC->CNTL | (unsigned int)RTC->CNTH << 16);

		RTC_DateParse(Counter);
		
  }
}