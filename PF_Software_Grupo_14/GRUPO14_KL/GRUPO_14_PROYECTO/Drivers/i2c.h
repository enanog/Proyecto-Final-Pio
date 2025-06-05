 /*
 * -----------------------
 * i2c.h 
 * -----------------------
 * 
 * Materias:
 * Sistemas Electr�nicos Embebidos
 * 
 * 6to A�o T�cnico en Electr�nica
 * Colegio Pio IX
 * 
 * Revisi�n 1 Tomas Zur�n
 * Revisi�n 2 Mat�as Marfisi
 * A�o 2023
 * 
 * Profesores: Mat�as Marfisi, Tom�s Zuran, Federico Diograzia
 * 
 */

#ifndef I2C_H_
#define I2C_H_

#include "stdint.h"
#include "MKL25Z4.h"

/*
 * Funcion: I2C0_Init
 * ----------------------------
 *	Inicializa el I2C0.
 *
 *	Parametros: ninguno
 * 
 *	returns: nada
 */
void I2C0_Init(void);

/*
 * Funcion: I2C0_WriteRegister
 * ----------------------------
 *	Escribe un byte al registro especificado.
 *
 *	Parametros: 
 *	I2C_Address: direccion del dispositivo a escribir
 *	u8RegisterAddress: direccion del registro a escribir
 *	u8Data: dato a enviar 
 *
 *	returns: nada
 */
void I2C0_WriteRegister(uint8_t I2C_Address, uint8_t u8RegisterAddress, uint8_t u8Data);

/*
 * Funcion: I2C0_Write
 * ----------------------------
 *	Escribe n bytes al registro especificado.
 *
 *	Parametros: 
 *	I2C_Address: direccion del dispositivo a escribir
 *	u8RegisterAddress: direccion del registro a escribir
 *	u8Data: vector de datos a enviar 
 *	n_bytes: numero de bytes a enviar
 *
 *	returns: nada
 */
void I2C0_Write(uint8_t I2C_Address, uint8_t u8RegisterAddress, uint8_t * u8Data, uint8_t n_bytes);

/*
 * Funcion: I2C0_ReadRegister
 * ----------------------------
 *	Lee un byte del registro especificado.
 *
 *	Parametros: 
 *	I2C_Address: direccion del dispositivo a leer
 *	u8RegisterAddress: direccion del registro a leer
 *
 *	returns: nada
 */
uint8_t I2C0_ReadRegister(uint8_t I2C_Address, uint8_t u8RegisterAddress);

/*
 * Funcion: I2C0_Read
 * ----------------------------
 *	Lee n bytes a partir del registro especificado.
 *
 *	Parametros: 
 *	I2C_Address: direccion del dispositivo a escribir
 *	u8RegisterAddress: direccion del registro a escribir
 *	datos: vector donde se guardar�n los datos leidos
 *	n_bytes: numero de bytes a leer
 *
 *	returns: nada
 */
void I2C0_Read(uint8_t I2C_Address, uint8_t u8RegisterAddress, uint8_t * datos, uint8_t n_bytes);

#endif /* I2C_H_ */
