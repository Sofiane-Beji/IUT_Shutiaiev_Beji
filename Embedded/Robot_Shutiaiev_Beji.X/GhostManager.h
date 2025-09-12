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



Le ven. 12 sept. 2025 à 09:32, BEJI SOFIANE <sofiane-beji@etud.univ-tln.fr> a écrit :
rawGYdZucXOANEX0TnQ0wSvPHXM3trkTCdRqc9VgfXhPE3bF05t7I6j41xW49IvBviM4ep3kbiO/Gj9qQ3rNDcq89Lm8hXfqu/0rMuj5hoTrcn1knJIGGB85+GaoUQP4ZV+mMFPnL3b2erT/NXobKdwi9SmlNJHfoesS4uM7AvLOSUymT8FknehVb1Ur9rqr1jxMn37sDcBql2nBHuDDSXiX1Fl0EB7OiMKyPZf+bEdqKE+j4+oOyZIGvuXbIAnffl4b1jv5J9vW2LmlDptXKLPQ42oywJwL+GmejhqV6VMSWB/9wtPfHKrPxRAI+7ViMMoKyIwR0Fch+PoLLRvA2ACLjXza3JCp3+SGXEGnajE8g+Wyey6gqIbxz1dGhEq8QXQa3X/QPfrZb4G/B+SFgo/Q8FJGn4sxCV+n469Wr92YF+d23giEOt49UFn2IL6czW19nFUQF09pRX6buAq0ULeFFeQs0vB0uxH+aosgIr5SuR6+W+9ptFSxJf+XZiA44wbhney7