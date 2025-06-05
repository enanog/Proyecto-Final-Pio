/*
 * -----------------------
 * UART.c (para el MKL25Z)
 * -----------------------
 * 
 * Materia:
 * Taller - Sistemas Electrónicos Digitales
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Revisión 3
 * Año 2023
 * 
 * Alumnos: Tomás BRULA, Dylan FRIGERIO
 * 
 */

#include "ADC.h"

enum{
	B8 = 0,
	B12,
	B10,
	B16
};

/*
 *	Funcion: ADC_Init
 * ----------------------------
 *	Inicializa un modulo ADC. 
 *
 *  channel: elijo que canal usar | 0 -> a | 1 -> b
 *  coversion: elijo de cuantos bits hacer la conversión | 0 -> 8bits| 1 -> 12bits | 2 -> 10bits | 3 -> 16bits
 *	returns: nada
 */
void ADC_Init(uint8_t channel,	uint8_t conversion){
	//Activo el ADC
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
	//Registrosdel ADC
	switch(conversion){
		case B8:
			//ADC0_CFG1 = 0b11010100; /Low power, fBus / 4, long sample, ADC de 8 bits, bus clock (la fmax del ADC son 6MHz en este caso)
			ADC0_CFG1 = ADC_CFG1_ADLPC_MASK | (0b10<< ADC_CFG1_ADIV_SHIFT) | ADC_CFG1_ADLSMP_MASK |
			(0b00 << ADC_CFG1_MODE_SHIFT) | (0 * ADC_CFG1_ADICLK_MASK);
			break;
		case B12:
			//ADC0_CFG1 = 0b11010100; /Low power, fBus / 4, long sample, ADC de 12 bits, bus clock (la fmax del ADC son 6MHz en este caso)
			ADC0_CFG1 = ADC_CFG1_ADLPC_MASK | (0b10<< ADC_CFG1_ADIV_SHIFT) | ADC_CFG1_ADLSMP_MASK |
			(0b01 << ADC_CFG1_MODE_SHIFT) | (0 * ADC_CFG1_ADICLK_MASK);
			break;
		case B10:
			//ADC0_CFG1 = 0b11010100; /Low power, fBus / 4, long sample, ADC de 12 bits, bus clock (la fmax del ADC son 6MHz en este caso)
			ADC0_CFG1 = ADC_CFG1_ADLPC_MASK | (0b10<< ADC_CFG1_ADIV_SHIFT) | ADC_CFG1_ADLSMP_MASK |
			(0b10 << ADC_CFG1_MODE_SHIFT) | (0 * ADC_CFG1_ADICLK_MASK);
			break;
		case B16:
			//ADC0_CFG1 = 0b11010100; /Low power, fBus / 4, long sample, ADC de 12 bits, bus clock (la fmax del ADC son 6MHz en este caso)
			ADC0_CFG1 = ADC_CFG1_ADLPC_MASK | (0b10<< ADC_CFG1_ADIV_SHIFT) | ADC_CFG1_ADLSMP_MASK |
			(0b11 << ADC_CFG1_MODE_SHIFT) | (0 * ADC_CFG1_ADICLK_MASK);
			break;
	}		
	if(channel == 0)
		//ADC0_CFG2 = 0b00100; //Canales A, asynchronous clock disabled, highspeed, longestsample
		ADC0_CFG2 = (0 * ADC_CFG2_MUXSEL_MASK) | (0 * ADC_CFG2_ADACKEN_MASK) | ADC_CFG2_ADHSC_MASK | (0 * ADC_CFG2_ADLSTS_MASK);
	else if(channel == 1)
		//ADC0_CFG2 = 0b00100; //Canales A, asynchronous clock disabled, highspeed, longestsample
		ADC0_CFG2 = (1 * ADC_CFG2_MUXSEL_MASK) | (0 * ADC_CFG2_ADACKEN_MASK) | ADC_CFG2_ADHSC_MASK | (0 * ADC_CFG2_ADLSTS_MASK);
	//ADC0_SC2 = 0b000000000; //Compare function disabled, DMA is disabled
	ADC0_SC2 = 0;
	//ADC0_SC3 = 0b000000000; //uncalibrated, only one conversion, hardware average disabled.
	ADC0_SC3 = (0 * ADC_SC3_ADCO_MASK) | (0 * ADC_SC3_AVGE_MASK) | (0 * ADC_SC3_AVGS_MASK);
	//ADC0_SC1A = 0b01000000; //con ISR, single ended, canal 0 (código binario de los últimos 5 bits)
}

/*
 *	Funcion: ADC_Channel
 * ----------------------------
 *	Inicializa un pin como ADC. 
 *
 *  pin: elijo que pin usar | 0 - 31 (apunte)
 *  continuous: elijo de que manera convertir | 0 -> por interrupción | 1 -> continua
 *	diff: elji si activar o no el diferencial | 0 -> no diferencial | 1 -> diferencial
 *	returns: nada
 */
void ADC_Channel(uint8_t pin, uint8_t continuous, uint8_t diff){
	ADC0_SC1A = continuous * ADC_SC1_AIEN_MASK | (diff * ADC_SC1_DIFF_MASK) | ADC_SC1_ADCH(pin);
}

/*
 *	Funcion: ADC_Ready
 * ----------------------------
 *	Lee el pin de ADC. 
 *
 *  pin: elijo que pin usar | 0 - 31 (apunte)
 *  continuous: elijo de que manera convertir | 0 -> por interrupción | 1 -> continua
 *	diff: elji si activar o no el diferencial | 0 -> no diferencial | 1 -> diferencial
 *	returns: valor del ADC
 */
uint16_t ADC_Read(uint8_t pin, uint8_t continuous, uint8_t diff){
	uint16_t valor = 0;
	if(continuous == 0){
		if((ADC0_SC1A & ADC_SC1_COCO_MASK)!=0){
			valor = ADC0_RA;
			ADC0_SC1A = continuous * ADC_SC1_AIEN_MASK | (diff * ADC_SC1_DIFF_MASK) | ADC_SC1_ADCH(pin);
		}
	}
	if(continuous == 1){
		if((ADC0_SC1A & ADC_SC1_COCO_MASK)!=0)
			valor = ADC0_RA;
	}
	return  valor;
}