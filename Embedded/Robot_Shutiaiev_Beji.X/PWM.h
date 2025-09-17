/* 
 * File:   PWM.h
 * Author: TP_EO_6
 *
 * Created on 11 septembre 2024, 15:05
 */

#ifndef PWM_H
#define	PWM_H



#ifdef	__cplusplus
extern "C" {
#endif

void InitPWM(void);
void PWMSpeedConsigne(float vitesseEnPourcents, char moteur);
void PWMUpdateSpeed();
void PWMSpeedMS(float vitesseEnMS, char moteur);
//void PWMSetSpeed(float vitesseEnPourcents, _Bool moteur);


#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

#define MOTEUR_DROIT 1
#define MOTEUR_GAUCHE 2