/*
 * ------------------------------
 * Comando.C (para el MKL25Z)
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

#include "Inicializacion.h"
#include "CNY70.h"
#include "GPIO.h"
 
uint8_t CNY70_Read(){
	if(GPIO_Read(CNY70) == 0)
		return 1;
	else
		return 0;
}
 