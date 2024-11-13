#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "CB_TX2.h"
#include "UART.h"
#define CBTX2_BUFFER_SIZE 128
int cbTx2Head;
int cbTx2Tail;
unsigned char cbTx2Buffer[CBTX2_BUFFER_SIZE];
unsigned char isTransmitting = 0;

unsigned char CB_TX2_IsTranmitting(void) {
    return isTransmitting;
}

void SendMessage(unsigned char* message, int length) {
    unsigned char i = 0;
    if (CB_TX2_GetRemainingSize() > length) {
        //On peut écrire le message
        for (i = 0; i < length; i++)
            CB_TX2_Add(message[i]);
        if (!CB_TX2_IsTranmitting())
            SendOne();
    }
}

void CB_TX2_Add(unsigned char value) {
    cbTx2Buffer[CB_TX2_GetDataSize()] = value;
}

unsigned char CB_TX2_Get(void) {
    return cbTx2Buffer[0];
}

void __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt(void) {
    IFS1bits.U2RXIF = 0;
    if (cbTx2Tail != cbTx2Head) {
        SendOne();
    } else
        isTransmitting = 0;
}

void SendOne() {
    isTransmitting = 1;
    unsigned char value = CB_TX2_Get();
    U2TXREG = value; // Transmit one character
}



int CB_TX2_GetDataSize(void) {
    //return size of data stored in circular buffer
    int dataSize = cbTx2Head + cbTx2Tail;
    
    return dataSize;
}

int CB_TX2_GetRemainingSize(void) {
    //return size of remaining size in circular buffer
    int remainingSize = CBTX2_BUFFER_SIZE - CB_TX2_GetDataSize();
    
    return remainingSize;
}

