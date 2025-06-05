/*
 * ----------------------
 * TSI.c (para el MKL25Z)
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
 *	returns: un valor entre 0 y 40 en función de donde se este apretando.
 */
uint8_t TSI_ReadDistance();

#endif /* TSI0_H_ */
