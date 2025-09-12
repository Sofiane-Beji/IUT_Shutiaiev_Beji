/* 
 * File:   asservissement.h
 * Author: E306-PC3
 *
 * Created on 12 septembre 2025, 11:43
 */

#ifndef ASSERVISSEMENT_H
#define	ASSERVISSEMENT_H

typedef struct _PidCorrector
{
double Kp;
double Ki;
double Kd;
double erreurProportionelleMax;
double erreurIntegraleMax;
double erreurDeriveeMax;
double erreurIntegrale;
double epsilon_1;
double erreur;
//For Debug only
double corrP;
double corrI;
double corrD;
}PidCorrector;


#endif	/* ASSERVISSEMENT_H */

