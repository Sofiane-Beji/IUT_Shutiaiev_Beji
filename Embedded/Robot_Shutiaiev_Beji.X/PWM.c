#include <xc.h>
#include "IO.h"
#include "PWM.h"
#define PWMPER 24.0

void InitPWM(void) {
    PTCON2bits.PCLKDIV = 0b000; //Divide by 1
    PTPER = 100 * PWMPER; //�Priode en pourcentage
    //�Rglage PWM moteur 1 sur hacheur 1
    IOCON1bits.PMOD = 0b11; //PWM I/O pin pair is in the True Independent Output mode
    IOCON1bits.PENL = 1;
    IOCON1bits.PENH = 1;
    FCLCON1 = 0x0003; //�Dsactive la gestion des faults
    IOCON2bits.PMOD = 0b11; //PWM I/O pin pair is in the True Independent Output mode
    IOCON2bits.PENL = 1;
    IOCON2bits.PENH = 1;
    FCLCON2 = 0x0003; //�Dsactive la gestion des faults

    IOCON3bits.PMOD = 0b11; //PWM I/O pin pair is in the True Independent Output mode
    IOCON3bits.PENL = 1;
    IOCON3bits.PENH = 1;
    FCLCON3 = 0x0003; //�Dsactive la gestion des faults
    IOCON4bits.PMOD = 0b11; //PWM I/O pin pair is in the True Independent Output mode
    IOCON4bits.PENL = 1;
    IOCON4bits.PENH = 1;
    FCLCON4 = 0x0003; //�Dsactive la gestion des faults

    /* Enable PWM Module */
    PTCONbits.PTEN = 1;
}
double talon = 20;

void PWMSetSpeed(float vitesseEnPourcents, _Bool moteur) {
    if (moteur == 0) {//moteur droit
        if (vitesseEnPourcents > 0) {
            PDC1 = vitesseEnPourcents * PWMPER + talon;
            SDC1 = talon;
        } else {
            SDC1 = -vitesseEnPourcents * PWMPER + talon;
            PDC1 = talon;
        }
    } else if (moteur == 1) {//moteur gauche
        if (vitesseEnPourcents > 0) {
            PDC2 = vitesseEnPourcents * PWMPER + talon;
            SDC2 = talon;
        } else {
            SDC2 = -vitesseEnPourcents * PWMPER + talon;
            PDC2 = talon;
        }
    }
}