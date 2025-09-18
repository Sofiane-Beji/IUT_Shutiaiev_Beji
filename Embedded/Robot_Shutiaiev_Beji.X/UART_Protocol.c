/*
 * File:   UART_Protocol.c
 * Author: E306-PC3
 *
 * Created on 10 décembre 2024, 10:53
 */


#include "xc.h"
#include "CB_TX2.H"
#include "UART_Protocol.h"
#include "IO.h"
#include "CB_TX2.H"
#include "asservissement.h"
#include "Robot.h"
#include "Utilities.h"

unsigned char chk;

unsigned char UartCalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    unsigned char checksum = 0;
    checksum ^= 0xFE;
    checksum ^= (msgFunction >> 8);
    checksum ^= (msgFunction >> 0);
    checksum ^= (msgPayloadLength >> 8);
    checksum ^= (msgPayloadLength >> 0);


    for (int i = 0; i < msgPayloadLength; i++) {
        checksum ^= msgPayload[i];
    }
    return checksum;
}

void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {

    unsigned char tram[6 + msgPayloadLength];
    tram[0] = 0xFE;
    tram[1] = (msgFunction >> 8);
    tram[2] = (msgFunction);
    tram[3] = (msgPayloadLength >> 8);
    tram[4] = (msgPayloadLength);

    for (int i = 5; i < msgPayloadLength + 5; i++) {
        tram[i] = (msgPayload[i - 5]);
    }
    char checkSum = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
    tram[5 + msgPayloadLength] = checkSum;
    SendMessage(tram, 6 + msgPayloadLength);
}
int msgDecodedFunction = 0;
int msgDecodedPayloadLength = 0;
unsigned char msgDecodedPayload[128];
int msgDecodedPayloadIndex = 0;

enum StateReception {
    Waiting,
    FunctionMSB,
    FunctionLSB,
    PayloadLengthMSB,
    PayloadLengthLSB,
    Payload,
    CheckSum
} rcvState;


int CalculateChecksum(int msgFunction, int msgPayloadLength, unsigned char* msgPayload) {
    int checksum = 0xFE;
    
    checksum ^= (msgFunction >> 8);
    checksum ^= (msgFunction);
    checksum ^= (msgPayloadLength >> 8);
    checksum ^= (msgPayloadLength);

    //checksum ^= (int)msgPayload[0];
//    int i=0;
    for (int i = 0; i < msgPayloadLength; i++) {
        int test = (int)msgPayload[i];  
        checksum ^= (int)msgPayload[i];       
    }
    return checksum;

}
/*void UartProcessDecodedMessage(int function, int payloadLength, unsigned char* payload)
{
//Fonction appelee apres le decodage pour executer l?action
//correspondant au message recu
    //UartEncodeAndSendMessage(function,payloadLength,payload);
    if(function == 0x0020)
    {
        int test = payload[1];
//        switch(payload[1])
//        {
//            unsigned char ledState = payload[2];
//            case 0x30:
//                if(ledState == 0x31)
//                    LED_BLANCHE_2 = 1;
//                else if(ledState == 0x30)
//                    LED_BLANCHE_2 = 0;
//            case 0x31:
//                if(ledState == 0x31)
//                    LED_BLEUE_2 = 1;
//                else if(ledState == 0x30)
//                    LED_BLEUE_2 = 0;
//            case 0x32:
//                if(ledState == 0x31)
//                    LED_ORANGE_2 = 1;
//                else if(ledState == 0x30)
//                    LED_ORANGE_2 = 0;
//            case 0x33:
//                if(ledState == 0x31)
//                    LED_ROUGE_2 = 1;
//                else if(ledState == 0x30)
//                    LED_ROUGE_2 = 0;
//            case 0x34:
//                if(ledState == 0x31)
//                    LED_VERTE_2 = 1;
//                else if(ledState == 0x30)
//                    LED_VERTE_2 = 0;
//                
//        }
        if((int)payload[1] == 0x31){
            if(payload[2] == 0x31){
                LED_BLEUE_2 = 1;
            }else if(payload[2] == 0x30){
                LED_BLEUE_2 = 0;
            }
        }else if((int)payload[1] == 0x32){
            if(payload[2] == 0x31){
                LED_ORANGE_2 = 1;
            }else if(payload[2] == 0x30){
                LED_ORANGE_2 = 0;
            }
        }else if((int)payload[1] == 0x33){
            if(payload[2] == 0x31){
                LED_ROUGE_2 = 1;
            }else if(payload[2] == 0x30){
                LED_ROUGE_2 = 0;
            }
        }else if((int)payload[1] == 0x34){//sex    
            if(payload[2] == 0x31){
                LED_VERTE_2 = 1;
            }else if(payload[2] == 0x30){
                LED_VERTE_2 = 0;
            }
        }else if((int)payload[1] == 0x30){
            if(payload[2] == 0x31){
                LED_BLANCHE_2 = 1;
            }else if(payload[2] == 0x30){
                LED_BLANCHE_2 = 0;
            }
        }
    }
    if(function == 0x0040)
    {
        if(payload[1] == 0x00)
        {
            //int speed = atoi(payload & 0x);
            
        }
        else if(payload[1] == 0x01){}
    }
    
    if(function == 0x0069){
        //PidCorrector.Kp = payload[1];
    }


}*/



