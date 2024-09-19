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

int main(void) {

    //Initialisation oscillateur

    InitOscillator();
    InitIO();
    InitADC1();
    InitTimer1();
    InitTimer23();
    InitPWM();

    //PWMSetSpeed(, MOTEUR_DROIT);
    //PWMSetSpeed(-10, MOTEUR_GAUCHE);



    // Configuration des input et output (IO)

    LED_BLEUE_1 = 1;
    LED_ROUGE_1 = 1;
    LED_VERTE_1 = 1;
    LED_BLANCHE_2 = 1;
    LED_BLEUE_2 = 1;
    LED_ORANGE_2 = 1;
    LED_ROUGE_2 = 1;
    LED_VERTE_2 = 1;


    // Boucle Principale

    while (1) {
        //LED_BLANCHE_1 = !LED_BLANCHE_1;
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096;
            robotState.distanceTelemetreExtremGauche = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096;
            robotState.distanceTelemetreExtremDroit = 34 / volts - 5;
        }
        
        if(robotState.distanceTelemetreExtremGauche<30.0){
            LED_BLANCHE_1 = 1;
        }else{
            LED_BLANCHE_1 = 0;
        }
        if(robotState.distanceTelemetreGauche<30.0){
            LED_BLEUE_1 = 1;
        }else{
            LED_BLEUE_1 = 0;
        }
        if(robotState.distanceTelemetreCentre<30.0){
            LED_ORANGE_1 = 1;
        }else{
            LED_ORANGE_1 = 0;
        }
        if(robotState.distanceTelemetreDroit<30.0){
            LED_ROUGE_1 = 1;
        }else{
            LED_ROUGE_1 = 0;
        }
        if(robotState.distanceTelemetreExtremDroit<30.0){
            LED_VERTE_1 = 1;
        }else{
            LED_VERTE_1 = 0;
        }
    } // fin main
}