/*
 * ------------------------------
 * Antirrebote.h (para el MKL25Z)
 * ------------------------------
 * 
 * Materias:
 * Taller - Sistemas Embebidos
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Revisión 2
 * Año 2023
 * 
 * Alumnos: Tomás BRULA, Dylan FRIGERIO
 * 
 */
 
#ifndef ANTIRREBOTE_H_
#define ANTIRREBOTE_H_

#include "MKL25Z4.h"
#include "GPIO.h"

enum{
	BAJO = 0,
	ANTIRREBOTE,	// 1
	ALTO 					// 2
};

typedef struct{
	GPIO_MemMapPtr portAnt;
	uint8_t pinAnt;
	uint8_t estadoAnt;
	uint8_t rpullup;
	uint8_t cant_pulsos;
} pin_antirrebote_t;

#define NOT_PULLUP		0
#define PULLUP 				1

#define CANT_PULSADOR 10

#define EST_BAJO 			0
#define EST_ALTO 			1

#define DES 					0
#define ASC			 			1

#define LOOSE					0
#define PUSH 					1

extern pin_antirrebote_t pulsador[CANT_PULSADOR];	// Variable que guarda datos del pulsador

// Ingreso puerto del pin y numero del pin y lo seteo como entrada, si habilito o no rpullup interna - SI = 1
void GPIO_Antirrebote(GPIO_MemMapPtr portDir, uint8_t pin, uint8_t r_pullup);

// Lee el valor del pin, hace un antirrebote y devuelve 1 cada vez que pasa por un flanco ASC o DES
uint8_t ME_AntirrebotePulsos(GPIO_MemMapPtr portDir, uint8_t pin, unsigned int t_ant, uint8_t pulso);
	
#endif /* ANTIRREBOTE_H_ */