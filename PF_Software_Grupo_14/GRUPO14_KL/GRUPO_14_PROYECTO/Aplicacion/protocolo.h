/*
 * ------------------------------
 * protocolo.h (para el MKL25Z)
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

#ifndef PROTOCOLO_H
#define PROTOCOLO_H

#include "UART.h"
#include "Inicializacion.h"
#include "Comandos.h"
#include "TFT.h"
#include "ILI9341.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern void protocolo_me(uint8_t uart, char caracter_recibido);
extern void envio_trama(char comando, char *datos);

extern uint8_t cant_comandos;
extern uint32_t cant_JS;

#endif /* PROTOCOLO_H_ */