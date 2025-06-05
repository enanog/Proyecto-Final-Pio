/*
 * ------------------------------
 * Bordado.h (para el MKL25Z)
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

#ifndef BORDADO_H_
#define BORDADO_H_

#include "MKL25Z4.h"
#include "CNY70.h"
#include "TFT.h"
#include "NEMA17.h"
#include "protocolo.h"

extern uint8_t start;
extern uint8_t auxCheck;

void startEmbroider(char *ptr, uint8_t aux);

void envioTramas(char* ptr);

void screenMain();

void needle(uint16_t x, uint16_t y, uint16_t color);

void changeColorWarning(uint16_t color);

void endWarning();

void rangeWarning();

void cancelWarning();

#endif /* BORDADO_H_ */