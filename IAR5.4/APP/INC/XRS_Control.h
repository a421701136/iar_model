#include "hw_config.h"
#include "Delay.h"
#include "USART.h"



#define Open_Power_ON_PC_CTRL    GPIOG->BSRR = PC_Power_ON_ctrl;
#define Close_Power_ON_PC_CTRL   GPIOG->BRR = PC_Power_ON_ctrl;


//XRS POWER
#define Power_Cutoff   GPIOB->BRR = CUTOFF_OUT;
#define Power_On       GPIOB->BSRR = CUTOFF_OUT;

void Motor_Control(unsigned char Flag);
void Init_System(void);
void Self_Check(void);
void Send_PC_Cutoff(void);
void COM_Process(u8 CMD_Buf[]);
void Output_State(void);
void XRS_Control_Init(void);