#include <xc.h>
#include "timer.h"
#include "IO.h"
#include "PWM.h"
#include "Robot.h"
#include "ADC.h"
//Initialisation d?un timer 16 bits

unsigned char toggle = 0;

void InitTimer1(void) {
    //Timer1 pour horodater les mesures (1ms)
    T1CONbits.TON = 0; // Disable Timer
    T1CONbits.TCKPS = 0b00; //Prescaler
    //11 = 1:256 prescale value
    //10 = 1:64 prescale value
    //01 = 1:8 prescale value
    //00 = 1:1 prescale value
    T1CONbits.TCS = 0; //clock source = internal clock
    PR1 = 4096;
    IFS0bits.T1IF = 0; // Clear Timer Interrupt Flag
    IEC0bits.T1IE = 1; // Enable Timer interrupt
    T1CONbits.TON = 1; // Enable Timer
}
//Interruption du timer 1

/*void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;
    LED_BLANCHE_1 = !LED_BLANCHE_1;
}*/

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0;
    PWMUpdateSpeed();
    //LED_BLANCHE_1 = 1;
    //LED_ORANGE_1 = !LED_ORANGE_1;
    ADC1StartConversionSequence();
    avoidingObstacles();
}

//Initialisation d?un timer 32 bits

void InitTimer23(void) {
    T3CONbits.TON = 0; // Stop any 16-bit Timer3 operation
    T2CONbits.TON = 0; // Stop any 16/32-bit Timer3 operation
    T2CONbits.T32 = 1; // Enable 32-bit Timer mode
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TCKPS = 0b10; // Select 1:1 Prescaler
    TMR3 = 0x00; // Clear 32-bit Timer (msw)
    TMR2 = 0x00; // Clear 32-bit Timer (lsw)
    PR3 = 14; // Load 32-bit period value (msw)
    PR2 = 19996; // Load 32-bit period value (lsw)
    IPC2bits.T3IP = 0x01; // Set Timer3 Interrupt Priority Level
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
    IEC0bits.T3IE = 1; // Enable Timer3 interrupt
    T2CONbits.TON = 1; // Start 32-bit Timer
}
//Interruption du timer 32 bits sur 2-3
//
//void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
//    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
//    LED_ORANGE_1 = !LED_ORANGE_1;
//}

//Interruption du timer 32 bits sur 2-3

/*void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
    if (toggle == 0) {
        PWMSetSpeed(20, MOTEUR_DROIT);
        PWMSetSpeed(20, MOTEUR_GAUCHE);
        toggle = 1;
    } else {
        PWMSetSpeed(-20, MOTEUR_DROIT);
        PWMSetSpeed(-20, MOTEUR_GAUCHE);
        toggle = 0;
    }
}*/

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
//    if (toggle == 0) {
////        robotState.vitesseDroiteConsigne = 10.00;
//        PWMSpeedConsigne(10.0,MOTEUR_DROIT);
//        
//        toggle = 1;
//    } else {
////        robotState.vitesseDroiteConsigne = 30.00;
//        PWMSpeedConsigne(30.0,MOTEUR_DROIT);
//        toggle = 0;
//    }
    
}

/*void avoidingObstacles()
    {
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
        
        float distanceLimit = 80; 
        float distance[5];
        distance[0] = robotState.distanceTelemetreExtremGauche;
        distance[1] = robotState.distanceTelemetreGauche;
        distance[2] = robotState.distanceTelemetreCentre;
        distance[3] = robotState.distanceTelemetreDroit;
        distance[4] = robotState.distanceTelemetreExtremDroit;
        
        for(int i = 0; i <= 4; i++)
        {
            if(distance[i] > distanceLimit){distance[i] = 80;}
        }
        
        float min = distance[0];
        int minIndex = 0;
        for(int i = 0; i < 4; i++)
        {
            if(distance[i] < min){ min = distance[i]; minIndex = i;}
        }
        if(minIndex = 0)
        {
            LED_BLANCHE_2 = 1;
        }
        else{
            LED_BLANCHE_2 = 0;
        }
        if(minIndex = 1)
        {
            LED_BLEUE_2 = 1;
        }
        else{
            LED_BLEUE_2 = 0;
        }
        if(minIndex = 2)
        {
            //fast forw
            PWMSpeedConsigne(20.0,MOTEUR_DROIT);
            PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
        }
        else if(minIndex > 2)
        {
            //left
            PWMSpeedConsigne(20.0,MOTEUR_DROIT);
            PWMSpeedConsigne(5.0,MOTEUR_GAUCHE);
        }
        else if(minIndex < 2)
        {
            PWMSpeedConsigne(5.0,MOTEUR_DROIT);
            PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
            //right
        }
    }*/


void avoidingObstacles(){
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
    
    if(robotState.distanceTelemetreExtremGauche<30 && robotState.distanceTelemetreGauche>30 && robotState.distanceTelemetreExtremDroit>30 && robotState.distanceTelemetreCentre>30){
        PWMSpeedConsigne(-15.0,MOTEUR_DROIT);
        PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
    }else if(robotState.distanceTelemetreGauche<30 && robotState.distanceTelemetreExtremDroit>30 && robotState.distanceTelemetreCentre>30){
        PWMSpeedConsigne(-5.0,MOTEUR_DROIT);
        PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
    }else if(robotState.distanceTelemetreExtremDroit<30 && robotState.distanceTelemetreDroit>30 && robotState.distanceTelemetreExtremGauche>30 && robotState.distanceTelemetreCentre>30){
        PWMSpeedConsigne(-20.0,MOTEUR_DROIT);
        PWMSpeedConsigne(15.0,MOTEUR_GAUCHE);
    }else if(robotState.distanceTelemetreDroit<30 && robotState.distanceTelemetreExtremGauche>30 && robotState.distanceTelemetreCentre>30){
        PWMSpeedConsigne(-20.0,MOTEUR_DROIT);
        PWMSpeedConsigne(5.0,MOTEUR_GAUCHE);
    }else if(robotState.distanceTelemetreExtremGauche<30 && robotState.distanceTelemetreGauche<30 && robotState.distanceTelemetreDroit<30 && robotState.distanceTelemetreExtremDroit<30){
        PWMSpeedConsigne(10.0,MOTEUR_DROIT);
        PWMSpeedConsigne(10.0,MOTEUR_GAUCHE);
    }else if(robotState.distanceTelemetreCentre<30){
        if(robotState.distanceTelemetreExtremGauche<robotState.distanceTelemetreExtremDroit || robotState.distanceTelemetreGauche<robotState.distanceTelemetreDroit){
            PWMSpeedConsigne(-5.0,MOTEUR_DROIT);
            PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
        }else if(robotState.distanceTelemetreExtremGauche>robotState.distanceTelemetreExtremDroit || robotState.distanceTelemetreGauche>robotState.distanceTelemetreDroit){
            PWMSpeedConsigne(-20.0,MOTEUR_DROIT);
            PWMSpeedConsigne(5.0,MOTEUR_GAUCHE);
        }
    }else{
        PWMSpeedConsigne(-20.0,MOTEUR_DROIT);
        PWMSpeedConsigne(20.0,MOTEUR_GAUCHE);
    }
    
    
    
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