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

#include "UART.h"

uint16_t wLength = 0;
uint16_t rLength = 0;

uint8_t auxTX = 0;
uint16_t auxRX = 0; 
uint8_t estadoRxTx = 0;

char wUart[LENGTH_UART] = {0};

void UART_init(uint8_t uart, uint32_t baud){
	
	uint16_t baudrate = 0;
	
	UART_Type * uart_ptr[] = UART_BASE_PTRS;
	UART0_Type * uart_ptr0 = UART0_BASE_PTR;	
	
	SIM_SCGC4 |= 1 << (SIM_SCGC4_UART0_SHIFT + uart); // clock UART 0
	// selecciono clock del UART0 MCGIRCLK
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);

	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	// Configura pines limiando los flags de int pendientes y seleccionando los pines
	PORTA_PCR2 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR2 |= (PORT_PCR_ISF_MASK| (0x2 << PORT_PCR_MUX_SHIFT));
	PORTA_PCR1 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR1 |= (PORT_PCR_ISF_MASK| (0x2 << PORT_PCR_MUX_SHIFT));
	
	if(uart == 0){
	
		uart_ptr0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK); // Deshabilita la UART0

		uart_ptr0->C1 = 0;
		uart_ptr0->C4 = UART0_C4_OSR(15);
		
		// Cálculo los baudrate deseados
		baudrate = CLOCK_FREQ / ((uart_ptr0->C4+1)*baud);
		
		// Guardo los bits del 0 al 7
		uart_ptr0->BDL = UART0_BDL_SBR(baudrate & 0xFF);
		// Guardo los bits del 7 al 12
		uart_ptr0->BDH = UART0_BDH_SBR(baudrate >> 8);

		uart_ptr0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK); // Habilitamos Tx y Rx
	}
	
	else{
		uart_ptr[uart]->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK); // Deshabilita la UART0

		uart_ptr[uart]->C1 = 0;
		uart_ptr[uart]->C4 = UART0_C4_OSR(15);

		// Cálculo los baudrate deseados
		baudrate = BUS_FREQ / (16*baud);
		
		// Guardo los bits del 0 al 7
		uart_ptr[uart]->BDL = UART0_BDL_SBR(baudrate & 0xFF);
		// Guardo los bits del 7 al 12
		uart_ptr[uart]->BDH = UART0_BDH_SBR(baudrate >> 8);

		uart_ptr[uart]->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK); // Habilitamos Tx y Rx
	}
}


void UART_initOverflow(uint8_t uart, uint32_t baud){
	
	uint16_t baudrate = 0;
	
	UART_Type * uart_ptr[] = UART_BASE_PTRS;
	UART0_Type * uart_ptr0 = UART0_BASE_PTR;	
	
	SIM_SCGC4 |= 1 << (SIM_SCGC4_UART0_SHIFT + uart); // clock UART 0
	// selecciono clock del UART0 MCGIRCLK
	SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);

	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	// Configura pines limiando los flags de int pendientes y seleccionando los pines
	PORTA_PCR2 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR2 |= (PORT_PCR_ISF_MASK| (0x2 << PORT_PCR_MUX_SHIFT));
	PORTA_PCR1 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR1 |= (PORT_PCR_ISF_MASK| (0x2 << PORT_PCR_MUX_SHIFT));
	
	if(uart == 0){
	
		uart_ptr0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK); // Deshabilita la UART0

		uart_ptr0->C1 = 0;
		uart_ptr0->C4 = UART0_C4_OSR(15);
		
		// Cálculo los baudrate deseados
		baudrate = CLOCK_FREQ / ((uart_ptr0->C4+1)*baud);
		
		// Guardo los bits del 0 al 7
		uart_ptr0->BDL = UART0_BDL_SBR(baudrate & 0xFF);
		// Guardo los bits del 7 al 12
		uart_ptr0->BDH = UART0_BDH_SBR(baudrate >> 8);

		uart_ptr0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK); // Habilitamos Rx
		
		uart_ptr0->C2 |= UART0_C2_RIE_MASK;
	}
	
	else{
		uart_ptr[uart]->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK); // Deshabilita la UART0

		uart_ptr[uart]->C1 = 0;
		uart_ptr[uart]->C4 = UART0_C4_OSR(15);

		// Cálculo los baudrate deseados
		baudrate = BUS_FREQ / (16*baud);
		
		// Guardo los bits del 0 al 7
		uart_ptr[uart]->BDL = UART0_BDL_SBR(baudrate & 0xFF);
		// Guardo los bits del 7 al 12
		uart_ptr[uart]->BDH = UART0_BDH_SBR(baudrate >> 8);

		uart_ptr[uart]->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK); // Habilitamos Rx
		
		uart_ptr[uart]->C2 |= UART0_C2_RIE_MASK;
	}
	
	NVIC_EnableIRQ(UART0_IRQn + uart);
}

void UART0_IRQHandler(){
	wUart[wLength] = UART0_D;	// Recibo el caracter
	wLength++;
	
	if(wLength >= LENGTH_UART)						// Si es mayor lo devuelvo a 0 (ciclico)
		wLength = 0;
}

void UART_Transmit(uint8_t uart, char data){
	if(UART_TX_Ready(uart))
		UART0_D = data;
}

uint8_t UART_TX_Ready(uint8_t uart){
	switch(uart){
		case 0:
			if (UART0_S1 & UART0_S1_TDRE_MASK)
				return 1;
			break;
		case 1:
			if (UART1_S1 & UART0_S1_TDRE_MASK)
				return 1;
			break;
		case 2:
			if (UART2_S1 & UART0_S1_TDRE_MASK)
				return 1;
			break;
		default:
			break;
		}
	return 0;
}

char UART_Receive(uint8_t uart){
	char dato_rx = 0;
	
	dato_rx = UART0_D;
	
	return dato_rx;
}

uint8_t UART_RX_Ready(uint8_t uart){
	switch(uart){
		case 0:
			if (UART0_S1 & UART0_S1_RDRF_MASK)
				return 1;
			break;
		case 1:
			if (UART1_S1 & UART0_S1_RDRF_MASK)
				return 1;
			break;
		case 2:
			if (UART2_S1 & UART0_S1_RDRF_MASK)
				return 1;
			break;
		default:
			break;
		}
	return 0;
}

uint8_t UART_Diff_EscLec(){
	if(wLength != rLength)
		return 1;
	else
		return 0;
}

void UART_RX_OverflowOFF(uint8_t uart){
	UART_Type * uart_ptr[] = UART_BASE_PTRS;
	UART0_Type * uart_ptr0 = UART0_BASE_PTR;
	
	if(uart == 0)
		uart_ptr0->C2 &= ~UART0_C2_RIE_MASK;
	else
		uart_ptr[uart]->C2 &= ~UART0_C2_RIE_MASK;
}

void UART_RX_OverflowON(uint8_t uart){
	UART_Type * uart_ptr[] = UART_BASE_PTRS;
	UART0_Type * uart_ptr0 = UART0_BASE_PTR;
	
	if(uart == 0)
		uart_ptr0->C2 |= UART0_C2_RIE_MASK;
	else
		uart_ptr[uart]->C2 |= UART0_C2_RIE_MASK;
}