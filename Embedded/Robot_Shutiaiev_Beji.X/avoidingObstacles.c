#include "Robot.h"
#include "IO.h"
#include "PWM.h"
#include "ADC.h"
#include "timer.h"

/*void avoidingObstacles() {
    if (robotState.avoidingObstaclesBool != 1) {
        return;
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
    }

    int sensorCase = 0;
    float limitS = 29.0; //44.0
    float limitSs = 38.0; //52.0

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

    float speedNormal = 23.0;
    float speedTurn = 9.0;//13.0
    float speedSlow = 5.0;//5.0


    
        if (sensorCase == 0b0000 || sensorCase == 0b10001) {
            sensorCase = 0;
        }
        if (sensorCase == 0b00001) {
            sensorCase = 1;
        }
        if (sensorCase == 0b00010 || sensorCase == 0b00011 || sensorCase == 0b10011 || sensorCase == 0b10010) {
            sensorCase = 2;
        }
        if (sensorCase == 0b00100 || sensorCase == 0b01110) {
            sensorCase = 4;
        }
        if (sensorCase >= 0b00101 && sensorCase <= 0b00111) {
            sensorCase = 5;
        }
        if (sensorCase == 0b01000 || sensorCase == 0b11000 || sensorCase == 0b11001 || sensorCase == 0b01001) {
            sensorCase = 8;
        }
        if (sensorCase == 0b10000) {
            sensorCase = 9;
        }
        if (sensorCase == 0b01010 || sensorCase == 0b01011 || sensorCase == 0b01101 || sensorCase == 0b1111) {
            sensorCase = 10;
        }
        if (sensorCase == 0b01100 || (sensorCase >= 0b10100 && sensorCase <= 0b10111) || (sensorCase >= 0b11010 && sensorCase <= 0b11111)) {
            sensorCase = 12;
        }
    if(STOP == 1){
        sensorCase = 20;
    }

    switch (sensorCase) {
        case 0:
            PWMSpeedConsigne(speedNormal, MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal - 0.7, MOTEUR_GAUCHE);
            break;
        case 1:
            PWMSpeedConsigne(speedNormal - 5, MOTEUR_DROIT);
            PWMSpeedConsigne(speedTurn, MOTEUR_GAUCHE);
            break;
        case 2:
            PWMSpeedConsigne(speedNormal - 5, MOTEUR_DROIT);
            PWMSpeedConsigne(speedSlow, MOTEUR_GAUCHE);
            break;
        case 4:
            if (robotState.distanceTelemetreDroit <= robotState.distanceTelemetreGauche) {
                PWMSpeedConsigne(speedNormal - 5, MOTEUR_DROIT);
                PWMSpeedConsigne(speedSlow, MOTEUR_GAUCHE);
            } else {
                PWMSpeedConsigne(speedSlow, MOTEUR_DROIT);
                PWMSpeedConsigne(speedNormal - 5, MOTEUR_GAUCHE);
            }
            break;
        case 5:
            PWMSpeedConsigne(speedNormal - 5, MOTEUR_DROIT);
            PWMSpeedConsigne(speedSlow, MOTEUR_GAUCHE);
            break;
        case 8:
            PWMSpeedConsigne(speedSlow, MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal - 5, MOTEUR_GAUCHE);
            break;
        case 9:
            PWMSpeedConsigne(speedTurn, MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal - 5, MOTEUR_GAUCHE);
            break;
        case 10:
            PWMSpeedConsigne(speedNormal, MOTEUR_DROIT);
            PWMSpeedConsigne(-speedNormal, MOTEUR_GAUCHE);
            break;
        case 12:
            PWMSpeedConsigne(-speedNormal, MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal, MOTEUR_GAUCHE);
            break;
        case 20:
            PWMSpeedConsigne(0.0, MOTEUR_DROIT);
            PWMSpeedConsigne(0.0, MOTEUR_GAUCHE);
            break;


    }
    //    if(robotState.distanceTelemetreExtremGauche<30 && robotState.distanceTelemetreGauche>30 && robotState.distanceTelemetreExtremDroit>30 && robotState.distanceTelemetreCentre>30){
    //        PWMSpeedConsigne(-15.0,MOTEUR_DROIT);
    //        PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
    //    }else if(robotState.distanceTelemetreGauche<30 && robotState.distanceTelemetreExtremDroit>30 && robotState.distanceTelemetreCentre>30){
    //        PWMSpeedConsigne(-5.0,MOTEUR_DROIT);
    //        PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
    //    }else if(robotState.distanceTelemetreExtremDroit<30 && robotState.distanceTelemetreDroit>30 && robotState.distanceTelemetreExtremGauche>30 && robotState.distanceTelemetreCentre>30){
    //        PWMSpeedConsigne(-20.0,MOTEUR_DROIT);
    //        PWMSpeedConsigne(15.0,MOTEUR_GAUCHE);
    //    }else if(robotState.distanceTelemetreDroit<30 && robotState.distanceTelemetreExtremGauche>30 && robotState.distanceTelemetreCentre>30){
    //        PWMSpeedConsigne(-20.0,MOTEUR_DROIT);
    //        PWMSpeedConsigne(5.0,MOTEUR_GAUCHE);
    //    }else if(robotState.distanceTelemetreExtremGauche<30 && robotState.distanceTelemetreGauche<30 && robotState.distanceTelemetreDroit<30 && robotState.distanceTelemetreExtremDroit<30){
    //        PWMSpeedConsigne(10.0,MOTEUR_DROIT);
    //        PWMSpeedConsigne(10.0,MOTEUR_GAUCHE);
    //    }else if(robotState.distanceTelemetreCentre<30){
    //        if(robotState.distanceTelemetreExtremGauche<robotState.distanceTelemetreExtremDroit || robotState.distanceTelemetreGauche<robotState.distanceTelemetreDroit){
    //            PWMSpeedConsigne(-5.0,MOTEUR_DROIT);
    //            PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
    //        }else if(robotState.distanceTelemetreExtremGauche>robotState.distanceTelemetreExtremDroit || robotState.distanceTelemetreGauche>robotState.distanceTelemetreDroit){
    //            PWMSpeedConsigne(-20.0,MOTEUR_DROIT);
    //            PWMSpeedConsigne(5.0,MOTEUR_GAUCHE);
    //        }
    //    }else{
    //        PWMSpeedConsigne(-20.0,MOTEUR_DROIT);
    //        PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
    //    }
    //    


    //    if(robotState.distanceTelemetreExtremGauche<30){
    //        PWMSpeedConsigne(5.0,MOTEUR_DROIT);
    //        PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
    //    }else{
    //        PWMSpeedConsigne(20.0,MOTEUR_DROIT);
    //        PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
    //    }

    //    int i1;
    //    int i2;
    //    int i3;
    //    if(robotState.distanceTelemetreCentre < 30){i1 = 1;}
    //    else{i1 = 0;}
    //    if(robotState.distanceTelemetreExtremGauche < 30 || robotState.distanceTelemetreGauche < 30){i2 = 1;}
    //    else{i2 = 0;}
    //    if(robotState.distanceTelemetreDroit < 30 || robotState.distanceTelemetreExtremDroit < 30){i3 = 1;}
    //    else{i3 = 0;}
    //
    //    if(i1 == 0){PWMSpeedConsigne(-20.0,MOTEUR_DROIT);}else{PWMSpeedConsigne(0.0,MOTEUR_DROIT);}
    //    if((i2 == 0 && i1 == 0 && i3 == 0) || i1 == 1){PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);}else{PWMSpeedConsigne(0.0,MOTEUR_GAUCHE);}

}*/

