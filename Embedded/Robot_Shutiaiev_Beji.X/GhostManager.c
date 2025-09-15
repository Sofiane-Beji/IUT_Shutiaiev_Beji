#include "GhostManager.h"
#include "Utilities.h"
#include "Robot.h"
#include "math.h"
#include "PWM.h"
#include "UART_Protocol.h"
#include "timer.h"
#include "ChipConfig.h"
#include "QEI.h"

volatile GhostPosition ghostPosition;
static unsigned long lastUpdateTime = 0;

int current_state = IDLE;
int index = 0;

#define MAX_POS 7

struct Waypoint {
    double x;
    double y;
    int last_rotate;
};
typedef struct Waypoint Waypoint_t;

void InitTrajectoryGenerator(void) {
    ghostPosition.x = 1.33;
    ghostPosition.y = 0.0;
    ghostPosition.theta = -PI;
    ghostPosition.linearSpeed = 0.0;
    ghostPosition.angularSpeed = 0.0;
    ghostPosition.targetX = 0.0;
    ghostPosition.targetY = 0.0;
    ghostPosition.angleToTarget = 0.0;
    ghostPosition.distanceToTarget = 0.0;
}

Waypoint_t waypoints[MAX_POS] = {{0, 0, 0}, {0, 0.5, 0}, {-1, 0.5, 0}, {-1, -0.5, 0}, {0, -0.5, 0}, {0, 0, 0}, {1.3, 0, 1}};


