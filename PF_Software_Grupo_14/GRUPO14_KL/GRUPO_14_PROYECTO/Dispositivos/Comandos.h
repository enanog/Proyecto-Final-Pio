/*
 * ------------------------------
 * Comando.h (para el MKL25Z)
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
 
#ifndef COMANDO_H
#define COMANDO_H

#include "MKL25Z4.h"
#include "TFT.h"
#include "ILI9341.h"
#include "NEMA17.h"
#include "TRIAC.h"
#include "Teclado.h"

extern uint16_t cmd;	// Variable global que indica de que color dibujar
extern int coloresImagen[20];	// Variable global que indica de que color dibujar
extern uint16_t comandosMax;	// Variable global que indica de que color dibujar

/*
 * Función: lectura
 * -----------------
 * Imprime y manda al sistema de eje donde tiene que hacer un bordar un punto y el color.
 *
 * $ D x1i x2i x3i y1i y2i x1f x2f x3f y1f y2f #
 *
 * $: byte de inicio de trama.
 * D: comando de dibujar linea recta desde inicio a fin.
 * x1i: 4bits en HEX de la coordenada en X del pixel inicio.
 * x2i - x3i: byte en HEX de la coordenada en X del pixel inicio.
 * y1i - y2i: byte en HEX de la coordenada en Y del pixel inicio.
 * x1f: 4bits en HEX de la coordenada en X del pixel fin.
 * x2f - x3f: byte en HEX de la coordenada en X del pixel fin.
 * y1f: 4bits en HEX de la coordenada en Y del pixel fin.
 * ck: byte de checksum (verificación).
 * #: byte de finalización.
 *
 * Parámetros:
 * datos: datos recibidos de la UART.
 */
void lecturaDisplay(char *datos);


/*
 * Función: switchColorDisplay
 * ---------------------------
 * Guardo el color en una variable global para otras funciones.
 *
 * $ C X X #
 *
 * $: byte de inicio de trama.
 * C: comando de color a imprimir
 * X X: color a utilizar.
 * ck: byte de checksum (verificación).
 * #: byte de finalización.
 *
 * Parámetros:
 * datos: datos recibidos de la UART.
 */
void switchColorDisplay(char *datos);

/*
 * Función: sizeImage
 * --------------------
 * Recibe el tamaño de la imagen a bordar en mm x 10^-1. Máximo -> 1500 en X | 1000 en Y
 *
 * $ T x1 x2 x3 y1 y2 y3 #
 *
 * $: byte de inicio de trama.
 * T: comando que me indica el tamaño máximo de la imagén.
 * x1: 4bits en HEX de la coordenada en X del pixel.
 * x2 - x3: byte en HEX de la coordenada en X del pixel.
 * x1: 4bits en HEX de la coordenada en Y del pixel.
 * y2 - y3: byte en HEX de la coordenada en Y del pixel.
 * ck: byte de checksum (verificación).
 * #: byte de finalización.
 *
 * Parámetros:
 * datos: datos recibidos de la UART.
 */
void sizeImage(char *datos);

/*
 * Función: moveJump
 * -------------------
 * Mueve el pantografo sin bordar. Medidas en mm x10^-1.
 *
 * $ J x1 x2 x3 y1 y2 y3 #
 *
 * $: byte de inicio de trama.
 * J: comando que hace mover el sistema de ejes sin bordar.
 * x1: 4bits en HEX de la coordenada en X del pixel.
 * x2 - x3: byte en HEX de la coordenada en X del pixel.
 * x1: 4bits en HEX de la coordenada en Y del pixel.
 * y2 - y3: byte en HEX de la coordenada en Y del pixel.
 * ck: byte de checksum (verificación).
 * #: byte de finalización.
 *
 * Parámetros:
 * datos: datos recibidos de la UART.
 */
void moveJump(char *datos);

/*
 * Función: moveStitch
 * ---------------------
 * Mueve el pantografo bordando. Medidas en mm x10^-1.
 *
 * $ S x1 x2 x3 y1 y2 y3 #
 *
 * $: byte de inicio de trama.
 * S: comando que hace mover el sistema de ejes bordando.
 * x1: 4bits en HEX de la coordenada en X del pixel.
 * x2 - x3: byte en HEX de la coordenada en X del pixel.
 * x1: 4bits en HEX de la coordenada en Y del pixel.
 * y2 - y3: byte en HEX de la coordenada en Y del pixel.
 * ck: byte de checksum (verificación).
 * #: byte de finalización.
 *
 * Parámetros:
 * datos: datos recibidos de la UART.
 */
void moveStitch(char *datos);

/*
 * Función: endEmbroider
 * -----------------------
 * Avisa que ya termino de bordar.
 *
 * $ E #
 *
 * $: byte de inicio de trama.
 * E: comando fin.
 * ck: byte de checksum (verificación).
 * #: byte de finalización.
 */
void endEmbroider();

/*
 * Función: switchColorDisplay
 * ---------------------------
 * Guardo el color en una variable global para otras funciones.
 *
 * $ H X X #
 *
 * $: byte de inicio de trama.
 * H: comando de color de hilo
 * X X: color a utilizar.
 * ck: byte de checksum (verificación).
 * #: byte de finalización.
 *
 * Parámetros:
 * datos: datos recibidos de la UART.
 */
void switchColorEmbroider(char *datos);


/*
 * Función: colorEmbroider
 * ------------------------
 * Indica que colores se utilizan en las imagenes.
 *
 * $ P XX XX XX XX #
 *
 * $: byte de inicio de trama.
 * W: comando apagar motor.
 * XX: comando de colores.
 * #: byte de finalización.
 */
void colorEmbroider(char *datos);


/*
 * Función: commandMax
 * ------------------------
 * Indica la cantidad de Stichs y Jumps que debe de hacer.
 *
 * $ W XXXX #
 *
 * $: byte de inicio de trama.
 * W: comando apagar motor.
 * XXXX: cantidad de comandos.
 * #: byte de finalización.
 */
void commandMax(char *datos);

#endif /* COMANDO_H */