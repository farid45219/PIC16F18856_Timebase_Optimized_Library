/* 
 * File:   timebase.h
 * Author: MD. Faridul Islam
 * PIC16F18856 Timebase Library
 * Created on October 26, 2022, 15:24 PM
 */

void     Timebase_Struct_Init(void);
void     Timebase_Timer_Config(void);
void     Timebase_Init(void);
void     Timebase_Wait_Unit_Time(void);
uint8_t  Timebase_Total_Tokens_Executing(void);
void     Timebase_Add_Token(void);
void     Timebase_Remove_Token(void);
void     Timebase_Remove_All_Tokens(void);
void     Timebase_Set_Status_Busy(void);
void     Timebase_Clear_Status_Busy(void);
uint8_t  Timebase_Check_Busy_Status(void);
void     Timebase_Set_Timer_Reset_Request_Status(void);
void     Timebase_Clear_Timer_Reset_Request_Status(void);
uint8_t  Timebase_Check_Timer_Reset_Request_Status(void);
uint32_t Timebase_Get_SubSeconds_Value(void);
uint32_t Timebase_Get_Seconds_Value(void);
void     Timebase_Set_SubSeconds_Value(uint32_t value);
void     Timebase_Set_Seconds_Value(uint32_t value);
void     Timebase_Reset_SubSeconds_Value(void);
void     Timebase_Reset_Seconds_Value(void);
void     Timebase_Safely_Reset_Timer(void);
void     Timebase_ISR_Executables(void);
void     Timebase_Interrupt_Service_Routine(void);