void avoidingObstacles() {
    if (robotState.avoidingObstaclesBool != 1) {
        return;
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
    }

    int sensorCase = 0;
    float limitS = 30.0; //44.0
    float limitSs = 45.0; //52.0

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

    float speedNormal = 23.0;
    float speedTurn = 9.0;//13.0
    float speedSlow = 5.0;//5.0

    if(sensorCase == 0b00000){sensorCase = 0;}
    if(sensorCase == 0b00001){sensorCase = 1;}
    if(sensorCase == 0b00010){sensorCase = 2;}
    if(sensorCase == 0b00011){sensorCase = 2;}
    if(sensorCase == 0b00100){sensorCase = 4;}
    if(sensorCase == 0b00101){sensorCase = 12;}
    if(sensorCase == 0b00110){sensorCase = 12;}
    if(sensorCase == 0b00111){sensorCase = 12;}
    if(sensorCase == 0b01000){sensorCase = 8;}
    if(sensorCase == 0b01001){sensorCase = 12;}
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
 
    if(STOP == 1){
        sensorCase = 20;
    }

    switch (sensorCase) {
        case 0://AVANT
            PWMSpeedConsigne(speedNormal, MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal - 0.7, MOTEUR_GAUCHE);
            break;
        case 1://-GAUCHE
            PWMSpeedConsigne(speedNormal - 5, MOTEUR_DROIT);
            PWMSpeedConsigne(speedTurn, MOTEUR_GAUCHE);
            break;
        case 2://+GAUCHE
            PWMSpeedConsigne(speedNormal - 5, MOTEUR_DROIT);
            PWMSpeedConsigne(speedSlow, MOTEUR_GAUCHE);
            break;
        case 4://MILIEU CHOIX
            if (robotState.distanceTelemetreDroit <= robotState.distanceTelemetreGauche) {
                PWMSpeedConsigne(speedNormal - 5, MOTEUR_DROIT);
                PWMSpeedConsigne(speedSlow, MOTEUR_GAUCHE);
            } else {
                PWMSpeedConsigne(speedSlow, MOTEUR_DROIT);
                PWMSpeedConsigne(speedNormal - 5, MOTEUR_GAUCHE);
            }
            break;
//        case 5:
//            PWMSpeedConsigne(speedNormal - 5, MOTEUR_DROIT);
//            PWMSpeedConsigne(speedSlow, MOTEUR_GAUCHE);
//            break;
        case 8://+DROIT
            PWMSpeedConsigne(speedSlow, MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal - 5, MOTEUR_GAUCHE);
            break;
        case 9://-DROIT
            PWMSpeedConsigne(speedTurn, MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal - 5, MOTEUR_GAUCHE);
            break;
        case 10:// 1/2 TOUR GAUCHE
            PWMSpeedConsigne(speedNormal-5, MOTEUR_DROIT);
            PWMSpeedConsigne(-speedNormal+15, MOTEUR_GAUCHE);
            break;
        case 12:// 1/2 TOUR DROIT
            PWMSpeedConsigne(-speedNormal+15, MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal-5, MOTEUR_GAUCHE);
            break;
        case 20: // ARRET
            PWMSpeedConsigne(0.0, MOTEUR_DROIT);
            PWMSpeedConsigne(0.0, MOTEUR_GAUCHE);
            break;


    }
}