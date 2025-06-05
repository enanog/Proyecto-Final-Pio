/*
 * --------------------------
 * SysTick.h (para el MKL25Z)
 * --------------------------
 * 
 * Materias:
 * Sistemas Electr�nicos Embebidos
 * 
 * 6to A�o T�cnico en Electr�nica
 * Colegio Pio IX
 * 
 * Revisi�n 1
 * A�o 2023
 * 
 * Profesores: Mat�as Marfisi, Tom�s Zuran, Federico Diograzia
 * 
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "MKL25Z4.h"
#include "UART.h"
#include "Teclado.h"
#include "TRIAC.h"

#define CLOCK_FREQ 48000000

#define SYSTICK_ENABLE 		(1 << 0)
#define SYSTICK_TICKINT 	(1 << 1)
#define SYSTICK_CLKSOURCE	(1 << 2)

extern uint8_t tiempoColumna;					// timer para cambiar el estado de las columnas del teclado
extern uint8_t tiempo_teclado;
extern uint32_t tiempo_Ant;						// timer para el antirrebote del teclado
extern uint32_t tiempo_Triac;					
extern uint32_t habilitacion_Triac;		
extern uint32_t tiempo_inercia_motor;
extern uint32_t t_CNY70;
extern uint32_t flag_t_CNY70;
extern uint32_t warningColor;

void SysTick_Init(int ms); // Tiempo en ms deseado para interrupcion

#endif
