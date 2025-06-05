/*
 * -----------------------
 * ADC.h (para el MKL25Z)
 * -----------------------
 * 
 * Materia:
 * Taller - Sistemas Electr�nicos Digitales
 * 
 * 6to A�o T�cnico en Electr�nica
 * Colegio Pio IX
 * 
 * Revisi�n 3
 * A�o 2023
 * 
 * Alumnos: Tom�s BRULA, Dylan FRIGERIO
 * 
 */

#ifndef ADC_H_
#define ADC_H_

#include "MKL25Z4.h"

extern void ADC_Init(uint8_t channel,	uint8_t conversion);
extern void ADC_Channel(uint8_t pin, uint8_t continuous, uint8_t diff);
extern uint16_t ADC_Read(uint8_t pin, uint8_t continuous, uint8_t diff);

#endif /* ADC_H_ */