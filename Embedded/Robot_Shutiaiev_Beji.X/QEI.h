/* 
 * File:   QEI.h
 * Author: E306-PC3
 *
 * Created on 22 avril 2025, 16:13
 */

#ifndef QEI_H
#define	QEI_H

#ifdef	__cplusplus
extern "C" {
#endif

void InitQEI2();
void InitQEI1();

void QEIUpdateData();
void SendPositionData();

#ifdef	__cplusplus
}
#endif

#endif	/* QEI_H */

