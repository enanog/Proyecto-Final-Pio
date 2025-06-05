/*
 * -----------------------
 * ADC.h (para el MKL25Z)
 * -----------------------
 * 
 * Materia:
 * Taller - Sistemas Electrónicos Digitales
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Revisión 3
 * Año 2023
 * 
 * Alumnos: Tomás BRULA, Dylan FRIGERIO
 * 
 */

#ifndef ADC_H_
#define ADC_H_

#include "MKL25Z4.h"

extern void ADC_Init(uint8_t channel,	uint8_t conversion);
extern void ADC_Channel(uint8_t pin, uint8_t continuous, uint8_t diff);
extern uint16_t ADC_Read(uint8_t pin, uint8_t continuous, uint8_t diff);

#endif /* ADC_H_ */