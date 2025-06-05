/*
 * ----------------------
 * TSI.c (para el MKL25Z)
 * ----------------------
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
 
#ifndef TSI0_H_
#define TSI0_H_

#include "MKL25Z4.h"

/*
 * Funcion: TSI_Init
 * ----------------------------
 *	Inicializa el modulo TSI.
 *	Parametros:
 *	Nada
 *
 *	returns: nada
 */
void TSI_Init();

/*
 * Funcion: TSI_ReadDistance
 * ----------------------------
 *	Obtiene el valor actual del TSI.
 *	Parametros:
 *	Nada
 *
 *	returns: un valor entre 0 y 40 en funci�n de donde se este apretando.
 */
uint8_t TSI_ReadDistance();

#endif /* TSI0_H_ */
