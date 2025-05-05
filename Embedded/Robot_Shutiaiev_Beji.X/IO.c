/*
 * File:   IO.c
 */

#include <xc.h>
#include "IO.h"

void InitIO()
{
    //****************************************************************************************************/
    // Declaration des pin Analogiques
    //****************************************************************************************************/
    ANSELA=0;             //Desactivation de toutes entree analogique
    ANSELB=0;             //Desactivation de toutes entree analogique
    ANSELD=0;             //Desactivation de toutes entree analogique
    ANSELC=0;             //Desactivation de toutes entree analogique
    ANSELE=0;             //Desactivation de toutes entree analogique
    ANSELG=0;             //Desactivation de toutes entree analogique

    // Configuration des sorties

    //******* LED ***************************
    _TRISJ6 = 0;  _TRISK15 = 0;// LED Orange
    _TRISJ5 = 0; _TRISA0 = 0;//LED Blanche
    _TRISJ4 = 0; _TRISA9 = 0;// LED Bleue
    _TRISJ11 = 0; _TRISA10 = 0;// LED Rouge
    _TRISH10 = 0; _TRISH3 = 0;// LED Verte 
    
    
    
    
    
    
    //****** Moteurs ************************

    // Configuration des entr�es
    

    /****************************************************************************************************/
    // Gestion des pin remappables
    /****************************************************************************************************/
    UnlockIO(); // On unlock les registres d'entr�es/sorties, ainsi que les registres des PPS
    
    //Assignation des remappable pins
    _U2RXR = 18; //Remappe la RP... sur l?�entre Rx1
    _RP98R = 0b00011; //Remappe la sortie Tx1 vers RP...
    
    //******************** QEI *****************
    _QEA2R = 97; //assign QEI A to pin RP97
    _QEB2R = 113; //assign QEI B to pin RP96
    _QEA1R = 124; //assign QEI A to pin RP70
    _QEB1R = 126; //assign QEI B to pin RP69
    
    LockIO(); // On lock les registres d'entr�es/sorties, ainsi que les registres des PPS
}


void LockIO() {
    asm volatile ("mov #OSCCON,w1 \n"
                "mov #0x46, w2 \n"
                "mov #0x57, w3 \n"
                "mov.b w2,[w1] \n"
                "mov.b w3,[w1] \n"
                "bset OSCCON, #6":: : "w1", "w2", "w3");
    
}

void UnlockIO() {
    asm volatile ("mov #OSCCON,w1 \n"
                "mov #0x46, w2 \n"
                "mov #0x57, w3 \n"
                "mov.b w2,[w1] \n"
                "mov.b w3,[w1] \n"
                "bclr OSCCON, #6":: : "w1", "w2", "w3");
}
