#include "Robot.h"
#include "IO.h"
#include "PWM.h"
#include "ADC.h"
#include "timer.h"
#include "UART_Protocol.h"


void avoidingObstacles() {
    
    if(STOP == 1){
//        PWMSpeedConsigne(0.0, MOTEUR_DROIT);
//        PWMSpeedConsigne(0.0, MOTEUR_GAUCHE);
        //return;
    }
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
        unsigned char payload_telemetre[4];
        payload_telemetre[0] = (unsigned char)robotState.distanceTelemetreExtremGauche;
        payload_telemetre[1] = (unsigned char)robotState.distanceTelemetreGauche;
        payload_telemetre[2] = (unsigned char)robotState.distanceTelemetreCentre;
        payload_telemetre[3] = (unsigned char)robotState.distanceTelemetreDroit;
        payload_telemetre[4] = (unsigned char)robotState.distanceTelemetreExtremDroit;
        UartEncodeAndSendMessage((int)0x0030, 5, (unsigned char*)payload_telemetre);
    }

    int sensorCase = 0;
    float limitS = 28.0; //30.0
    float limitSs = 28.0; //35.0

    if (robotState.distanceTelemetreExtremDroit <= limitSs) {
        sensorCase += 0b10000;
    }
    if (robotState.distanceTelemetreDroit <= limitS) {
        sensorCase += 0b1000;
    }
    if (robotState.distanceTelemetreCentre <= limitS) {
        sensorCase += 0b100;
    }
    if (robotState.distanceTelemetreGauche <= limitS) {
        sensorCase += 0b10;
    }
    if (robotState.distanceTelemetreExtremGauche <= limitSs) {
        sensorCase += 0b1;
    }

    float speedNormal = 0.0;//24.0;
    float speedTurn = 0.0;//3.0  //0
    float speedSlow = 0.0;//-5.0;//6.0 //-5
    
    
    
//    if(robotState.distanceTelemetreDroit <= 19.0 && robotState.distanceTelemetreGauche <= 19.0)
//    {
//        PWMSpeedConsigne(-speedNormal+8, MOTEUR_DROIT);
//        PWMSpeedConsigne(speedNormal-8, MOTEUR_GAUCHE);
//        robotState.delay = 1;
//        robotState.delayTime = 1;
//        robotState.turn = 1;
//        return;
//    }
    
    

    if(sensorCase == 0b00000){sensorCase = 0;}
    if(sensorCase == 0b00001){sensorCase = 1;}
    if(sensorCase == 0b00010){sensorCase = 2;}
    if(sensorCase == 0b00011){sensorCase = 2;}
    if(sensorCase == 0b00100){sensorCase = 4;}
    if(sensorCase == 0b00101){sensorCase = 10;}
    if(sensorCase == 0b00110){sensorCase = 10;}
    if(sensorCase == 0b00111){sensorCase = 10;}
    if(sensorCase == 0b01000){sensorCase = 8;}
    if(sensorCase == 0b01001){sensorCase = 10;}
    if(sensorCase == 0b01010){sensorCase = 12;}
    if(sensorCase == 0b01011){sensorCase = 10;}
    if(sensorCase == 0b01100){sensorCase = 12;}
    if(sensorCase == 0b01101){sensorCase = 10;}
    if(sensorCase == 0b01110){sensorCase = 10;}
    if(sensorCase == 0b01111){sensorCase = 10;}
    if(sensorCase == 0b10000){sensorCase = 9;}
    if(sensorCase == 0b10001){sensorCase = 0;}
    if(sensorCase == 0b10010){sensorCase = 10;}
    if(sensorCase == 0b10011){sensorCase = 10;}
    if(sensorCase == 0b10100){sensorCase = 12;}
    if(sensorCase == 0b10101){sensorCase = 12;}
    if(sensorCase == 0b10110){sensorCase = 12;}
    if(sensorCase == 0b10111){sensorCase = 10;}
    if(sensorCase == 0b11000){sensorCase = 8;}
    if(sensorCase == 0b11001){sensorCase = 12;}
    if(sensorCase == 0b11010){sensorCase = 12;}
    if(sensorCase == 0b11011){sensorCase = 12;}
    if(sensorCase == 0b11100){sensorCase = 12;}
    if(sensorCase == 0b11101){sensorCase = 12;}
    if(sensorCase == 0b11110){sensorCase = 12;}
    if(sensorCase == 0b11111){sensorCase = 12;}
 
    
    
    
    
//    switch (sensorCase) {
//        case 0://AVANT
//            PWMSpeedConsigne(speedNormal, MOTEUR_DROIT);
//            PWMSpeedConsigne(speedNormal - 1.0, MOTEUR_GAUCHE);
//            break;
//        case 1://-GAUCHE
//            PWMSpeedConsigne(speedNormal - 8.0, MOTEUR_DROIT);
//            PWMSpeedConsigne(speedTurn, MOTEUR_GAUCHE);
//            break;
//        case 2://+GAUCHE
//            PWMSpeedConsigne(speedNormal - 8.0, MOTEUR_DROIT);
//            PWMSpeedConsigne(speedSlow, MOTEUR_GAUCHE);
//            break;
//        case 4://MILIEU CHOIX
//            //if (robotState.distanceTelemetreDroit <= robotState.distanceTelemetreGauche) {
//                PWMSpeedConsigne(speedNormal - 8.0, MOTEUR_DROIT);
//                PWMSpeedConsigne(speedSlow, MOTEUR_GAUCHE);
////            } else {
////                PWMSpeedConsigne(speedSlow, MOTEUR_DROIT);
////                PWMSpeedConsigne(speedNormal - 8.0, MOTEUR_GAUCHE);
////            }
//            break;
////        case 5:
////            PWMSpeedConsigne(speedNormal - 5, MOTEUR_DROIT);
////            PWMSpeedConsigne(speedSlow, MOTEUR_GAUCHE);
////            break;
//        case 8://+DROIT
//            PWMSpeedConsigne(speedSlow, MOTEUR_DROIT);
//            PWMSpeedConsigne(speedNormal - 8.0, MOTEUR_GAUCHE);
//            break;
//        case 9://-DROIT
//            PWMSpeedConsigne(speedTurn, MOTEUR_DROIT);
//            PWMSpeedConsigne(speedNormal - 8.0, MOTEUR_GAUCHE);
//            break;
//        case 10:// 1/2 TOUR GAUCHE
//            PWMSpeedConsigne(speedNormal-8.0, MOTEUR_DROIT);
//            PWMSpeedConsigne(-speedNormal+8.0, MOTEUR_GAUCHE);
//            break;
//        case 12:// 1/2 TOUR DROIT
//            PWMSpeedConsigne(-speedNormal+8.0, MOTEUR_DROIT);
//            PWMSpeedConsigne(speedNormal-8.0, MOTEUR_GAUCHE);
//            break;
//        case 20: // ARRET
//            PWMSpeedConsigne(0.0, MOTEUR_DROIT);
//            PWMSpeedConsigne(0.0, MOTEUR_GAUCHE);
//            break;


    //}
}