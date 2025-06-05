/*
 * ------------------------------
 * Inicialización.c (para el MKL25Z)
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
#include "GPIO.h"


void init_TFTPin(){
	GPIO_Init(DCRS);
	GPIO_Mode(DCRS, OUTPUT);
	GPIO_Set(DCRS, LOW);
	
	GPIO_Init(RESET_TFT);
	GPIO_Mode(RESET_TFT, OUTPUT);
	GPIO_Set(DCRS, LOW);
}


void init_CNY70Pin(){
	GPIO_InputPullUP(CNY70);
}

void init_TRIACPin(){
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	PORTD_PCR7 = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0b1011);
	NVIC_EnableIRQ(PORTD_IRQn);
	
	GPIO_Init(GPIOD, 6);
	GPIO_Mode(GPIOD, 6, OUTPUT);
	GPIO_Set(GPIOD, 6, LOW);
}

void init_NEMAPin(){
	GPIO_Init(DIRX);
	GPIO_Mode(DIRX, OUTPUT);
	GPIO_Set(DIRX, LOW);

	GPIO_Init(STEPX);
	GPIO_Mode(STEPX, OUTPUT);
	GPIO_Set(STEPX, LOW);	
	
	GPIO_Init(DIRY);
	GPIO_Mode(DIRY, OUTPUT);
	GPIO_Set(DIRY, LOW);
	
	GPIO_Init(STEPY);
	GPIO_Mode(STEPY, OUTPUT);
	GPIO_Set(STEPY, LOW);
}

void init_FINESPin(){
	GPIO_InputPullUP(FINX);
	GPIO_InputPullUP(FINY);
}

void init_TecladoPin(){
	GPIO_Init(COLUMN1);
	GPIO_Mode(COLUMN1, OUTPUT);
	GPIO_Set(COLUMN1, LOW);
	GPIO_Init(COLUMN2);
	GPIO_Mode(COLUMN2, OUTPUT);
	GPIO_Set(COLUMN2, LOW);
	GPIO_Init(COLUMN3);
	GPIO_Mode(COLUMN3, OUTPUT);
	GPIO_Set(COLUMN3, LOW);
	GPIO_Init(COLUMN4);
	GPIO_Mode(COLUMN4, OUTPUT);
	GPIO_Set(COLUMN4, LOW);
	
	GPIO_InputPullUP(ROW1);
	GPIO_InputPullUP(ROW2);
	GPIO_InputPullUP(ROW3);
	GPIO_InputPullUP(ROW4);
}