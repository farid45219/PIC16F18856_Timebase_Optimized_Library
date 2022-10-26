

#include<xc.h>
#define _XTAL_FREQ                32000000     /*Configure according to system clock*/


/////////////////////// Device Specific Timer Configurations /////////////////////
#define TIMEBASE_PRESCALER        128    /*Configurable values 1,2,4,8,16,32,64,128*/
#define TIMEBASE_POSTSCALER       5      /*Configurable from 1 to 16*/

#define TIMEBASE_PERIOD_REG       T6PR
#define TIMEBASE_TxCLKCON_REG     T6CLKCON
#define TIMEBASE_CLKCON_MFI31K    0x05
#define TIMEBASE_CLKCON_LFI       0x04
#define TIMEBASE_CLKCON_HFI_16M   0x03
#define TIMEBASE_CLKCON_FOSC      0x02
#define TIMEBASE_CLKCON_FOSCBY4   0x01
#define TIMEBASE_TxCON_REG        T6CON
#define TIMEBASE_ON_BIT_POS       7
#define TIMEBASE_PRESCALE_1       0x00
#define TIMEBASE_PRESCALE_2       0x01
#define TIMEBASE_PRESCALE_4       0x02
#define TIMEBASE_PRESCALE_8       0x03
#define TIMEBASE_PRESCALE_16      0x04
#define TIMEBASE_PRESCALE_32      0x05
#define TIMEBASE_PRESCALE_64      0x06
#define TIMEBASE_PRESCALE_128     0x07
#define TIMEBASE_INT_FLAG_REG     PIR4
#define TIMEBASE_INT_FLAG_BIT_POS 5
#define TIMEBASE_INT_REG          PIE4
#define TIMEBASE_INT_BIT_POS      5
#define TIMEBASE_INTCON_REG       INTCON
#define TIMEBASE_PEIE_BIT_POS     6
#define TIMEBASE_GIE_BIT_POS      7


#define TIMEBASE_SEC_TO_SEC_FACT  50

typedef struct timebase_t{
    volatile uint8_t  Status;
    volatile uint16_t SubSecondsToSecondsFactor;
    uint32_t          LastTimeSample;
    volatile uint32_t ValueSubSeconds;
    volatile uint32_t ValueSeconds;
}timebase_t;

timebase_t Timebase;

void Timebase_Struct_Init(void){
    Timebase.Status=0;
    Timebase.SubSecondsToSecondsFactor=TIMEBASE_SEC_TO_SEC_FACT;
    Timebase.LastTimeSample=0;
    Timebase.ValueSubSeconds=0;
    Timebase.ValueSeconds=0;
}


/////////////////////// Device Specific Timer Configurations /////////////////////
void Timebase_Timer_Config(void){
    TIMEBASE_TxCLKCON_REG=TIMEBASE_CLKCON_FOSCBY4;
    TIMEBASE_TxCON_REG=0x00;
    TIMEBASE_PERIOD_REG=250;
    TIMEBASE_TxCON_REG=(uint8_t)((TIMEBASE_PRESCALE_128<<4)|(TIMEBASE_POSTSCALER-1));
    TIMEBASE_TxCON_REG|=(1<<TIMEBASE_ON_BIT_POS);
    TIMEBASE_INT_REG|=(1<<TIMEBASE_INT_BIT_POS);
    if(!(TIMEBASE_INTCON_REG & (1<<TIMEBASE_PEIE_BIT_POS))){
        TIMEBASE_INTCON_REG|=(1<<TIMEBASE_PEIE_BIT_POS);
    }
    if(!(TIMEBASE_INTCON_REG & (1<<TIMEBASE_GIE_BIT_POS))){
        TIMEBASE_INTCON_REG|=(1<<TIMEBASE_GIE_BIT_POS);
    }
    if(!(TIMEBASE_INT_FLAG_REG & (1<<TIMEBASE_INT_FLAG_BIT_POS))){
        TIMEBASE_INT_FLAG_REG&=~(1<<TIMEBASE_INT_FLAG_BIT_POS);
    }
}
void Timebase_Init(void){
    Timebase_Struct_Init();
    Timebase_Timer_Config();
}

void Timebase_Wait_Unit_Time(void){
    while(Timebase.LastTimeSample==Timebase.ValueSubSeconds);
    Timebase.LastTimeSample=Timebase.ValueSubSeconds;
}

uint8_t Timebase_Total_Tokens_Executing(void){
    return (Timebase.Status & 0x3F);
}

void Timebase_Add_Token(void){
    Timebase.Status+=1;
    if( Timebase_Total_Tokens_Executing() > 63){
        Timebase.Status|=63;
    }
}

void Timebase_Remove_Token(void){
    if( Timebase_Total_Tokens_Executing() > 0){
        Timebase.Status-=1;
    }
}

void Timebase_Remove_All_Tokens(void){
    Timebase.Status&=0xC0;
}

void Timebase_Set_Status_Busy(void){
    Timebase.Status|=(1<<7);
}

void Timebase_Clear_Status_Busy(void){
    Timebase.Status&=~(1<<7);
}

uint8_t Timebase_Check_Busy_Status(void){
    if(Timebase.Status & (1<<7)){
        return 1;
    }else{
        return 0;
    }
}

void Timebase_Set_Timer_Reset_Request_Status(void){
    Timebase.Status|=(1<<6);
}

void Timebase_Clear_Timer_Reset_Request_Status(void){
    Timebase.Status&=~(1<<6);
}

uint8_t Timebase_Check_Timer_Reset_Request_Status(void){
    if(Timebase.Status & (1<<6)){
        return 1;
    }else{
        return 0;
    }
}

uint32_t Timebase_Get_SubSeconds_Value(void){
    Timebase_Add_Token();
    return Timebase.ValueSubSeconds;
}

uint32_t Timebase_Get_Seconds_Value(void){
    Timebase_Add_Token();
    return Timebase.ValueSeconds;
}

void Timebase_Set_SubSeconds_Value(uint32_t value){
    Timebase.ValueSubSeconds=value;
}

void Timebase_Set_Seconds_Value(uint32_t value){
    Timebase.ValueSeconds=value;
}

void Timebase_Reset_SubSeconds_Value(void){
    Timebase.ValueSubSeconds=0;
}

void Timebase_Reset_Seconds_Value(void){
    Timebase.ValueSeconds=0;
}

void Timebase_Safely_Reset_Timer(void){
    if(Timebase_Check_Timer_Reset_Request_Status() && (Timebase_Total_Tokens_Executing()==0)){
        Timebase_Set_Status_Busy();
        Timebase_Reset_SubSeconds_Value();
        //Timebase_Reset_Seconds_Value();
        Timebase_Remove_All_Tokens();
        Timebase_Clear_Timer_Reset_Request_Status();
        Timebase_Clear_Status_Busy();
    }
}

void Timebase_ISR_Executables(void){
    Timebase.ValueSubSeconds++;
    if((Timebase.ValueSubSeconds%Timebase.SubSecondsToSecondsFactor)==0){
        Timebase.ValueSeconds++;
        if(Timebase.ValueSubSeconds>86400000){
            Timebase_Set_Timer_Reset_Request_Status(); 
        }
    }
}


///////////////////////////  Device Specific Functions  //////////////////////////
void Timebase_Interrupt_Service_Routine(void){
    if(TIMEBASE_INT_FLAG_REG & (1<<TIMEBASE_INT_FLAG_BIT_POS)){
        Timebase_ISR_Executables();
        TIMEBASE_INT_FLAG_REG&=~(1<<TIMEBASE_INT_FLAG_BIT_POS);
    }
}

