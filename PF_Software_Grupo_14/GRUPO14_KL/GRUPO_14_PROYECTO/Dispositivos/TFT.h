/*
 * ------------------------------
 * TFT.c (para el MKL25Z)
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
 
#ifndef TFT_H_
#define TFT_H_

#include "MKL25Z4.h"
#include "GPIO.h"

#define RAM		8000

typedef struct{
	uint16_t x;
	uint16_t y;
	uint8_t centerx;
	uint8_t centery;
}xy_t;

extern xy_t display;
extern uint16_t bufferDisplay[RAM/2];
extern uint8_t save;

/*
 * Función: ILI9341_init
 * ----------------------
 * Inicializo ILI9341.
 */
void ILI9341_Init();


/*
 * Función: ILI9341_drawPixel
 * ---------------------------
 * Dibujo un pixel en el ILI9341.
 * 
 * Parámetros:
 * x: envío posición de X del pixel.
 * y: envío posición de Y del pixel.
 * color: envío el color del pixel.
 *
 * returns: nada.
 */
void ILI9341_drawPixel(int16_t x, int16_t y, uint16_t color);


/*
 * Función: ILI9341_drawPixel
 * ---------------------------
 * Dibujo un pixel de tramaño definido en el ILI9341.
 * 
 * Parámetros:
 * x: envío posición de X del pixel.
 * y: envío posición de Y del pixel.
 * color: envío el color del pixel.
 * pixelWidth: ancho del pixel (x).
 * pixelHeight: alto del pixel (y).
 *
 * returns: nada.
 */
void ILI9341_printPixel(uint16_t x, uint16_t y, uint16_t color, uint16_t pixelWidth, uint16_t pixelHeight);


/*
 * Función: ILI9341_drawString
 * ----------------------------
 * Dibujo un Char en el ILI9341.
 * 
 * Parámetros:
 * x: envío posición de X del pixel.
 * y: envío posición de Y del pixel.
 * color: envío el color del pixel.
 * letter: envío letra a escribir.
 * pixelWidth: ancho del pixel (x).
 * pixelHeight: alto del pixel (y).
 *
 * returns: valor de x y en el que termino de dibujar.
 */
void ILI9341_drawLetter(uint16_t x, uint16_t y, uint16_t color, char letter, uint8_t pixelWidth, uint8_t pixelHeight);


/*
 * Función: ILI9341_drawString
 * ----------------------------
 * Dibujo un String en el ILI9341.
 * 
 * Parámetros:
 * x: envío posición de X del pixel.
 * y: envío posición de Y del pixel.
 * color: envío el color del pixel.
 * ptr: envío string a escribir.
 * pixelWidth: ancho del pixel (x).
 * pixelHeight: alto del pixel (y).
 *
 * returns: nada.
 */
void ILI9341_drawString(uint16_t x, uint16_t y, uint16_t color, char* ptr, uint8_t pixelWidth, uint8_t pixelHeight);

void ILI9341_printLine(uint16_t xi, uint16_t yi, uint16_t color, uint16_t xf, uint16_t yf);

void ILI9341_printBuffer();

// Figuras
void drawFrame(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color, uint8_t fill, int8_t dash_length);

void drawVerticalProgressBar(uint16_t x, uint16_t y, uint16_t height, uint16_t width, uint16_t progress, uint16_t total, uint16_t fillColor, uint8_t aux);

void drawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color, uint8_t fill);

void drawCheckbox(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t boxColor, uint16_t checkmarkColor, uint8_t isChecked);

void drawTriangle(uint16_t x, uint16_t y, uint16_t size, uint16_t color, uint8_t fill);

#endif /* TFT_H_ */