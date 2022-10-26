
#include<xc.h>
#define _XTAL_FREQ    32000000    /*Configure according to system clock*/

#define UART_PORTA    0x00        /*Don't Change*/
#define UART_PORTB    0x01        /*Don't Change*/
#define UART_PORTC    0x02        /*Don't Change*/

/*Check the data-sheet for access of the UART in the port PPS*/
/*Also check the device if it has the PPS access*/
#define UART_TX_PORT  UART_PORTB  /*Configurable*/
#define UART_TX_PIN   7           /*Configurable*/
#define UART_RX_PORT  UART_PORTB  /*Configurable*/
#define UART_RX_PIN   6           /*Configurable*/
#define UART_BUF_SIZE 64          /*Configurable, Change in header file also*/

/*Don't Change until you're completely aware of the effects*/
#define UART_TX_PPS_VAL    0x10  
#define UART_RX_PPS_REG    RXPPS
#define UART_TXSTA_REG     TX1STA
#define UART_TRMT_BIT_POS  1
#define UART_BRGH_BIT_POS  2
#define UART_TXEN_BIT_POS  5
#define UART_RCSTA_REG     RC1STA
#define UART_OERR_BIT_POS  1
#define UART_FERR_BIT_POS  2
#define UART_CREN_BIT_POS  4
#define UART_SPEN_BIT_POS  7
#define UART_BAUDCON_REG   BAUD1CON
#define UART_BRG16_BIT_POS 3
#define UART_SPBRG_REG     SP1BRG
#define UART_TXREG         TX1REG
#define UART_RXREG         RC1REG
#define UART_PIR_REG       PIR3
#define UART_RCIF_BIT_POS  5
#define UART_PIE_REG       PIE3
#define UART_RCIE_BIT_POS  5
#define UART_INTCON_REG    INTCON
#define UART_PEIE_BIT_POS  6
#define UART_GIE_BIT_POS   7

/*Don't change anything below this line*/
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

void UART_Struct_Init(void){
    UART.TxPort=UART_TX_PORT;
    UART.TxPin=UART_TX_PIN;
    UART.RxPort=UART_RX_PORT;
    UART.RxPin=UART_RX_PIN;
    for(uint8_t i=0;i<UART_BUF_SIZE;i++){UART.Buffer[i]=0;}
    UART.BufferIndex=0;
}

void UART_Config_Transmitter(void){
    if     (UART.TxPort==UART_PORTA){
        TRISA|=(uint8_t)(1<<UART.TxPin);
        ANSELA&=(uint8_t)(~(1<<UART.TxPin));
        if     (UART.TxPin==0){RA0PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==1){RA1PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==2){RA2PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==3){RA3PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==4){RA4PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==5){RA5PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==6){RA6PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==7){RA7PPS=UART_TX_PPS_VAL;}
    }
    else if(UART.TxPort==UART_PORTB){
        TRISB|=(uint8_t)(1<<UART.TxPin);
        ANSELB&=(uint8_t)(~(1<<UART.TxPin));
        if     (UART.TxPin==0){RB0PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==1){RB1PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==2){RB2PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==3){RB3PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==4){RB4PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==5){RB5PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==6){RB6PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==7){RB7PPS=UART_TX_PPS_VAL;}
    }
    else if(UART.TxPort==UART_PORTC){
        TRISC|=(uint8_t)(1<<UART.TxPin);
        ANSELC&=(uint8_t)(~(1<<UART.TxPin));
        if     (UART.TxPin==0){RC0PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==1){RC1PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==2){RC2PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==3){RC3PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==4){RC4PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==5){RC5PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==6){RC6PPS=UART_TX_PPS_VAL;}
        else if(UART.TxPin==7){RC7PPS=UART_TX_PPS_VAL;}
    }
    UART_TXSTA_REG  |= (1<<UART_TXEN_BIT_POS);
    UART_RCSTA_REG  |= (1<<UART_SPEN_BIT_POS);
}

void UART_Config_Receiver(void){
    if     (UART.RxPort==UART_PORTA){
        //TRISA|=(uint8_t)(1<<UART.RxPin);
        //ANSELA&=(uint8_t)(~(1<<UART.RxPin));
        UART_RX_PPS_REG=(uint8_t)UART.RxPin;
    }
    else if(UART.RxPort==UART_PORTB){
        //TRISB|=(uint8_t)(1<<UART.RxPin);
        //ANSELB&=(uint8_t)(~(1<<UART.RxPin));
        UART_RX_PPS_REG=(uint8_t)(UART.RxPin+0x08);
    }
    else if(UART.RxPort==UART_PORTC){
        //TRISC|=(uint8_t)(1<<UART.RxPin);
        //ANSELC&=(uint8_t)(~(1<<UART.RxPin));
        UART_RX_PPS_REG=(uint8_t)(UART.RxPin+0x10);
    }
    UART_RCSTA_REG  |= (1<<UART_CREN_BIT_POS);
    UART_RCSTA_REG  |= (1<<UART_SPEN_BIT_POS);
}

