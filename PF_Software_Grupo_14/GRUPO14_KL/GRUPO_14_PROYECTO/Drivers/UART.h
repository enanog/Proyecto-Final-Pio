/*
 * -----------------------
 * UART.h (para el MKL25Z)
 * -----------------------
 * 
 * Materia:
 * Taller - Sistemas Electrónicos Digitales
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Revisión 3
 * Año 2023
 * 
 * Alumnos: Tomás BRULA, Dylan FRIGERIO
 * 
 */

#ifndef UART_H_
#define UART_H_

#include "MKL25Z4.h"

#define CLOCK_FREQ 		48000000
#define BUS_FREQ 		24000000

#define LENGTH_UART 1400					// tamaño del buffer (caracteres)

enum{
	RX = 0,
	TX,
	ENDRX
};

extern uint16_t wLength;					// indice de escritura
extern uint16_t rLength;					// indice de lectura

extern char wUart[LENGTH_UART];		// buffer de los comandos 

extern uint8_t auxTX;							// suma cada vez que ejecuto un comando
extern uint16_t auxRX;						// almacena la cantidad de caracteres cuando se piden nuevos comandos
extern uint8_t estadoRxTx;				// maquina de estado RX y TX

/*
 * Funcion: UART_Init
 * ----------------------------
 * Inicializa un modulo UART. Ajustar el valor de baudrate deseado.
 *
 *	     48MHz												|		 24MHz
 *	---------------- = SBR -> UART0		|	----------- = SBR	-> UART1/2
 *	(OSR+1) *  baud										|	16 *  baud
 *
 * Parametros:
 * uart: número de UART
 * baud: baudios de la comunicación
 *
 * returns: nada
 */
void UART_init(uint8_t uart, uint32_t baud);


/*
 * Funcion: UART_initOverflow
 * ----------------------------
 * Inicializa un modulo UART por interrupciones. Ajustar el valor de baudrate deseado.
 *
 *	     48MHz												|		 24MHz
 *	---------------- = SBR -> UART0		|	----------- = SBR	-> UART1/2
 * (OSR+1) *  baud										|	16 *  baud
 *
 * Parametros:
 * uart: número de UART
 * baud: baudios de la comunicación
 *
 * returns: nada
 */
void UART_initOverflow(uint8_t uart, uint32_t baud);

/*
 * Funcion: UART_Transmit
 * ----------------------------
 * Permite enviar un caracter por el UART.
 *
 * Parametros:
 * uart: número de UART
 * data: caracter a enviar
 *
 * returns: nada
 */
void UART_Transmit(uint8_t uart, char data);

/*
 * Funcion: UART_TX_Ready
 * ----------------------------
 * Verifica si el UART esta disponible para transmitir.
 *
 * Parametros:
 * uart: número de UART
 *
 * returns: 0 -> no está disponible
 * returns: 1 -> está disponible
 */
char UART_Receive(uint8_t uart);

/*
 * Funcion: UART_Receive
 * ----------------------------
 * Permite recbe un caracter por el UART.
 *
 * Parametros:
 * uart: número de UART
 * data: caracter a enviar
 *
 * returns: nada
 */
uint8_t UART_TX_Ready(uint8_t uart);

/*
 * Funcion: UART_RX_Ready
 * ------------------------
 * Verifica si el UART esta disponible para recibir.
 *
 * Parametros:
 * uart: número de UART
 *
 * returns: 0 -> no está disponible
 * returns: 1 -> está disponible
 */
uint8_t UART_RX_Ready(uint8_t uart);

/*
 * Funcion: UART_Diff_EscLec
 * ----------------------------
 * Verifica si el indice de escritura y lectura son distintos.
 *
 * Parametros: nada.
 *
 * returns: 0 -> si son iguales.
 * returns: 1 -> si son distintos.
 */
uint8_t UART_Diff_EscLec();

/*
 * Funcion: UART_RX_OverflowOFF
 * -----------------------------
 * Me deshabilita las interrrupciones.
 * 
 * Parametros:
 * uart: número de UART.
 *
 * returns: nada.
 */
void UART_RX_OverflowOFF(uint8_t uart);

/*
 * Funcion: UART_RX_OverflowON
 * -----------------------------
 * Me habilita las interrrupciones.
 * 
 * Parametros:
 * uart: número de UART.
 *
 * returns: nada.
 */
void UART_RX_OverflowON(uint8_t uart);

#endif /* UART_H_ */