void UpdateTrajectory() // Mise a jour de la trajectoire en fonction de l'etat actuel par rapport au waypoint
{
    // Target -> le waypoint : c'est où on veut aller
    double thetaTarget = atan2(ghostPosition.targetY - ghostPosition.y, ghostPosition.targetX - ghostPosition.x);
    // Theta entre le robot et où on veut aller
    double thetaRestant = moduloByAngle(ghostPosition.theta, thetaTarget) - ghostPosition.theta;
    ghostPosition.angleToTarget = thetaRestant;
    // Theta à partir duquel on considère que c'est good
    double thetaArret = ghostPosition.angularSpeed * ghostPosition.angularSpeed / (2 * accelerationAngulaire);
    // Pas d'angle à ajouter
    double incrementAng = ghostPosition.angularSpeed / FREQ_ECH_QEI;
    double incremntLin = ghostPosition.linearSpeed / FREQ_ECH_QEI;

    double distanceArret = ghostPosition.linearSpeed * ghostPosition.linearSpeed / (2 * accelerationLineaire);

    double distanceRestante = sqrt((ghostPosition.targetX - ghostPosition.x) * (ghostPosition.targetX - ghostPosition.x)
            + (ghostPosition.targetY - ghostPosition.y) * (ghostPosition.targetY - ghostPosition.y));
    ghostPosition.distanceToTarget = distanceRestante;

    /* ################## IDLE ################## */
    if (current_state == IDLE) {
        if(index < MAX_POS) {
            Waypoint_t nextWay = waypoints[index++];
            ghostPosition.targetX = nextWay.x;
            ghostPosition.targetY = nextWay.y;
            current_state = (nextWay.last_rotate ? LASTROTATE : ROTATING);
        }
       
    /* ################## ROTATIONING ################## */
    } else if (current_state == ROTATING || current_state == LASTROTATE) {

        if (ghostPosition.angularSpeed < 0) thetaArret = -thetaArret;

        if (((thetaArret >= 0 && thetaRestant >= 0) || (thetaArret <= 0 && thetaRestant <= 0)) && (Abs(thetaRestant) >= Abs(thetaArret))) {
            // on accélère en rampe saturée
            if (thetaRestant > 0) {
                ghostPosition.angularSpeed = Min(ghostPosition.angularSpeed + accelerationAngulaire / FREQ_ECH_QEI, MAX_ANGULAR_SPEED);
            } else if (thetaRestant < 0) {
                ghostPosition.angularSpeed = Max(ghostPosition.angularSpeed - accelerationAngulaire / FREQ_ECH_QEI, -MAX_ANGULAR_SPEED);
            }
            //        else {
            //            ghostPosition.angularSpeed = 0;
            //        }

        } else {
            //on freine en rampe saturée
            if (thetaRestant >= 0 && ghostPosition.angularSpeed > 0) {
                ghostPosition.angularSpeed = Max(ghostPosition.angularSpeed - accelerationAngulaire / FREQ_ECH_QEI, 0);
            } else if (thetaRestant >= 0 && ghostPosition.angularSpeed < 0) {
                ghostPosition.angularSpeed = Min(ghostPosition.angularSpeed + accelerationAngulaire / FREQ_ECH_QEI, 0);
            } else if (thetaRestant <= 0 && ghostPosition.angularSpeed > 0) {
                ghostPosition.angularSpeed = Max(ghostPosition.angularSpeed - accelerationAngulaire / FREQ_ECH_QEI, 0);
            } else if (thetaRestant <= 0 && ghostPosition.angularSpeed < 0) {
                ghostPosition.angularSpeed = Min(ghostPosition.angularSpeed + accelerationAngulaire / FREQ_ECH_QEI, 0);
            }

            if (Abs(thetaRestant) < Abs(incrementAng)) {
                incrementAng = thetaRestant;
            }
        }

        ghostPosition.theta += incrementAng;
        robotState.consigneVitesseAngulaire = ghostPosition.angularSpeed;

        if (ghostPosition.angularSpeed == 0 && (Abs(thetaRestant) < 0.01)) {
            ghostPosition.theta = thetaTarget;
            if(current_state != LASTROTATE)
                current_state = ADVANCING;
            else
                current_state = IDLE;
        }

       
    /* ################## AVANCING ################## */
    } else if (current_state == ADVANCING) {

        if ((distanceRestante != 0) && (Modulo2PIAngleRadian(thetaRestant) < 0.01)) {
            if (((distanceArret >= 0 && distanceRestante >= 0) || (distanceArret <= 0 && distanceRestante <= 0)) && Abs(distanceRestante) >= Abs(distanceArret)) {
                if (distanceRestante > 0) {
                    ghostPosition.linearSpeed = Min(ghostPosition.linearSpeed + accelerationLineaire / FREQ_ECH_QEI, MAX_LINEAR_SPEED);
                } else if (distanceRestante < 0) {
                    ghostPosition.linearSpeed = Max(ghostPosition.linearSpeed - accelerationLineaire / FREQ_ECH_QEI, -MAX_LINEAR_SPEED);
                }
            } else {

                if (distanceRestante >= 0 && ghostPosition.linearSpeed > 0) {
                    ghostPosition.linearSpeed = Max(ghostPosition.linearSpeed - accelerationLineaire / FREQ_ECH_QEI, 0);
                } else if (distanceRestante >= 0 && ghostPosition.linearSpeed < 0) {
                    ghostPosition.linearSpeed = Min(ghostPosition.linearSpeed + accelerationLineaire / FREQ_ECH_QEI, 0);
                } else if (distanceRestante <= 0 && ghostPosition.linearSpeed > 0) {
                    ghostPosition.linearSpeed = Max(ghostPosition.linearSpeed - accelerationLineaire / FREQ_ECH_QEI, 0);
                } else if (distanceRestante <= 0 && ghostPosition.linearSpeed < 0) {
                    ghostPosition.linearSpeed = Min(ghostPosition.linearSpeed + accelerationLineaire / FREQ_ECH_QEI, 0);
                }

                if (Abs(distanceRestante) < Abs(incremntLin)) {
                    incremntLin = distanceRestante;
                }
            }


        }

        if ((Abs(distanceRestante) < 0.0001)) {
            ghostPosition.linearSpeed = 0;
            ghostPosition.x = ghostPosition.targetX;
            ghostPosition.y = ghostPosition.targetY;
            current_state = IDLE;
        }
       
        ghostPosition.x += incremntLin * cos(ghostPosition.theta);
        ghostPosition.y += incremntLin * sin(ghostPosition.theta);
        robotState.consigneVitesseLineaire = ghostPosition.linearSpeed;
    }
    SendGhostData();
}



//void rotationTarget(double currentTime) {
//    double deltaTime = (timestamp - lastUpdateTime) / 1000.0;
//    double angleToTarget = atan2(ghostPosition.targetY - ghostPosition.y, ghostPosition.targetX - ghostPosition.x);
//    double angleDifference = moduloByAngle(ghostPosition.theta, angleToTarget - ghostPosition.theta);
//    double thetaArret = pow(ghostPosition.angularSpeed, 2) / (2 * MAX_ANGULAR_ACCEL);
//    
//    if(ghostPosition.angularSpeed < 0)
//    {
//        thetaArret = -thetaArret;
//    }
//
//    if (((thetaArret >= 0 && angleDifference >= 0) || (thetaArret <=0 && angleDifference<=0)) && fabs(angleDifference) >= fabs(thetaArret)) {
//        if(angleDifference>0)
//        {
//            //Si la destination est devant, on accélère en positif en saturant la vitesse à Vmax
//            ghostPosition.angularSpeed += MAX_ANGULAR_ACCEL * deltaTime;
//        }
//        else if(angleDifference<0)
//        {
//            //Si la destination est derrière, on accélère en négatif en saturant la vitesse à -Vmax
//            ghostPosition.angularSpeed += -MAX_ANGULAR_ACCEL * deltaTime;
//        }
//        else
//        {
//            //Sinon, on met la vitesse à 0
//            ghostPosition.angularSpeed = 0;
//        }
//    }
//    else {
//        // On freine en rampe saturée
//        if(angleDifference > 0)
//        {
//            //Si la destination est devant, on freine en positif en saturant la vitesse
//            ghostPosition.angularSpeed -= MAX_ANGULAR_ACCEL * deltaTime;
//        }
//        else if(angleDifference < 0)
//        {
//            //Si la destination est derrière, on freine en négatif en saturant la vitesse
//            ghostPosition.angularSpeed -= -MAX_ANGULAR_ACCEL * deltaTime;
//        }
//        else
//        {
//            //Sinon, on met la vitesse à 0
//            ghostPosition.angularSpeed = 0;
//        }
//    }
//
//    if (ghostPosition.angularSpeed > MAX_ANGULAR_SPEED) ghostPosition.angularSpeed = MAX_ANGULAR_SPEED;
//
//    ghostPosition.angularSpeed = fmin(fmaghostPosition.angularSpeed, -MAX_ANGULAR_SPEED), MAX_ANGULAR_SPEED);
//    ghostPosition.theta += ghostPosition.angularSpeed * deltaTime;
//    ghostPosition.theta = moduloByAngle(0, ghostPosition.theta);
//
//    ghostPosition.angleToTarget = angleDifference;
//}

