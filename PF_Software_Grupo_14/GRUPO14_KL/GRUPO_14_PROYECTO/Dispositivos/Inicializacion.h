/*
 * ------------------------------
 * Inicializacion.c (para el MKL25Z)
 * ------------------------------
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
 
#ifndef INICIALIZACION_H_
#define INICIALIZACION_H_

#include "MKL25Z4.h"
#include "GPIO.h"
#include "Antirrebote.h"

/* ----------- TFT --------------*/
#define DCRS 								GPIOC, 3
#define RESET_TFT						GPIOC, 0
/* ----------- CNY70 -------------*/
#define CNY70 							GPIOE, 29
/* ----------- TRIAC -------------*/
#define CONTROL_VELOCIDAD 	GPIOD, 6
#define CRUCE_POR_CEROS 		GPIOD, 7
/* ----------- NEMA --------------*/
#define DIRX							 	GPIOE, 5
#define STEPX						 		GPIOE, 4
#define DIRY							 	GPIOE, 3
#define STEPY						 		GPIOE, 2
/* ----------- FINES -------------*/
#define FINX							 	GPIOB, 9
#define FINY						 		GPIOB, 8
/* ----------- TFT --------------*/
#define DCRS 								GPIOC, 3
#define RESET_TFT						GPIOC, 0
/* ----------- CNY70 -------------*/
#define CNY70 							GPIOE, 29
/* ----------- TRIAC -------------*/
#define CONTROL_VELOCIDAD 	GPIOD, 6
#define CRUCE_POR_CEROS 		GPIOD, 7
/* ----------- NEMA --------------*/
#define DIRX							 	GPIOE, 5
#define STEPX						 		GPIOE, 4
#define DIRY							 	GPIOE, 3
#define STEPY						 		GPIOE, 2
/* ----------- FINES -------------*/
#define FINX							 	GPIOB, 9
#define FINY						 		GPIOB, 8
/* ----------- TECLADO -----------*/
#define ROW4							 	GPIOE, 0
#define ROW3						 		GPIOD, 1
#define ROW2							 	GPIOD, 3
#define ROW1						 		GPIOD, 2
#define COLUMN1						 	GPIOD, 0
#define COLUMN2					 		GPIOD, 5
#define COLUMN3						 	GPIOC, 9
#define COLUMN4					 		GPIOC, 8

/*
 * Funci�n: init_TFTPin
 * ----------------------
 * Inicializa pines del display TFT SPI.
 */
void init_TFTPin();


/*
 * Funci�n: init_CNY70Pin
 * ------------------------
 * Inicializa pines del display TFT SPI.
 */
void init_CNY70Pin();


/*
 * Funci�n: init_TRIACPin
 * ------------------------
 * Inicializa pines del circuito del TRIAC.
 */
void init_TRIACPin();


/*
 * Funci�n: init_NEMAPin
 * -----------------------
 * Inicializa pines de los dos motores NEMA
 */
void init_NEMAPin();


/*
 * Funci�n: init_FINESPin
 * ------------------------
 * Inicializa pines de los cuatro fines de carrera
 */
void init_FINESPin();

/*
 * Funci�n: init_TecladoPin
 * ------------------------
 * Inicializa pines de los cuatro fines de carrera
 */
void init_TecladoPin();


#endif /* INICIALIZACION_H_ */
