/*
 * File:   GhostManager.h
 * Author: TP-EO-5
 *
 * Created on 10 avril 2024, 12:09
 */

#ifndef GHOSTMANAGER_H
#define GHOSTMANAGER_H


#define GHOST_DATA 0x0010

// Parametres de trajectoire
#define MAX_LINEAR_SPEED 1 // m/s
//#define MAX_LINEAR_ACCEL 0.2 // m/s^2

#define MAX_ANGULAR_SPEED 1 * M_PI * 1.0 // rad/s
//#define MAX_ANGULAR_ACCEL 1 * M_PI * 1.0 // rad/s^2

//#define ANGLE_TOLERANCE 0.05 // radians
//#define DISTANCE_TOLERANCE 0.1 // metres

//#define VitesseMaxAngulaire 0.2
//#define ToleranceAngulaire 0.005
#define accelerationAngulaire 0.9
//#define VitesseMaxLineaire 1 // 1m.s-1
//#define Tolerancedistance 0.1 // 10 cm
#define accelerationLineaire 0.9



// Etat de controle de la trajectoire
typedef enum {
    IDLE,
    ROTATING,
    ADVANCING,
    LASTROTATE
} TrajectoryState;

// Position et vitesse du Ghost
typedef struct {
    TrajectoryState state;
    //EtatTraj state;
   
    double x;
    double y;
    double theta;
    double linearSpeed;
    double angularSpeed;
    double targetX;
    double targetY;
    double angleToTarget;
    double distanceToTarget;  
} GhostPosition;


extern volatile GhostPosition ghostPosition;
void UpdateTrajectory();
void SendGhostData();
void InitTrajectoryGenerator(void);
void rotationTarget(double currentTime);

void InitTrajectory(void);
void UpdateTrajectory();
void SendGhostInfo();
void PIDPosition();
#endif /* GHOSTMANAGER_H */