void UART_Config_Receiver_Interrupt(void){
    UART_PIE_REG   |=(1<<UART_RCIE_BIT_POS);
    if(!(UART_INTCON_REG & (1<<UART_PEIE_BIT_POS))){
        UART_INTCON_REG|=(1<<UART_PEIE_BIT_POS);
    }
    if(!(UART_INTCON_REG & (1<<UART_GIE_BIT_POS))){
        UART_INTCON_REG|=(1<<UART_GIE_BIT_POS); 
    }
}

void UART_Init(uint32_t BAUD){
    uint32_t BRG_VAL=_XTAL_FREQ;
    BRG_VAL=((BRG_VAL/4)/BAUD)-1;
    UART_SPBRG_REG=(uint16_t)BRG_VAL;
    UART_BAUDCON_REG |= (1<<UART_BRG16_BIT_POS);
    UART_TXSTA_REG   |= (1<<UART_BRGH_BIT_POS);
    UART_Struct_Init();
    UART_Config_Transmitter();
    UART_Config_Receiver();
    UART_Config_Receiver_Interrupt();
}

void UART_Transmit_Byte(uint8_t val){
    UART_TXREG=val;
    while((UART_TXSTA_REG & (1<<UART_TRMT_BIT_POS))==0);
}

void UART_Transmit_Byte_Hex(uint8_t val){
    uint8_t hex_digits[2];
    hex_digits[0]=(uint8_t)(val>>4);
    hex_digits[1]=(uint8_t)(val & 0x0F);
    if(hex_digits[0]>9){hex_digits[0]+=55;}
    else {hex_digits[0]+=48;}
    if(hex_digits[1]>9){hex_digits[1]+=55;}
    else {hex_digits[1]+=48;}
    UART_Transmit_Byte('0');
    UART_Transmit_Byte('x');
    UART_Transmit_Byte(hex_digits[0]);
    UART_Transmit_Byte(hex_digits[1]);
}

void UART_Transmit_Byte_Bin(uint8_t val){
    uint8_t bin_digits[8];
    for(uint8_t i=0;i<8;i++){
        bin_digits[i]= (uint8_t)((val & 0x01)+48);
        val>>=1;
    }
    UART_Transmit_Byte('0');
    UART_Transmit_Byte('b');
    for(uint8_t i=8;i>0;i--){
        uint8_t index=(uint8_t)(i-1);
        UART_Transmit_Byte(bin_digits[index]);
    }
}

void UART_Transmit_Text(char *str){
    uint8_t i=0;
    while(str[i]!='\0'){
        UART_Transmit_Byte(str[i]);
        i++;
    }
}

void UART_Transmit_Unsigned_Number(uint32_t num){
    uint8_t digits=0;
    uint8_t buf[8];
    if(num==0){UART_Transmit_Byte('0');}
    else{
        while(num!=0){
            buf[digits]=num%10;
            num/=10;
            digits++;
        }
        for(uint8_t i=digits; i>0; i--){
            uint8_t temp=i;
            temp-=1;
            temp=buf[temp];
            temp+=48;
            UART_Transmit_Byte(temp);
        }
    }
}

void UART_Transmit_Number(int32_t num){
    if(num<0){UART_Transmit_Byte('-');num=-num;}
    UART_Transmit_Unsigned_Number((uint32_t)num);
}

uint8_t UART_Receive_Byte(void){
    uint8_t val=0;
    if(UART_RCSTA_REG & (1<<UART_FERR_BIT_POS)){
        val=UART_RCSTA_REG;
        val=UART_RXREG;
        UART.Error=0x01;
    }
    else if(UART_RCSTA_REG & (1<<UART_OERR_BIT_POS)){
        UART_RCSTA_REG&=~(1<<UART_CREN_BIT_POS);
        UART_RCSTA_REG|=(1<<UART_CREN_BIT_POS);
        UART.Error=0x02;
    }
    else{
        val=UART_RXREG;
        UART.Error=0x00;
    }
    return val;
}

void UART_Interrupt_Service_Routine(void){
    if(UART_PIR_REG & (1<<UART_RCIF_BIT_POS)){
        volatile uint8_t received_byte=UART_Receive_Byte();
        if(UART.Error==0){
            UART.Buffer[UART.BufferIndex]=received_byte;
            UART.BufferIndex++;
            if(UART.BufferIndex>=UART_BUF_SIZE){UART.BufferIndex=0;}
        }
    }
}

