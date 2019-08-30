#include "XRS_Control.h"



unsigned char MOTOR_STU1;
unsigned char MOTOR_STU2;
unsigned char GZ_EN1_STU;
unsigned char GZ_EN2_STU;
unsigned char GZ_IN1_STU;
unsigned char GZ_IN2_STU;
unsigned char XRAY_LED_STU;
unsigned char WARN_LED_STU;
unsigned char VCC5V_DET;


#define Open_GZ1          {GPIOG->BSRR=GZ_EN1_CTRL;\
                           GZ_EN1_STU=1;}

#define Close_GZ1         {GPIOG->BRR=GZ_EN1_CTRL;\
                           GZ_EN1_STU=0;}

#define Open_GZ2          {GPIOG->BSRR=GZ_EN2_CTRL;\
                           GZ_EN2_STU=1;}

#define Close_GZ2         {GPIOG->BRR=GZ_EN2_CTRL;\
                           GZ_EN2_STU=0;}

#define Open_XRAY_LED     {GPIOG->BRR = XRAY_ON_CTRL;\
                           XRAY_LED_STU = 1;}
       
#define Close_XRAY_LED    {GPIOG->BSRR  = XRAY_ON_CTRL;\
                           XRAY_LED_STU = 0;} 

#define Open_WARN_LED     {GPIOB->BRR = Alarm_LED_CTRL; \
                           WARN_LED_STU = 1;}
#define Close_WARN_LED    {GPIOB->BSRR  = Alarm_LED_CTRL;\
                           WARN_LED_STU = 0;}

#define Get_GZ_IN1_STU    {GZ_IN1_STU = ((GPIOA->IDR & GZ_IN1_STATUS) == GZ_IN1_STATUS) ? 1 : 0;}

#define Get_GZ_IN2_STU    {GZ_IN2_STU = ((GPIOA->IDR & GZ_IN2_STATUS) == GZ_IN2_STATUS) ? 1 : 0;}

#define MOTOR_STOP_CTRL   {GPIOB->BRR = MOT_DIR_CTRL;\
                           GPIOB->BRR = MOT_EN_CTRL;\
                           MOTOR_STU2 = 0;\
		                       MOTOR_STU1 = 0;}

#define MOTOR_FWD_CTRL    {GPIOB->BRR = MOT_DIR_CTRL;\
                           Delay_MS(500);\
                           GPIOB->BSRR = MOT_EN_CTRL;\
                           MOTOR_STU2 = 0;\
		                       MOTOR_STU1 = 1;}

#define MOTOR_BACK_CTRL   {GPIOB->BRR = MOT_EN_CTRL;\
                           Delay_MS(500);\
                           GPIOB->BSRR = MOT_DIR_CTRL;\
                           Delay_MS(500);\
                           GPIOB->BSRR = MOT_EN_CTRL;\
                           MOTOR_STU2 = 1;\
		                       MOTOR_STU1 = 0;}

#define Get_PC_Power_STU  {VCC5V_DET = ((GPIOA->IDR & PC5V_Detect_STATUS) == PC5V_Detect_STATUS) ? 1 : 0;}

#define		MOTOR_STOP	    0x01
#define		MOTOR_FWD	      0x03
#define		MOTOR_BACK	    0x04

#define 	GZ_IN_ON		    0x01	
#define 	GZ_IN_OFF		    0x00

#define   PC_POWER_OFF    0x01

unsigned char Power_Flag;
unsigned char Chk_Flag = 0x00;



void Motor_Control(unsigned char Flag)
{

	if(Flag == MOTOR_STOP)
	{
		//P3=P3&0xCF;		
		//MOT_FWD_MCU  = 0;
		//Delay_ms(250);
		//MOT_BACK_MCU = 0;
    MOTOR_STOP_CTRL;
		
		Delay_MS(100);
		//Delay_s(1);
		return ;
	}
	if(Flag == MOTOR_FWD)
	{
		MOTOR_FWD_CTRL;
		Delay_MS(100);
		return;
	}
	if(Flag == MOTOR_BACK)
	{
		MOTOR_BACK_CTRL; 
		Delay_MS(100);
		return ;	
	}
}


