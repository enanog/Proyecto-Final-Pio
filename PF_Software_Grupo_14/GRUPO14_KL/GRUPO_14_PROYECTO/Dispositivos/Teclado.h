/*
 * ------------------------------
 * Teclado.h (para el MKL25Z)
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

#ifndef TECLADO_H_
#define TECLADO_H_

#include "MKL25Z4.h"
#include "Inicializacion.h"
#include "SysTick.h"

#define UP 			32		// Mover el pantografo para la adelante
#define	DOWN		128		// Mover el pantografo para la atras
#define	RIGHT		512		// Mover el pantografo para la derecha
#define	LEFT		8			// Mover el pantografo para la izquierda
#define	OK			64 		// Iniciar bordado
#define	CANCEL	16 		// Cancelar bordado
#define	STOP		256		// Parar bordado
#define	GO_ON		1024	// Seguir bordando
#define	ZEROS		1			// Ir a la posición 0;0
#define	MM			2			// Moverse cada un milimetro
#define	CM			4			// Moverse cada un centimetro
#define	RST	2048	// Moverse cada un centimetro

enum{
	C1 = 0,		// Columna 1 en 1
	C2,				// Columna 2 en 1
	C3,				// Columna 3 en 1
	C4				// Columna 4 en 1
};
	
#define CANT_TECLAS 11

extern uint8_t estadoTeclado;
extern uint16_t teclado;
extern uint16_t teclado_ant;
extern uint16_t teclado_sr;
extern uint8_t estadoAntTeclado;

/*
 * Función: buttonPushed()
 * -------------------------
 * Detecta que pulsdor del teclado matricial fue presionado.
 *
 * returns: botón pulsado
 */
void buttonPushed();

#endif /* TECLADO_H_ */