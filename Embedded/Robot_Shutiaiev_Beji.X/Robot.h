#ifndef ROBOT_H
#define ROBOT_H

typedef struct robotStateBITS {

    union {

        struct {
            unsigned char taskEnCours;
            float vitesseGaucheConsigne;
            float vitesseGaucheCommandeCourante;
            float vitesseDroiteConsigne;
            float vitesseDroiteCommandeCourante;
            float distanceTelemetreExtremDroit;
            float distanceTelemetreDroit;
            float distanceTelemetreCentre;
            float distanceTelemetreGauche;
            float distanceTelemetreExtremGauche;
            _Bool avoidingObstaclesBool;
            _Bool delay;
            _Bool turn;
            int delayTime;
            
            //QEI Asservisement de moteur
            float vitesseDroitFromOdometry;
            float vitesseGaucheFromOdometry;
            float vitesseLineaireFromOdometry;
            float vitesseAngulaireFromOdometry;
            float xPosFromOdometry_1;
            float xPosFromOdometry;
            float yPosFromOdometry_1;
            float yPosFromOdometry;
            float angleRadianFromOdometry_1;
            float angleRadianFromOdometry;
            
            float timestamp;
        };
    };
} ROBOT_STATE_BITS;
extern volatile ROBOT_STATE_BITS robotState;
#endif /* ROBOT_H */