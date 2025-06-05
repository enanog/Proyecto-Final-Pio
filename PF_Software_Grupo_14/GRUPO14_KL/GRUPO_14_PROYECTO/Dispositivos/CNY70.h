/*
 * -------------------------
 * CNY70.h (para el MKL25Z)
 * -------------------------
 * 
 * Proyecto Final:
 * AEM (maquina de bordado automática)
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Año 2023
 * 
 * Alumnos: Matías ALMARAZ, Tomás BRULA, Dylan FRIGERIO, Agustín VARGAS
 * 
 */
 
#ifndef CNY70_H_
#define CNY70_H_ 

#include "MKL25Z4.h"
#include "GPIO.h"


/*
 * Función: CNY70_Read
 * --------------------
 * Leo el estado del CN70.
 *
 * returns:
 *  si es '0' devuelvo un 1.
 *  si es '1' devuelvo un 0.
 */
uint8_t CNY70_Read();

#endif /* CNY70_H_ */