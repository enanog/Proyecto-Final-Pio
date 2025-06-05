/*
 * -----------------------
 * GPIO.c (para el MKL25Z)
 * -----------------------
 * 
 * Materias:
 * Taller - Sistemas Electrónicos Digitales
 * Programación de Dispositivos Electrónicos
 * 
 * 5to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Revisión 3
 * Año 2022
 * 
 * Profesores: Tomás Zuran, Héctor Albano, Diego Codevilla
 * 
 */

#include "GPIO.h"

/*
 * Funcion: GPIO_Alt
 * ----------------------------
 *	Configura un pin en la alternativa deseada (verificar Reference Manual)
 *
 *	Parametros:
 *	port: registro de puerto (PORTA, PORTB, PORTC, PORTD, PORTE)
 *	pin: numero de pin
 *	alt: numero de alternativa deseada
 * 
 *	returns: nada
 */
void GPIO_Alt(PORT_Type * port, uint8_t pin, uint8_t alt)
{
	// Configuro el pin en la alternativa correspondiente
	PORT_PCR_REG(port, pin) = (PORT_PCR_REG(port, pin) & (~PORT_PCR_MUX_MASK)) | PORT_PCR_MUX(alt);
}


//Le indica al pin que lo voy a utilizar como GPIO
void GPIO_Init(GPIO_MemMapPtr portDir, uint8_t pin) {
	uint8_t port;
	uint32_t *base;
	port = (((uint32_t)portDir) - ((uint32_t)PTA_BASE_PTR)) / 0x40;
	SIM_SCGC5 |= (1 << (SIM_SCGC5_PORTA_SHIFT + port));
	base = (uint32_t*)((uint32_t)PORTA_BASE_PTR + (0x1000 * port) + (pin * 4));
	*base |= (1 << PORT_PCR_MUX_SHIFT);
}

//Configura al pin como entrada o salida (0 = Entrada, 1 = Salida, ver defines en GPIO.h)
void GPIO_Mode(GPIO_MemMapPtr port, uint8_t pin, uint8_t mode) //Setea si el puerto es entrada o salida
{
	if(mode)
	{
		port->PDDR |= (1 << pin);
	}
	else
	{
		port->PDDR &= ~(1 << pin);
	}
}

//Setea el valor del pin en 0 o en 1, si el mismo fue previamente configurado como SALIDA.
void GPIO_Set(GPIO_MemMapPtr port, uint8_t pin, uint8_t value)
{
	if(value)
	{
		port->PSOR |= (1 << pin);
	}
	else
	{
		port->PCOR |= (1 << pin);
	}
}

//Lee el valor que actualmente tiene un pin. El mismo tuvo que ser previamente configurado como entrada.
uint8_t GPIO_Read(GPIO_MemMapPtr port, uint8_t pin)
{
	if((port->PDIR & (1 << pin)) != 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//Habilita la resistencia de pull UP interna del pin, si el mismo fue previamente configurado como entrada.
void GPIO_EnablePullUpResistor(GPIO_MemMapPtr portDir, uint8_t pin)
{
	uint8_t port;
	uint32_t *base;
	port = (((uint32_t)portDir) - ((uint32_t)PTA_BASE_PTR)) / 0x40;
	base = (uint32_t*)((uint32_t)PORTA_BASE_PTR + (0x1000 * port) + (pin * 4));
	*base |= (1 << PORT_PCR_PE_SHIFT);
	*base |= (1 << PORT_PCR_PS_SHIFT);
}

//Deshabilita la resistencia de pull (sea up o down) interna del pin.
void GPIO_DisablePullResistor(GPIO_MemMapPtr portDir, uint8_t pin)
{
	uint8_t port;
	uint32_t *base;
	port = (((uint32_t)portDir) - ((uint32_t)PTA_BASE_PTR)) / 0x40;
	base = (uint32_t*)((uint32_t)PORTA_BASE_PTR + (0x1000 * port) + (pin * 4));
	*base &= ~(1 << PORT_PCR_PE_SHIFT);
}

void GPIO_InputPullUP(GPIO_MemMapPtr portDir, uint8_t pin){
	// Inicializo el pin como entrada
	GPIO_Init(portDir, pin);
	GPIO_Mode(portDir, pin, INPUT);
	
	// Inicializo R-PULL UP interna (50K)
	GPIO_EnablePullUpResistor(portDir, pin);
}
