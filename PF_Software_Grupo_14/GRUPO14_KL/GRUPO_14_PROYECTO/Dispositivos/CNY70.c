/*
 * ------------------------------
 * Comando.C (para el MKL25Z)
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

#include "Inicializacion.h"
#include "CNY70.h"
#include "GPIO.h"
 
uint8_t CNY70_Read(){
	if(GPIO_Read(CNY70) == 0)
		return 1;
	else
		return 0;
}
 