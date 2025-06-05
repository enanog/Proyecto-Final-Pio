/*
 * ------------------------------
 * NEMA17.h (para el MKL25Z)
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
 * Alumnos: Matías ALMARAZ, Tomás BRULA, Dylan FRIGERIO, Agustín VARGAS
 * 
 */

#ifndef NEMA17_H_
#define NEMA17_H_

#include <stdint.h>
#include "Comandos.h"
#include "Inicializacion.h"
#include "GPIO.h"
#include "TPM.h"
#include "TFT.h"

#define SENTIDO_HORARIO 			1
#define SENTIDO_ANTIHORARIO 	0

#define X 					0					// Movimiento del perfil inferior
#define Y 					1					// Movimiento del perfil superior

//#define FULL_STEP					2		// Medida en mm x10^-1.
//#define HALF_STEP					1		// Medida en mm x10^-1.
#define QUARTER_STEP			0.5		// Medida en mm x10^-1.
//#define EIGHTH_STEP			0.25	// Medida en mm x10^-1.
//#define SIXTEENTH_STEP	0.13	// Medida en mm x10^-1.

#define MOTOR_VELOCIDAD		 	8			// frec: 120Hz

/*
 *        STEP				
 * ---------------------			
 * 1ms * MOTOR_VELOCIDAD		
 */
#ifdef FULL_STEP 
	#define STEP FULL_STEP
	#define MOVE_CM 					50		// Cantidad de pasos completos para desplazarse 10mm
	#define MOVE_MM 					5 		// Cantidad de pasos completos para desplazarse 1mm
	
#elif defined HALF_STEP
	#define STEP HALF_STEP
	#define MOVE_CM 					100		// Cantidad de medio pasos para desplazarse 10mm
	#define MOVE_MM 					10 		// Cantidad de medio pasos para desplazarse 1mm
	
#elif defined QUARTER_STEP
	#define STEP QUARTER_STEP
	#define MOVE_CM 					200		// Cantidad de cuartos de pasos para desplazarse 10mm
	#define MOVE_MM 					20 		// Cantidad de cuartos de pasos para desplazarse 1mm
	
#elif defined EIGHTH_STEP
	#define STEP EIGHTH_STEP
	#define MOVE_CM 					400		// Cantidad de octavos de pasos para desplazarse 10mm
	#define MOVE_MM 					40 		// Cantidad de octavos de pasos para desplazarse 1mm
	
#elif defined SIXTEENTH_STEP
	#define STEP SIXTEENTH_STEP
	#define MOVE_CM 					833		// Cantidad de octavos de pasos para desplazarse 10mm
	#define MOVE_MM 					83 		// Cantidad de octavos de pasos para desplazarse 1mm
	
#endif

typedef struct{
	uint16_t x;
	uint16_t y;
	uint16_t offsetx;
	uint16_t offsety;
} coordenadas_t;	// Medidas en mm x10^-1.

extern coordenadas_t sizeimagen;	// Medida en mm x10^-1.

extern coordenadas_t position;		// Medidas en mm x10^-1.

/*
 * Función: NEMA17_changeDIR
 * ---------------------------
 * Cambio la dirección del motor paso a paso.
 * 
 * Parámetros:
 * axis: indico de que motor quiero cambiar la dirección, X o Y.
 * direction: indico para que sentido quiero que gire el motor, HORARIO o ANTIHORARIO.
 *
 * returns: nada.
 */
void NEMA17_changeDIR(uint8_t axis, uint8_t direction);


/*
 * Función: NEMA17_moveContinuous
 * -------------------------------
 * Gira el motor x cantidad de pasos.
 * 
 * Parámetros:
 * axis: indico con que motor quiero mover continuamente, X o Y.
 * cantMove: indico la cantidad de pasos que tiene que dar el motor.
 *
 * returns: nada.
 */
void NEMA17_moveContinuous(uint8_t axis, uint16_t cantMove);


/*
 * Función: NEMA17_toPosition
 * ----------------------------
 * Se calcula la diferencia entre la posición actual y la posición de destino.
 * Se verifica y ajusta la dirección del motor NEMA17 en los ejes X e Y.
 *
 * Parámetros:
 * moveTo_x: indico el punto en X.
 * moveTo_y: indico el punto en X.
 *
 * returns: cantidad de pasos que tiene que hacer para llegar a destino.
 */
coordenadas_t NEMA17_toPosition(uint16_t moveTo_x, uint16_t moveTo_y);


/*
 * Función: NEMA17_positionStart
 * -------------------------------
 * Desplaza el pantografo hasta el punto inicio 0;0 (arriba a la izquierda).
 */
void NEMA17_positionStart();


/*
 * Función: NEMA17_newPositionStart
 * ----------------------------------
 * Calcula si se puede iniciar el bordado de la imagén o si sobredimensiona el pantografo. 
 *
 * Parámetros:
 * x - y: punto 0;0 de la imagen a bordar (arriba a la izquierda).  
 * sizex - sizey: el punto más alejado del 0;0 de la imagen a bordar (abajo a la derecha).
 *
 * returns: 0 -> sale del tamaño del pantografo | 1 -> se puede realizar el bordado.
 */
uint8_t NEMA17_newPositionStart(uint16_t x, uint16_t y, uint16_t sizex, uint8_t sizey);


/*
 * Función: NEMA17_moveManual
 * ----------------------------
 * Calcula si se puede iniciar el bordado de la imagén o si sobredimensiona el pantografo. 
 *
 * Parámetros:
 * button: botones del teclado matricial | UP -> Y(horario) | DOWN -> Y(antihorario) | RIGHT -> X(horario) | LEFT -> X(antihorario).  
 * movePrescaler: el punto más alejado del 0;0 de la imagen a bordar (abajo a la derecha).
 *
 * returns: 0 -> sale del tamaño del pantografo | 1 -> se puede realizar el movimiento.
 */
uint8_t NEMA17_movManual(uint16_t button, uint8_t movePrescaler);

#endif /* NEMA17_H_ */