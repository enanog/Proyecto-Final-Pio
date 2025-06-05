/*
 * --------------------------
 * SysTick.c (para el MKL25Z)
 * --------------------------
 * 
 * Materias:
 * Sistemas Electrónicos Embebidos
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Revisión 1
 * Año 2023
 * 
 * Profesores: Matías Marfisi, Tomás Zuran, Federico Diograzia
 * 
 */

#include "SysTick.h"

uint8_t tiempoColumna = 0;
uint32_t tiempo_Ant = 0;
uint32_t t_CNY70 = 0;
uint32_t flag_t_CNY70 = 0;
uint32_t tiempo_Triac = 0;					
uint32_t habilitacion_Triac = 0;		
uint32_t tiempo_inercia_motor = 0;
uint8_t tiempo_teclado = 0;
uint32_t warningColor = 0;

void SysTick_Handler(void)
{
	if(tiempo_Ant > 0)
		tiempo_Ant--;
	
	tiempoColumna++;
	
	if(tiempo_teclado > 0)
		tiempo_teclado--;
	
	if(habilitacion_Triac)	
		tiempo_Triac++;
	else
		tiempo_Triac = 0;
	
	if(tiempo_inercia_motor > 0)
		tiempo_inercia_motor--;
	
	if (tiempoColumna <= 10)							
		estadoTeclado = C1;		// Columna1
	else if (tiempoColumna > 10 && tiempoColumna <= 20)		
		estadoTeclado = C2;		// Columna2
	else if (tiempoColumna > 20 && tiempoColumna <= 30)		
		estadoTeclado = C3;		// Columna3
	else if (tiempoColumna > 30 && tiempoColumna <= 40)		
		estadoTeclado = C4;		// Columna4
	else
		tiempoColumna = 0;
	
	if(flag_t_CNY70)
		t_CNY70++;
	
	if(warningColor > 0)
		warningColor --;
	
	buttonPushed();
}

void SysTick_Init(int ms)
{
	SysTick->LOAD = ms * (CLOCK_FREQ / 1000) - 1;
	SysTick->CTRL = SYSTICK_ENABLE | SYSTICK_TICKINT | SYSTICK_CLKSOURCE;
}
