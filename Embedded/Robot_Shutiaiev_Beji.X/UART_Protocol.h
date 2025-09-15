/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#define STATE_ATTENTE 0
#define STATE_FUNCTION_MSB 1
#define STATE_FUNCTION_LSB 2
#define STATE_PAYLOAD_LENGTH_MSB 3
#define STATE_PAYLOAD_LENGTH_LSB 4
#define STATE_PAYLOAD 5
#define STATE_CHECKSUM 6
#define FUNCTION_TEXT 0x0080
#define FUNCTION_LED1 0x0021
#define FUNCTION_LED2 0x0022
#define FUNCTION_LED3 0x0023
#define FUNCTION_TELEMETRE_GAUCHE 0x0031
#define FUNCTION_TELEMETRE_CENTRE 0x0032
#define FUNCTION_TELEMETRE_DROIT 0x0033
#define FUNCTION_VITESSE_GAUCHE 0x0041
#define FUNCTION_VITESSE_CENTRE 0x0042
#define SET_ROBOT_STATE 0x0051
#define CONFIG_PID 0x0061
#define CONFIG_VLINEAIRE 0x0071
#define CONFIG_VANGULAIRE 0x0072
#define DOLCE_GHOSTO 0x0091

#include <xc.h> // include processor files - each processor file is guarded.  

    void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, unsigned char* msgPayload);
    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 
    void UartDecodeMessage(unsigned char c);
    //void UartProcessDecodedMessage(int function, int payloadLength, unsigned char* payload);
    void UartProcessDecodedMessage(int rcvFunction, int payloadLength, unsigned char* payload);


#endif	/* XC_HEADER_TEMPLATE_H */