void Init_System(void)
{
	Close_Power_ON_PC_CTRL;	//0--2s
	
	//Power_On;		
	Close_GZ1;
	Close_GZ2;
  MOTOR_STOP_CTRL;
	//MOT_FWD_MCU  = 0;
	//MOT_BACK_MCU = 0;
	Close_XRAY_LED;
	Close_WARN_LED;
	//Close_GZ1_ERR_LED;
	//Close_GZ2_ERR_LED;
	Delay_MS(4000);	
	//Open_Power_ON_PC_CTRL;
  Get_PC_Power_STU;
	if(VCC5V_DET == PC_POWER_OFF)
	{
		Delay_MS(500);
		Get_PC_Power_STU;
		if(VCC5V_DET == PC_POWER_OFF)
		{
			Open_Power_ON_PC_CTRL;
			Delay_MS(500);	
			Close_Power_ON_PC_CTRL;
		}
	}
	//System_nRST_CTRL = 1;
	//DAQ_nRST_CTRL = 0;
	//Delay_MS(200);	
	//SYS_nRST = 0;
	//DAQ_nRST_CTRL = 1;
}



void Self_Check(void)
{

	Open_GZ1;
	Delay_MS(800);	
	Get_GZ_IN1_STU;
	if(GZ_IN1_STU == GZ_IN_ON)	
	{							
		Chk_Flag = Chk_Flag | 0x01;		
		Open_WARN_LED;	
		//Open_GZ1_ERR_LED;	
		//Open_Power_ON_PC_CTRL;		
		Motor_Control(MOTOR_BACK);	
		Delay_MS(2000);			
		Motor_Control(MOTOR_STOP);	
		//Close_Power_ON_PC_CTRL;		
	}
	else	
	{
		Chk_Flag = Chk_Flag & 0xFE;		
		//Close_GZ1_ERR_LED;
	}
	Close_GZ1;		
	Open_GZ2;	
	Delay_MS(800);	
  Get_GZ_IN2_STU;
	if(GZ_IN2_STU == GZ_IN_ON)	
	{							
		Chk_Flag = Chk_Flag | 0x02;		
		Open_WARN_LED;	
		//Open_GZ2_ERR_LED;	
		//Open_Power_ON_PC_CTRL;		
		Motor_Control(MOTOR_FWD);	
		Delay_MS(2000);			
		Motor_Control(MOTOR_STOP);	
		//Close_Power_ON_PC_CTRL;		
	}
	else	
	{
		Chk_Flag = Chk_Flag & 0xFD;		
		//Close_GZ2_ERR_LED;
	}
	Close_GZ2;		
}


void Send_PC_Cutoff(void)
{
	UART2_SendByte(0xF2);		
	UART2_SendByte(0x00);
	UART2_SendByte(0x00);
	UART2_SendByte(0x00);
	UART2_SendByte(0x72);
	UART2_SendByte(0xFF);
}



