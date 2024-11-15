/* 
 * File:   CB_TX2.h
 * Author: Table2
 *
 * Created on 15 novembre 2024, 08:23
 */

#ifndef CB_TX2_H
#define	CB_TX2_H

#ifdef	__cplusplus
extern "C" {
#endif

void SendMessage(unsigned char* message, int length);
void CB_TX2_Add(unsigned char value);
unsigned char CB_TX2_Get(void);
void SendOne();
int CB_TX2_GetDataSize(void);
int CB_TX2_GetRemainingSize(void);



#ifdef	__cplusplus
}
#endif

#endif	/* CB_TX2_H */

