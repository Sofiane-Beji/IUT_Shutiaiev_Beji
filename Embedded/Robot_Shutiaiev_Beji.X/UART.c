#include <xc.h>
#include "UART.h"
#include "ChipConfig.h"
#include "main.h"
#define BAUDRATE 115200
#define BRGVAL ((FCY/BAUDRATE)/4)-1

void InitUART(void) {
    U2MODEbits.STSEL = 0; // 1-stop bit
    U2MODEbits.PDSEL = 0; // No Parity, 8-data bits
    U2MODEbits.ABAUD = 0; // Auto-Baud Disabled
    U2MODEbits.BRGH = 1; // Low Speed mode
    U2BRG = BRGVAL; // BAUD Rate Setting
    U2STAbits.UTXISEL0 = 0; // Interrupt after one Tx character is transmitted
    U2STAbits.UTXISEL1 = 0;
    IFS1bits.U2TXIF = 0; // clear TX interrupt flag
    IEC1bits.U2TXIE = 0; // Disable UART Tx interrupt
    U2STAbits.URXISEL = 0; // Interrupt after one RX character is received;
    IFS1bits.U2RXIF = 0; // clear RX interrupt flag
    IEC1bits.U2RXIE = 1; // Disable UART Rx interrupt
    U2MODEbits.UARTEN = 1; // Enable UART
    U2STAbits.UTXEN = 1; // Enable UART Tx
}



void SendMessageDirect(unsigned char* message, int length) {
    unsigned char i = 0;
    for (i = 0; i < length; i++) {
        while (U2STAbits.UTXBF); // wait while Tx buffer full
        U2TXREG = *(message)++; // Transmit one character
    }
}
