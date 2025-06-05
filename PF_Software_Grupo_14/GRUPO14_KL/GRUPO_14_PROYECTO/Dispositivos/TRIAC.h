/*
 * ------------------------------
 * TRIAC.H (para el MKL25Z)
 * ------------------------------
 * 
 * Proyecto Final:
 * AEM (maquina de bordado automática)
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Año 2023
 * 
 * Alumnos: Matías ALMARAZ, Tomás BRULA, Dylan FRIGERIO, AgustíN VARGAS
 * 
 */

#ifndef TRIAC_H_
#define TRIAC_H_

#include "MKL25Z4.h"
#include "Inicializacion.h"
#include "TPM.h"	
#include "SysTick.h"
#include "CNY70.h"
/*
 * El modificador extern nos permite acceder a una variable definida en este archivo, en cualquier otro.
 * En este ejemplo, se utilizará en main.c para controlar el estado del LED rojo.
 */
 
enum{
	ESPERA_HIGH = 0,
	ON,
	OFF,
	ESPERA_LOW
}; 
 
extern uint8_t ME_Triac;
extern uint32_t flanco_Triac;
extern uint32_t velocidad_Aguja;
extern uint8_t habilitacion_Motor;
extern uint8_t semiciclos;
/*
 * Función: control_Velocidad_Aguja
 * --------------------
 * Seteo de velocidad de la aguja.
 * 
 * Implementacion:
 *  Velocidades:
 *		0 -> Min
 *		7 -> Max
 */
void motor_Maquina(int velocidad_Aguja_Requerida);

/*
 * Función: cruce_Por_Ceros_Y_Velocidad()
 * --------------------
 * Maquina de estados que detecta el cruce por ceros y nos permite controlar el motor.
 */
void cruce_Por_Ceros_Y_Velocidad();

/*
 * Función: control_Velocidad_Aguja
 * --------------------
 * Seteo de velocidad de la aguja.
 *
 * Implementacion:
 *  Velocidades:
 *		1 -> Min
 *		9 -> Max
 */
 
 void control_Inercia(int velocidad_requerida);
 
 /*
 * Función: control_Inercia(int velocidad_requerida)
 * --------------------
 * Rompe la inercia del motor y hace que despues gire a la velocidad que quieras
 */
 
 void subir_Aguja();
 
 /*
 * Función: subir_Aguja
 * --------------------
 * Si la aguja esta abajo la sube
 */
 
#endif /* TRIAC_H_ */