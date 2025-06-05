/*
 * -------------------------
 * CNY70.h (para el MKL25Z)
 * -------------------------
 * 
 * Proyecto Final:
 * AEM (maquina de bordado autom�tica)
 * 
 * 6to A�o T�cnico en Electr�nica
 * Colegio Pio IX
 * 
 * A�o 2023
 * 
 * Alumnos: Mat�as ALMARAZ, Tom�s BRULA, Dylan FRIGERIO, Agust�n VARGAS
 * 
 */
 
#ifndef CNY70_H_
#define CNY70_H_ 

#include "MKL25Z4.h"
#include "GPIO.h"


/*
 * Funci�n: CNY70_Read
 * --------------------
 * Leo el estado del CN70.
 *
 * returns:
 *  si es '0' devuelvo un 1.
 *  si es '1' devuelvo un 0.
 */
uint8_t CNY70_Read();

#endif /* CNY70_H_ */