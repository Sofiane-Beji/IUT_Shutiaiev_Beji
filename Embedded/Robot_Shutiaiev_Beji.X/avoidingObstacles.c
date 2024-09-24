#include "Robot.h"
#include "IO.h"
#include "PWM.h"
#include "ADC.h"
void avoidingObstacles(){
    if(robotState.avoidingObstaclesBool != 1){return;}

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
    float limitS = 44.0;
    float limitSs = 52.0;
    
    if(robotState.distanceTelemetreExtremDroit <= limitSs){sensorCase+=0b10000;}
    if(robotState.distanceTelemetreDroit <= limitS){sensorCase+=0b1000;}
    if(robotState.distanceTelemetreCentre <= limitS){sensorCase+=0b100;}
    if(robotState.distanceTelemetreGauche <= limitS){sensorCase+=0b10;}
    if(robotState.distanceTelemetreExtremGauche <= limitSs){sensorCase+=0b1;}
    
    float speedNormal = 23.0;
    float speedTurn = 18.0;
    float speedSlow = 5.0;
    
    if(sensorCase == 0b0000 || sensorCase == 0b10001){sensorCase = 0;}
    if(sensorCase == 0b1 || sensorCase == 0b10010){sensorCase = 1;}
    if(sensorCase == 0b10 || sensorCase == 0b11 || sensorCase == 0b10011){sensorCase = 2;}
    if(sensorCase == 0b100 || sensorCase == 0b01110){sensorCase = 4;}
    if(sensorCase >= 0b101 && sensorCase <= 0b111){sensorCase = 5;}
    if(sensorCase == 0b1000 || sensorCase == 0b11000 || sensorCase == 0b11001){sensorCase = 8;}
    if(sensorCase == 0b1001 || sensorCase == 0b10000){sensorCase = 9;}
    if(sensorCase == 0b1010 || sensorCase == 0b1011 || sensorCase == 0b1101 || sensorCase == 0b1111){sensorCase = 10;}
    if(sensorCase == 0b1100 || (sensorCase >= 0b10100 && sensorCase <= 0b10111) || (sensorCase >= 0b11010 && sensorCase <= 0b11111)){sensorCase = 12;}
    
    switch(sensorCase)
    {
        case 0:
            PWMSpeedConsigne(speedNormal,MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal - 0.7,MOTEUR_GAUCHE);
            break;
        case 1:
            PWMSpeedConsigne(speedNormal,MOTEUR_DROIT);
            PWMSpeedConsigne(speedTurn,MOTEUR_GAUCHE);
            break;
        case 2:
            PWMSpeedConsigne(speedNormal,MOTEUR_DROIT);
            PWMSpeedConsigne(speedSlow,MOTEUR_GAUCHE);
            break;
        case 4:
            if(robotState.distanceTelemetreDroit <= robotState.distanceTelemetreGauche)
            {
                PWMSpeedConsigne(speedNormal,MOTEUR_DROIT);
                PWMSpeedConsigne(speedSlow,MOTEUR_GAUCHE);
            }
            else
            {
                PWMSpeedConsigne(speedSlow,MOTEUR_DROIT);
                PWMSpeedConsigne(speedNormal,MOTEUR_GAUCHE);
            }
            break;
        case 5:
            PWMSpeedConsigne(speedNormal,MOTEUR_DROIT);
            PWMSpeedConsigne(speedSlow,MOTEUR_GAUCHE);
            break;
        case 8:
            PWMSpeedConsigne(speedSlow,MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal,MOTEUR_GAUCHE);
            break;
        case 9:
            PWMSpeedConsigne(speedTurn,MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal,MOTEUR_GAUCHE);
            break;
        case 10:
            PWMSpeedConsigne(speedNormal,MOTEUR_DROIT);
            PWMSpeedConsigne(-speedNormal,MOTEUR_GAUCHE);
            break;
        case 12:
            PWMSpeedConsigne(-speedNormal,MOTEUR_DROIT);
            PWMSpeedConsigne(speedNormal,MOTEUR_GAUCHE);
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
}