//void rotationTarget(double currentTime) {
//    double deltaTime = (timestamp - lastUpdateTime) / 1000.0;
//    double angleToTarget = atan2(ghostPosition.targetY - ghostPosition.y, ghostPosition.targetX - ghostPosition.x);
//    double angleDifference = moduloByAngle(ghostPosition.theta, angleToTarget - ghostPosition.theta);
//    double thetaArret = pow(ghostPosition.angularSpeed, 2) / (2 * MAX_ANGULAR_ACCEL);
//
//    if (fabs(angleDifference) < ANGLE_TOLERANCE) {
//        ghostPosition.state = ADVANCING;
//        ghostPosition.angularSpeed = 0;
//    }
//
//    else {
//        // Soit thetaRest > 0 (rota positive) && accel
//        if (angleDifference > 0 && ghostPosition.angularSpeed <= MAX_ANGULAR_SPEED && angleDifference > thetaArret) {
//            // accel
//            ghostPosition.angularSpeed += MAX_ANGULAR_ACCEL * deltaTime;
//
//        }
//
//        // Soit thetaRest > 0 (rota positive) && deccel
//        if (angleDifference > 0 && ghostPosition.angularSpeed <= MAX_ANGULAR_SPEED && angleDifference < thetaArret) {
//            // decel
//            ghostPosition.angularSpeed += -MAX_ANGULAR_ACCEL * deltaTime;
//            
//        }
//
//        // Soit thetaRest < 0 (rota negative) && accel
//        if (angleDifference < 0 && ghostPosition.angularSpeed >= -MAX_ANGULAR_SPEED && angleDifference > thetaArret) {
//            // accel
//            ghostPosition.angularSpeed -= MAX_ANGULAR_ACCEL * deltaTime;
//        }
//
//        // Soit thetaRest < 0 (rota negative) && deccel
//        if (angleDifference < 0 && ghostPosition.angularSpeed >= -MAX_ANGULAR_SPEED && angleDifference < thetaArret) {
//            // decel
//            ghostPosition.angularSpeed -= -MAX_ANGULAR_ACCEL * deltaTime;
//        }
//    }
//
//    if (ghostPosition.angularSpeed > MAX_ANGULAR_SPEED) ghostPosition.angularSpeed = MAX_ANGULAR_SPEED;
//
//    ghostPosition.angularSpeed = fmin(fmaghostPosition.angularSpeed, -MAX_ANGULAR_SPEED), MAX_ANGULAR_SPEED);
//    ghostPosition.theta += ghostPosition.angularSpeed * deltaTime;
//    ghostPosition.theta = moduloByAngle(0, ghostPosition.theta);
//
//    ghostPosition.angleToTarget = angleDifference;
//}
//void PIDPosition(){
//    ghostPosition.erreurAngulaire = ghostPosition.thetaGhost - robotState.angleRadianFromOdometry ;
//    
//}

void SendGhostData() {
    unsigned char positionPayload[24];
    getBytesFromInt32(positionPayload, 0, robotState.timestamp);
    getBytesFromFloat(positionPayload, 4, (float) (ghostPosition.angularSpeed));
    getBytesFromFloat(positionPayload, 8, (float) (ghostPosition.linearSpeed));
    getBytesFromFloat(positionPayload, 12, (float) (ghostPosition.theta));
    getBytesFromFloat(positionPayload, 16, (float) (ghostPosition.x));
    getBytesFromFloat(positionPayload, 20, (float) (ghostPosition.y));
    UartEncodeAndSendMessage(GHOST_DATA, 24, positionPayload);
}
