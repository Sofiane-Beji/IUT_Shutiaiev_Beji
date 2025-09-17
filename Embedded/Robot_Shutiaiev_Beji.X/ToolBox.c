#include "Toolbox.h"
#include <math.h>

float Abs(float value) {
    if (value >= 0)
        return value;
    else return -value;
}

float Max(float value, float value2) {
    if (value > value2)
        return value;
    else
        return value2;
}

float Min(float value, float value2) {
    if (value < value2)
        return value;
    else
        return value2;
}



float LimitToInterval(float value, float lowLimit, float highLimit) {
    if (value > highLimit)
        value = highLimit;
    else if (value < lowLimit)
        value = lowLimit;
    return value;
}

float RadianToDegree(float value) {
    return value / PI * 180.0;
}

float DegreeToRadian(float value) {
    return value * PI / 180.0;
}

float DistanceIR (int ADCValeur){
    return 3079*powf(ADCValeur, -1.145);
}

float MeterSecondToPercent(float vitesse){
    return 100/3.5*vitesse;
}