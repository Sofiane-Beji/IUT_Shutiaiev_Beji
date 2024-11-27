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

int main(void) {

    //Initialisation oscillateur

    InitOscillator();
    InitIO();
    InitADC1();
    InitTimer1();

    InitTimer23();
    InitPWM();
    
    InitUART();
    //PWMSetSpeed(, MOTEUR_DROIT);
    //PWMSetSpeed(-10, MOTEUR_GAUCHE);



    // Configuration des input et output (IO)

    LED_BLEUE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;


    


    // Boucle Principale
    robotState.avoidingObstaclesBool = 1;
    while (1) {

        //LED_BLANCHE_1 = !LED_BLANCHE_1;
        for (int i = 0; i < CB_RX2_GetDataSize(); i++) {
            unsigned char c = CB_RX2_Get();
            SendMessage(&c, 1);
            
        }
        
        __delay32(1000);
        
        if(robotState.distanceTelemetreExtremGauche<20.0){
            LED_BLANCHE_1 = 1;
        }else{
            LED_BLANCHE_1 = 0;
        }
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