#define DISTROUES 0.21


#include "Robot.h"
#include "Toolbox.h"
#include "main.h"
#include "math.h"
#include "Utilities.h"
#include <xc.h>
#include "IO.h"


float QeiDroitPosition_T_1 = 0.0;
float QeiDroitPosition = 0.0;
float QeiGauchePosition_T_1 = 0.0;
float QeiGauchePosition = 0.0;



void InitQEI1() {
    QEI1IOCbits.SWPAB = 0; //QEAx and QEBx are swapped QEI1IOCbits 
    QEI1GECL = 0xFFFF;
    QEI1GECH = 0xFFFF;
    QEI1CONbits.QEIEN = 1; // Enable QEI Module
}

void InitQEI2() {
    QEI2IOCbits.SWPAB = 1; //QEAx and QEBx are not swapped
    QEI2GECL = 0xFFFF;
    QEI2GECH = 0xFFFF;
    QEI2CONbits.QEIEN = 1; // Enable QEI Module
}

void QEIUpdateData() {
    //On sauvegarde les anciennes valeurs
    QeiDroitPosition_T_1 = QeiDroitPosition;
    QeiGauchePosition_T_1 = QeiGauchePosition;
    //On actualise les valeurs des positions
    long QEI1RawValue = POS1CNTL;
    QEI1RawValue += ((long)POS1HLD << 16);
    long QEI2RawValue = POS2CNTL;
    QEI2RawValue += ((long)POS2HLD << 16);
    //Conversion en mm (regle pour la taille des roues codeuses)
    QeiDroitPosition = 0.00001620 * (float)QEI1RawValue;
    QeiGauchePosition = -0.00001620 * (float) QEI2RawValue;
    //Calcul des deltas de position
    float delta_d = QeiDroitPosition - QeiDroitPosition_T_1;
    float delta_g = QeiGauchePosition - QeiGauchePosition_T_1;
    //Calcul des vitesses
    //attention a remultiplier par la frequence d echantillonnage
    robotState.vitesseDroitFromOdometry = delta_d*FREQ_ECH_QEI;
    robotState.vitesseGaucheFromOdometry = delta_g*FREQ_ECH_QEI;
    robotState.vitesseLineaireFromOdometry = (robotState.vitesseDroitFromOdometry + robotState.vitesseGaucheFromOdometry) / 2.0;
    robotState.vitesseAngulaireFromOdometry = ((float)robotState.vitesseDroitFromOdometry - (float)robotState.vitesseGaucheFromOdometry) / DISTROUES;
    //Mise a jour du positionnement terrain a t-1
    robotState.xPosFromOdometry_1 = robotState.xPosFromOdometry;
    robotState.yPosFromOdometry_1 = robotState.yPosFromOdometry;
    robotState.angleRadianFromOdometry_1 = robotState.angleRadianFromOdometry;
    //Calcul des positions dans le referentiel du terrain

    robotState.xPosFromOdometry = robotState.xPosFromOdometry + robotState.vitesseLineaireFromOdometry * cos(robotState.angleRadianFromOdometry) * (1.0 / (float)FREQ_ECH_QEI);
    robotState.yPosFromOdometry = robotState.yPosFromOdometry + robotState.vitesseLineaireFromOdometry * sin(robotState.angleRadianFromOdometry) * (1.0 / (float)FREQ_ECH_QEI);
    robotState.angleRadianFromOdometry += ((float)robotState.vitesseAngulaireFromOdometry * (1.0 / (float)FREQ_ECH_QEI));
    if (robotState.angleRadianFromOdometry > PI)
        robotState.angleRadianFromOdometry -= 2 * PI;
    if (robotState.angleRadianFromOdometry < -PI)
        robotState.angleRadianFromOdometry += 2 * PI;
    
}

#define POSITION_DATA 0x0061

void SendPositionData() {
    unsigned char positionPayload[24];
    getBytesFromFloat(positionPayload, 0, (float) robotState.timestamp);
    getBytesFromFloat(positionPayload, 4, (float) (robotState.xPosFromOdometry));
    getBytesFromFloat(positionPayload, 8, (float) (robotState.yPosFromOdometry));
    getBytesFromFloat(positionPayload, 12, (float) (robotState.angleRadianFromOdometry));
    getBytesFromFloat(positionPayload, 16, (float) (robotState.vitesseLineaireFromOdometry));
    getBytesFromFloat(positionPayload, 20, (float) (robotState.vitesseAngulaireFromOdometry));
    UartEncodeAndSendMessage(POSITION_DATA, 24, positionPayload);
}