#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "Toolbox.h"
#include "PWM.h"
#include "ADC.h"
#include "Robot.h"
#include "UART.h"
#include "CB_TX2.h"
#include "CB_RX2.h"
#include "UART_Protocol.h"
#include <libpic30.h>
#include "QEI.h"

int main(void) {

    //Initialisation oscillateur

    InitOscillator();
    InitIO();
    InitADC1();
    InitTimer1();

    InitQEI1();
    InitQEI2();
    
    InitPWM();
    
    InitUART();
    robotState.start = 0;
    InitTrajectoryGenerator();
    
    //PWMSetSpeed(, MOTEUR_DROIT);
    //PWMSetSpeed(-10, MOTEUR_GAUCHE);
    
    //PWMSpeedConsigne(50, MOTEUR_DROIT);
    //PWMSpeedConsigne(50, MOTEUR_GAUCHE);
    //PWMUpdateSpeed();


    // Configuration des input et output (IO)

    LED_BLEUE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;


    

    
    // Boucle Principale
    robotState.avoidingObstaclesBool = 1;
    ADCClearConversionFinishedFlag();
    
    
    
    while (1) {
        //UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
        
        //LED_BLANCHE_1 = !LED_BLANCHE_1;
        for (int i = 0; i < CB_RX2_GetDataSize(); i++) {
            unsigned char c = CB_RX2_Get();
            UartDecodeMessage(c);
            //SendMessage(&c,1);
            
        }
        
        //PWMSpeedConsigne(10, MOTEUR_DROIT);//vmax 3.75 m/s
        //PWMSpeedConsigne(10, MOTEUR_GAUCHE);
        
//        PWMSpeedMS(0.5, MOTEUR_DROIT);
//        PWMSpeedMS(0.5, MOTEUR_GAUCHE);
        //__delay32(10000000);
        
//        if(robotState.distanceTelemetreExtremGauche<20.0){
//            LED_BLANCHE_1 = 1;
//        }else{
//            LED_BLANCHE_1 = 0;
//        }
        if(robotState.distanceTelemetreGauche<20.0){
            LED_BLEUE_1 = 1;
        }else{
            LED_BLEUE_1 = 0;
        }
        if(robotState.distanceTelemetreCentre<20.0){
            LED_ORANGE_1 = 1;
        }else{
            LED_ORANGE_1 = 0;
        }
        if(robotState.distanceTelemetreDroit<20.0){
            LED_ROUGE_1 = 1;
        }else{
            LED_ROUGE_1 = 0;
        }
        if(robotState.distanceTelemetreExtremDroit<20.0){
            LED_VERTE_1 = 1;
        }else{
            LED_VERTE_1 = 0;
        }
        
    } // fin main
    
    
}