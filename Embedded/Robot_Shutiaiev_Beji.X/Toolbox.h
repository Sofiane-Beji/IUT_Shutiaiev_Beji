/* 
 * File:   Toolbox.h
 * Author: TP_EO_6
 *
 * Created on 11 septembre 2024, 14:59
 */
#define PI 3.141592653589793

#ifndef TOOLBOX_H
#define	TOOLBOX_H

#ifdef	__cplusplus
extern "C" {
#endif

    float Abs(float value);
    float Max(float value, float value2);
    float Min(float value, float value2);
    float LimitToInterval(float value, float lowLimit, float highLimit);
    float RadianToDegree(float value);
    float DegreeToRadian(float value);


#ifdef	__cplusplus
}
#endif

#endif	/* TOOLBOX_H */

