 /*
 * -----------------------
 * SPI.h 
 * -----------------------
 * 
 * Materias:
 * Sistemas Electrónicos Embebidos
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Revisión 1 Matías Marfisi
 * Año 2023
 * 
 * Profesores: Matías Marfisi, Tomás Zuran, Federico Diograzia
 * 
 */

#ifndef SPI_H_
#define SPI_H_

#include "stdint.h"
#include "MKL25Z4.h"

/*
 * Funcion: SPI_Init
 * ----------------------------
 *	Inicializa el modulo SPI indicado a 1MHz, verificar frecuencia de ser necesario.
 *
 *	Parametros:
 *	spi: numero de spi a inicializar (0-1)
 *	polarity: polaridad de clock
 *	fase: sampleo vs toggle de datos
 * 
 *	returns: nada
 */
void SPI_Init(uint8_t spi, uint8_t polarity, uint8_t phase);

/*
 * Funcion: SPI_MasterWrite
 * ----------------------------
 *	Escribe datos a un SPI esclavo, y retorna la respuesta.
 *
 *	Parametros:
 *	spi: numero de spi a enviar.
 *	value: dato a enviar.
 * 
 *	returns: nada
 */
uint32_t SPI_MasterWrite(uint8_t spi, uint8_t value);

/*
 * Funcion: SPI_MasterRead
 * ----------------------------
 *	Escribe datos a un SPI esclavo, y retorna la respuesta.
 *
 *	Parametros:
 *	spi: numero de spi a enviar.
 * 
 *	returns: nada
 */
uint32_t SPI_MasterRead(uint8_t spi);

/*
 * Funcion: SPI_MasterBlockWrite
 * ----------------------------
 *	Escribe multiples datos a un SPI esclavo, y retorna la respuesta.
 *
 *	Parametros:
 *	spi: numero de spi a enviar.
 *	tx_buffer: vector de datos a enviar
 *	tx_length: largo de datos a enviar
 *	rx_buffer: vector de datos a recibir
 *	rx_length: largo de datos a recibir
 * 
 *	returns: nada
 */
uint32_t SPI_MasterBlockWrite(uint8_t spi, const char *tx_buffer, uint32_t tx_length, char *rx_buffer, uint32_t rx_length);

#endif
