/*
 * ----------------------
 * TPM.h (para el MKL25Z)
 * ----------------------
 * 
 * Materias:
 * Sistemas Electrónicos Embebidos
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Revisión 1
 * Año 2023
 * 
 * Profesores: Matías Marfisi, Tomás Zuran, Federico Diograzia
 * 
 */

#ifndef _TPM_H
#define _TPM_H

#include "MKL25Z4.h"
#include "GPIO.h"
#include "NEMA17.h"
#include "Bordado.h"

#define CLOCK_FREQ 		48000000

/*
 * Enum para bits PS TPMx_SC control clock prescaler (pag. 552 Reference Manual)
 */
enum{
	PRESCALER_1 = 0,
	PRESCALER_2, // 1 
	PRESCALER_4, // 2
	PRESCALER_8, // 3
	PRESCALER_16, // 4 
	PRESCALER_32, // 5
	PRESCALER_64, // 6
	PRESCALER_128 // 7
};

extern int velocidadX;
extern int velocidadY;
extern int tiempoEndMove;
extern int tiempoMotor;
extern uint8_t subirStop;

void TPM_InitOverflow(uint8_t tpm, uint32_t freq, uint8_t prescaler);
void TPM_InitPWM(uint8_t tpm, uint8_t channel, uint32_t freq, uint8_t prescaler, uint8_t duty);
void TPM_ChangeDuty(uint8_t tpm, uint8_t channel, uint8_t duty);

#endif