//0xFA, //查询
//0xF8, // 电机及光障命令
//0xF7, //射线灯及报警灯命令
//0xF1, //自检命令（复位命令
//0xF2, //关机命令（往返同
//0xF3:  //系统复位命令，复位硬件系统
void COM_Process(u8 CMD_Buf[])
{	
  unsigned char Flag[4];
	switch(CMD_Buf[0]) //命令头
	{
		case 0xFA:  //查询命令
			   
			   Flag[0] = 0x03 & (MOTOR_STU2<<1) | MOTOR_STU1;
			   //GZ_IN1_MCU = 1;
			   //GZ_IN2_MCU = 1;
			   Get_GZ_IN1_STU;
			   Get_GZ_IN2_STU;
			   //Flag[1] = (State_Flag & 0x3C);
			   //Flag[1] = Flag[1] >>2;
         Flag[1] = 0x0F & (GZ_IN2_STU<<3) | (GZ_IN1_STU<<2) |
                    (GZ_EN2_STU<<1) | GZ_EN1_STU;
			   //Flag[2] = (State_Flag & 0xC0);
			   //Flag[2] = Flag[2] >>6;
         Flag[2] = 0x03 & (WARN_LED_STU<<1) | XRAY_LED_STU;
			   if(Chk_Flag != 0x00)
			   {
			   	 Flag[2] = (Flag[2] | 0x10);
			   }
         
			   Flag[3] = 0xFA^Flag[0];
			   Flag[3] = Flag[3]^Flag[1];
			   Flag[3] = Flag[3]^Flag[2];
			   Flag[3] = Flag[3]&0x7F;
         
			   UART2_SendByte(0xFA);
			   UART2_SendByte(Flag[0]);
			   UART2_SendByte(Flag[1]);
			   UART2_SendByte(Flag[2]);
			   UART2_SendByte(Flag[3]);
			   UART2_SendByte(0xFF);
			   
		     break;
		case 0xF8:  //电机及光障命令
		
			   if((CMD_Buf[1]&0x03) == 0)			
			   {
			   	 Motor_Control(MOTOR_STOP);
			   	 Flag[0] = 0x00;	
			   }
			   else if((CMD_Buf[1]&0x03) == 0x01)	
			   {
			   	 Motor_Control(MOTOR_FWD);
			   	 Flag[0] = 0x01;		
			   }
			   else if((CMD_Buf[1]&0x03) == 0x02)
			   {
			   	 Motor_Control(MOTOR_BACK);
			   	 Flag[0] = 0x02;
			   }
         
			   if((CMD_Buf[2]&0x03) == 0x00)
			   {
			   	 Close_GZ1;
			   	 Close_GZ2;
			   	 Flag[1] = 0x00;
			   }
			   else if((CMD_Buf[2]&0x03) == 0x01)
			   {
			   	 Open_GZ1;
			   	 Close_GZ2;
			   	 Flag[1] = 0x01;		
			   }
			   else if((CMD_Buf[2]&0x03) == 0x02)		
			   {
			   	 Close_GZ1;
			   	 Open_GZ2;
			   	 Flag[1] = 0x02;		
			   }
         
			   Flag[2] = 0x00;	
			   Flag[3] = 0xF8^Flag[0];	
			   Flag[3] = Flag[3]^Flag[1];	
			   Flag[3] = Flag[3]^Flag[2];	
			   Flag[3] = Flag[3]&0x7F;		
         
			   UART2_SendByte(0xF8);
			   UART2_SendByte(Flag[0]);
			   UART2_SendByte(Flag[1]);
			   UART2_SendByte(Flag[2]);
			   UART2_SendByte(Flag[3]);
			   UART2_SendByte(0xFF);
			   
		     break;
		case 0xF7:  //射线灯及报警灯命令
      
			   if(((CMD_Buf[1]&0x03) == 0x00) || ((CMD_Buf[1]&0x03) == 0x02))
			   {
			   	 Flag[0] = XRAY_LED_STU;
			   }
			   else if((CMD_Buf[1]&0x03) == 0x01)
			   {
			   	 Close_XRAY_LED;
			     Flag[0] = 0x00;
			   }
			   else if((CMD_Buf[1]&0x03) == 0x03)
			   {
			   	 Open_XRAY_LED;
			   	 Flag[0] = 0x01;
			   }
         
         
			   if(((CMD_Buf[2]&0x03) == 0x00) || ((CMD_Buf[2]&0x03) == 0x02))
			   {
			   	 Flag[1] = WARN_LED_STU;
			   }
			   else if((CMD_Buf[2]&0x03) == 0x01)
			   {
			   	 Close_WARN_LED;
			   	 Flag[1] = 0x00;
			   }
			   else if((CMD_Buf[2]&0x03) == 0x03)
			   {
			   	 Open_WARN_LED;
			   	 Flag[1] = 0x01;
			   }
         
			   Flag[2] = 0x00;
         
			   Flag[3] = 0xF7^Flag[0];
			   Flag[3] = Flag[3]^Flag[1];
			   Flag[3] = Flag[3]^Flag[2];
			   Flag[3] = Flag[3]&0x7F;
         
			   UART2_SendByte(0xF7);
			   UART2_SendByte(Flag[0]);
			   UART2_SendByte(Flag[1]);
			   UART2_SendByte(Flag[2]);
			   UART2_SendByte(Flag[3]);
			   UART2_SendByte(0xFF);
		     
		     break;
		case 0xF1:   //自检命令（复位命令
			   Self_Check();
         
			   Flag[0] = 0x00;		
			   Flag[1] = Chk_Flag; 
			   Flag[2] = 0x00;		
         
			   Flag[3] = 0xF1^Flag[0];	
			   Flag[3] = Flag[3]^Flag[1];	
			   Flag[3] = Flag[3]^Flag[2];	
			   Flag[3] = Flag[3]&0x7F;		
         
			   UART2_SendByte(0xF1);
			   UART2_SendByte(Flag[0]);
			   UART2_SendByte(Flag[1]);
			   UART2_SendByte(Flag[2]);
			   UART2_SendByte(Flag[3]);
			   UART2_SendByte(0xFF);
		     
		     break;
		case 0xF2:  //关机命令（往返同
			   if( (CMD_Buf[1]==0x00) && (CMD_Buf[2]==0x00) && (CMD_Buf[3]==0x00)&& (CMD_Buf[4]==0x72) )
			   {
			   	 Delay_MS(50000);
			   	 Power_Cutoff;
			   	 Delay_MS(100);
			   }
         
		     break;
		case 0xF3:  //系统复位命令
			   if( (CMD_Buf[1]==0x01) && (CMD_Buf[2]==0x02) && (CMD_Buf[3]==0x03)&& (CMD_Buf[4]==0x72))
			   {
           SYS_RESET;
			   	 //GPIO_SetBits(GPIOB,SYS_nRST);
			   	 //DAQ_nRST_CTRL = 0;
			   	 //Delay_MS(200);
           //GPIO_ResetBits(GPIOB,SYS_nRST);
			   	 //System_nRST_CTRL = 0;
			   	 //DAQ_nRST_CTRL = 1;
			   	 UART2_SendByte(0xF3);
			   	 UART2_SendByte(0x01);
			   	 UART2_SendByte(0x02);
			   	 UART2_SendByte(0x03);
			   	 UART2_SendByte(0x72);
			   	 UART2_SendByte(0xFF);
			   }
			   
		     break;
		default:  //error
			   UART2_SendByte(0xFF);
			   UART2_SendByte(0xFF);
			   UART2_SendByte(0xFF);
			   UART2_SendByte(0xFF);
			   UART2_SendByte(0xFF);
			   UART2_SendByte(0xFF);
			   
		     break;
	}
	CMD_Buf[0]=0xFF;
}


unsigned short Get_State_Flag(void)
{
  unsigned short state;//用于保存硬件状态
  unsigned char State_Flag;//硬件状态REG
  State_Flag = 0;
  State_Flag = ((WARN_LED_STU<<7) | (XRAY_LED_STU<<6) | (GZ_IN2_STU<<5) |
               (GZ_IN1_STU<<4) | (GZ_EN2_STU<<3) | (GZ_EN1_STU<<2) |
                 (MOTOR_STU2<<1) | (MOTOR_STU1));  
  state = State_Flag;
  if( WARN_LED_STU == 1 )
    State_Flag = (0x80 | (~State_Flag));
  state = state + State_Flag*256;
  return state;
}


void Output_State(void)
{
	Get_GZ_IN1_STU;
  Get_GZ_IN2_STU;
  SYS_STATUS_ADDR = Get_State_Flag(); //发送X光机状态位到指定地址
}


void XRS_Control_Init(void)
{
	Power_On;
	Close_Power_ON_PC_CTRL;	//0--2s
	Power_Flag = 0;		
	Init_System();		
  UART_Init();	

	Output_State();		
	Self_Check();		

	Power_Flag = 0;	
}