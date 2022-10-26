/*
 * File:   main.c
 * Author: MD. Faridul Islam
 * PIC16F18856 Timebase Library
 * Created on October 26, 2022, 15:24 PM
 */


#include <xc.h>
#include "configbits.h"
#include "timebase.h"
#include "uart.h"

void main(void) {
    UART_Init(38400);
    UART_Transmit_Text((uint8_t*)"Serial Log Started!\r\n");
    Timebase_Init();
     
    while(1){
        
        Timebase_Wait_Unit_Time();
        Timebase_Remove_Token();
    }
}


void interrupt ISRs(void){
    UART_Interrupt_Service_Routine();
    Timebase_Interrupt_Service_Routine();
}