float correctKp;
float correctKi;
float correctKd;
float limitPX;
float limitPI;
float limitPD;
float correctKpT;
float correctKiT;
float correctKdT;
float limitPXT;
float limitPIT;
float limitPDT;


int rcvFunction;




void UartProcessDecodedMessage(int rcvFunction, int payloadLength, unsigned char* payload) {
    //Fonction appelee apres le decodage pour executer l?action correspondant au message recu
    switch (rcvFunction) {
        case FUNCTION_TEXT:
            UartEncodeAndSendMessage(0x0080, payloadLength, payload);
            break;
        /*case FUNCTION_LED1:
            LED_BLANCHE_1 = payload[0];
            break;
        case FUNCTION_LED2:
            LED_BLEUE_1 = payload[0];
            break;
        case FUNCTION_LED3:
            LED_ORANGE_1 = payload[0];
            break;*/
        /*case SET_ROBOT_STATE:
            SetRobotState(payload[0]);
            break;*/
            
        case CONFIG_PID:
            correctKp = getFloat(payload, 0);
            correctKi = getFloat(payload, 4);
            correctKd = getFloat(payload, 8);
            limitPX = getFloat(payload, 12);
            limitPI = getFloat(payload, 16);
            limitPD = getFloat(payload, 20);
            correctKpT = getFloat(payload, 24);
            correctKiT = getFloat(payload, 28);
            correctKdT = getFloat(payload, 32);
            limitPXT = getFloat(payload, 36);
            limitPIT = getFloat(payload, 40);
            limitPDT = getFloat(payload, 44);
            
            /*correctKp = 1.2f;
            correctKi = 2.9f;
            correctKd = 8.4f;
            limitPX = 5.0f;
            limitPI = 9.4f;
            limitPD = 2.8f;
            correctKpT = 1.2f;
            correctKiT = 2.9f;
            correctKdT = 8.4f;
            limitPXT = 5.0f;
            limitPIT = 9.4f;
            limitPDT = 2.8f;*/
            
            getBytesFromFloat((unsigned char*) &robotState.PidX, 0, (float) (correctKp));
            getBytesFromFloat((unsigned char*) &robotState.PidX, 4, (float) (correctKi));
            getBytesFromFloat((unsigned char*) &robotState.PidX, 8, (float) (correctKd));
            getBytesFromFloat((unsigned char*) &robotState.PidX, 12, (float) (limitPX));
            getBytesFromFloat((unsigned char*) &robotState.PidX, 16, (float) (limitPI));
            getBytesFromFloat((unsigned char*) &robotState.PidX, 20, (float) (limitPD));
            
            
            
            SetupPidAsservissement(&robotState.PidX, (double)correctKp,  (double)correctKi, (double)correctKd, (double)limitPX, (double)limitPI, (double)limitPD);
            SetupPidAsservissement(&robotState.PidTheta, (double)correctKpT,  (double)correctKiT, (double)correctKdT, (double)limitPXT, (double)limitPIT, (double)limitPDT);
            robotState.start = 1;
            
            
            
            //sendPID(0x0063);
            
            break;
            
        case CONFIG_VLINEAIRE:
            robotState.consigneVitesseLineaire = getFloat(payload, 0);
            break;
            
        case CONFIG_VANGULAIRE:
            robotState.consigneVitesseAngulaire = getFloat(payload, 0);
            break;
                  
        default:
            break;

    }

}

//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/

void robotStateChange(unsigned char rbState ) {
    unsigned char msg[5];
    int position = 0;
    unsigned long tstamp = robotState.timestamp;
    msg[position++] = rbState;
    msg[position++] = (unsigned char) (tstamp >> 24);
    msg[position++] = (unsigned char) (tstamp >> 16);
    msg[position++] = (unsigned char) (tstamp >> 8);
    msg[position++] = (unsigned char) tstamp;

    UartEncodeAndSendMessage(0x0050, 5, msg);
}






void UartDecodeMessage(unsigned char c) {
    
    switch (rcvState) {
        case Waiting:
            rcvState = Waiting;
            if (c == 0XFE)
                rcvState = FunctionMSB;
            break;
        case FunctionMSB:
            msgDecodedFunction |= (c << 8);
            rcvState = FunctionLSB;
            break;
        case FunctionLSB:
            msgDecodedFunction |= c;
            msgDecodedFunction = msgDecodedFunction;
            rcvState = PayloadLengthMSB;
            break;
        case PayloadLengthMSB:
            msgDecodedPayloadLength = (c << 8);
            rcvState = PayloadLengthLSB;
            break;
        case PayloadLengthLSB:
            msgDecodedPayloadLength |= c;
            msgDecodedPayloadLength = msgDecodedPayloadLength;
            if (msgDecodedPayloadLength > 0) {
                
                rcvState = Payload;
            } else {
                rcvState = CheckSum;
            }
            break;
       case Payload:
    msgDecodedPayload[msgDecodedPayloadIndex++] = c;
    if (msgDecodedPayloadIndex >= msgDecodedPayloadLength){
        msgDecodedPayloadIndex = 0;
        rcvState = CheckSum;
    }
    break;

        case CheckSum:
            
            chk = CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
            if (chk == c) {
                
                rcvState = Waiting;
                
                UartProcessDecodedMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
                msgDecodedFunction = 0;
                msgDecodedPayloadLength = 0;
                
          
            }

            break;
        
            
              
    }
}


