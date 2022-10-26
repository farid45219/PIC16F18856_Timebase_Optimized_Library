/* 
 * File:   uart.h
 * Author: MD. Faridul Islam
 * PIC16F18856 Hardware UART Library
 * Created on October 26, 2022, 15:24 PM
 */

#define UART_BUF_SIZE 64      /*Change according to uart.c file*/

typedef struct uart_t{
    uint8_t          TxPort;
    uint8_t          TxPin;
    uint8_t          RxPort;
    uint8_t          RxPin;
    volatile uint8_t Error;
    volatile uint8_t Buffer[UART_BUF_SIZE];
    volatile uint8_t BufferIndex;
}uart_t;

uart_t UART;

void    UART_Struct_Init(void);
void    UART_Config_Transmitter(void);
void    UART_Config_Receiver(void);
void    UART_Config_Receiver_Interrupt(void);
void    UART_Init(uint32_t BAUD);
void    UART_Transmit_Byte(uint8_t val);
void    UART_Transmit_Byte_Hex(uint8_t val);
void    UART_Transmit_Byte_Bin(uint8_t val);
void    UART_Transmit_Text(char *str);
void    UART_Transmit_Unsigned_Number(uint32_t num);
void    UART_Transmit_Number(int32_t num);
uint8_t UART_Receive_Byte(void);
void    UART_Interrupt_Service_Routine(void);

