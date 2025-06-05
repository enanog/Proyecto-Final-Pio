/*
 * ------------------------------
 * Antirrebote.c (para el MKL25Z)
 * ------------------------------
 * 
 * Materias:
 * Taller - Sistemas Embebidos
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Revisión 1
 * Año 2023
 * 
 * Alumnos: Tomás BRULA, Dylan FRIGERIO
 * 
 */
 
#include "Antirrebote.h"
#include "GPIO.h"
#include "SysTick.h"

pin_antirrebote_t pulsador[CANT_PULSADOR] = {0};
int cantP = 0;

// Ingreso puerto del pin y numero del pin, lo seteo como entrada y si habilito o no rpullup interna - SI = 1
extern void GPIO_Antirrebote(GPIO_MemMapPtr portDir, uint8_t pin, uint8_t r_pullup){
	static uint8_t i = 0;
	
	// Inicializo el pin como entrada
	GPIO_Init(portDir, pin);
	
	GPIO_Mode(portDir, pin, INPUT);
	
	// Guardo datos del pulsador
	pulsador[i].portAnt = portDir;
	pulsador[i].pinAnt = pin;
	pulsador[i].cant_pulsos = 0;
	
	/*
	 * Estado para máquina de estados del Antirrebote
	 * Estado inicial en OFF
	*/
	pulsador[i].estadoAnt = BAJO;
	
	if(r_pullup == 1){
		// Inicializo R-PULL UP interna (50K)
		GPIO_EnablePullUpResistor(portDir, pin);
		
		pulsador[i].rpullup = 1;
	}
	else
		pulsador[i].rpullup = 0;
	
	i++;
}

// Lee el valor del pin, hace un antirrebote y devuelve cant de pulsos del pulsador
uint8_t ME_AntirrebotePulsos(GPIO_MemMapPtr portDir, uint8_t pin, unsigned int t_ant, uint8_t pulso){
	uint8_t i;
	uint8_t p = EST_BAJO;		// Variable estado del pulsador
	uint8_t pa = EST_BAJO;	// Variable estado anterior del pulsador 
	
	// Busco pulsador apretado
	for(i = 0; i < CANT_PULSADOR; i++){
		if(pulsador[i].portAnt == portDir && pulsador[i].pinAnt == pin)
			break;
	}
	
	switch(pulsador[i].estadoAnt){
		case BAJO:
			//p = 0;
			/*
			 * Si el pulsador cambia de estado, se activa un temporizador y pasa de estado 
			 * Invierto las condiciones para que el estado inicial sea siempre el mismo
			 */
			if((GPIO_Read(portDir, pin) == EST_ALTO && pulsador[i].rpullup == 0) || (GPIO_Read(portDir, pin) == EST_BAJO && pulsador[i].rpullup == 1)){
				pulsador[i].estadoAnt = ANTIRREBOTE;
				tiempo_Ant = t_ant;
			}
			break;
			
		case ANTIRREBOTE:
			/*
			 * Si el pulsador cambia de estado BAJO a ALTO y se mantiene en ese estado luego del tiempo predefinido, pasa de estado 
			 * Sino vuelve al estado anterior
			 */
			if((GPIO_Read(portDir, pin) == EST_ALTO && tiempo_Ant == 0 && pa == EST_BAJO && pulsador[i].rpullup == 0)||
				(GPIO_Read(portDir, pin) == EST_BAJO && tiempo_Ant == 0 && pa == EST_BAJO && pulsador[i].rpullup == 1)){
				pulsador[i].estadoAnt = ALTO;
				pa = EST_ALTO;
				if(pulso == ASC)
					p = 1;
				//else
					//p = 0;
			}	
			else if((GPIO_Read(portDir, pin) == EST_BAJO && tiempo_Ant == 0 && pa == EST_BAJO && pulsador[i].rpullup == 0)||
				(GPIO_Read(portDir, pin) == EST_ALTO && tiempo_Ant == 0 && pa == EST_BAJO && pulsador[i].rpullup == 1))
					pulsador[i].estadoAnt = BAJO;
			
			
			/*
			 * Si el pulsador cambia de estado ALTO a BAJO y se mantiene en ese estado luego del tiempo predefinido, pasa de estado 
			 * Sino vuelve al estado anterior
			 */
			if((GPIO_Read(portDir, pin) == EST_BAJO && tiempo_Ant == 0 && pa == EST_ALTO && pulsador[i].rpullup == 0)||
				(GPIO_Read(portDir, pin) == EST_ALTO && tiempo_Ant == 0 && pa == EST_ALTO && pulsador[i].rpullup == 1)){
				pulsador[i].estadoAnt = BAJO;
				pa = EST_BAJO;
					
				if(pulso == DES)
					p = 1;
				
				//else
					//p = 0;
			}	
			else if((GPIO_Read(portDir, pin) == EST_ALTO && tiempo_Ant == 0 && pa == EST_ALTO && pulsador[i].rpullup == 0)||
				(GPIO_Read(portDir, pin) == EST_BAJO && tiempo_Ant == 0 && pa == EST_ALTO && pulsador[i].rpullup == 1))
					pulsador[i].estadoAnt = ALTO;
					
			break;
		case ALTO:
			//p = 0;
			/*
			 * Si el pulsador cambia de estado, se activa un temporizador y pasa de estado 
			 * Invierto las condiciones para que el estado inicial sea siempre el mismo
			 */
			if((GPIO_Read(portDir, pin) == EST_BAJO && pulsador[i].rpullup == 0) || (GPIO_Read(portDir, pin) == EST_ALTO && pulsador[i].rpullup == 1)){
					pulsador[i].estadoAnt = ANTIRREBOTE;
					tiempo_Ant = t_ant;
				}
			break;
		default:
			break;
	}
	if(p == 1)
		cantP++;
	return p;
}