/* 
 * File:   CB_RX2.h
 * Author: Table2
 *
 * Created on 15 novembre 2024, 09:00
 */

#ifndef CB_RX2_H
#define	CB_RX2_H

#ifdef	__cplusplus
extern "C" {
#endif

void CB_RX2_Add(unsigned char value);
unsigned char CB_RX2_Get(void);
unsigned char CB_RX2_IsDataAvailable(void);
int CB_RX2_GetDataSize(void);
int CB_RX2_GetRemainingSize(void);


#ifdef	__cplusplus
}
#endif

#endif	/* CB_RX2_H */

