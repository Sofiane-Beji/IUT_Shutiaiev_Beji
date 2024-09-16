/* 
 * File:   ADC.h
 * Author: TP_EO_6
 *
 * Created on 16 septembre 2024, 13:48
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

void InitADC1(void);
void ADC1StartConversionSequence();
unsigned int * ADCGetResult(void);
unsigned int ADCIsConversionFinished(void);
void ADCClearConversionFinishedFlag(void);


#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

