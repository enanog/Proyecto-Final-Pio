 /*
 * -----------------------
 * SPI.c
 * -----------------------
 * 
 * Materias:
 * Sistemas Electrónicos Embebidos
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Revisión 1 Matías Marfisi
 * Año 2023
 * 
 * Profesores: Matías Marfisi, Tomás Zuran, Federico Diograzia
 * 
 */
 
#include "SPI.h"

static void SPI_Frequency(uint8_t spi, uint32_t hz);
static void SPI_Format(uint8_t spi, uint8_t polarity, uint8_t phase);

void SPI_Init(uint8_t spi, uint8_t polarity, uint8_t phase) {
	SPI_Type* spi_ptr[] = SPI_BASE_PTRS;
	
	/* Configuro pines a utilizar para cada spi*/
	switch(spi){
		case 0:
			/*
				SPI0:
				PTC4 -> CS ALT2
				PTC5 -> SCK ALT2
				PTC6 -> MOSI ALT2
				PTC7 -> MISO ALT2
			*/
			/* MODIFICAR ACORDE A PIN A UTILIZAR */
			SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; // Habilito el puerto de los pines de comunicacion que voy a usar
			PORTC_PCR4 = PORT_PCR_MUX(2);
			PORTC_PCR5 = PORT_PCR_MUX(2);
			PORTC_PCR6 = PORT_PCR_MUX(2);
			PORTC_PCR7 = PORT_PCR_MUX(2);
			break;
			
		case 1:
			/*
				SPI1:
				PTE1 -> MOSI ALT2
				PTE2 -> SCK ALT2
				PTE3 -> MISO ALT2
				PTE4 -> CS ALT2
			*/
			/* MODIFICAR ACORDE A PIN A UTILIZAR */
			SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK; // Habilito el puerto de los pines de comunicacion que voy a usar
			PORTE_PCR1 = PORT_PCR_MUX(2);
			PORTE_PCR2 = PORT_PCR_MUX(2);
			PORTE_PCR3 = PORT_PCR_MUX(2);
			PORTE_PCR4 = PORT_PCR_MUX(2);
			break;
	}

	/* Habilito clock SPI */
	SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK + spi;
	
	/* Habilito modulo SPI */
	spi_ptr[spi]->C1 |= SPI_C1_SPE_MASK;
	
	/* Habilito pin CS automatico */
	spi_ptr[spi]->C1 |= SPI_C1_SSOE_MASK;
	spi_ptr[spi]->C2 |= SPI_C2_MODFEN_MASK;
	
	/* Configuro polaridad y fase */
	SPI_Format(spi, polarity, phase);
	
	/* Configuro frecuencia */
	SPI_Frequency(spi, 12000000);
}

static void SPI_Format(uint8_t spi, uint8_t polarity, uint8_t phase) {
	SPI_Type* spi_ptr[] = SPI_BASE_PTRS;
	
	/* Selecciono modo Master y configuro polaridad y fase */
	uint8_t c1_data = ((1) << 4) | (polarity << 3) | (phase << 2);

	/* Clear MSTR, CPOL and CPHA bits */
	spi_ptr[spi]->C1 &= ~(0x7 << 2);
	
	/* Write new value */
	spi_ptr[spi]->C1 |= c1_data;
}

static void SPI_Frequency(uint8_t spi, uint32_t hz) {
	SPI_Type* spi_ptr[] = SPI_BASE_PTRS;
	
	uint32_t error = 0;
	uint32_t p_error = 0xffffffff;
	uint32_t ref = 0;
	uint8_t  spr = 0;
	uint8_t  ref_spr = 0;
	uint8_t  ref_prescaler = 0;

	uint32_t PCLK = 24000000; // Bus Clock 24MHz
	uint8_t prescaler = 1;
	uint8_t divisor = 2;

	for (prescaler = 1; prescaler <= 8; prescaler++) {
			divisor = 2;
			for (spr = 0; spr <= 8; spr++, divisor *= 2) {
					ref = PCLK / (prescaler*divisor);
					if (ref > (uint32_t)hz)
							continue;
					error = hz - ref;
					if (error < p_error) {
							ref_spr = spr;
							ref_prescaler = prescaler - 1;
							p_error = error;
					}
			}
	}
	/* Set SPPR and SPR */
	spi_ptr[spi]->BR = ((ref_prescaler & 0x7) << 4) | (ref_spr & 0xf);
}

static inline uint32_t SPI_Writeable(uint8_t spi) {
	SPI_Type* spi_ptr[] = SPI_BASE_PTRS;
  return (spi_ptr[spi]->S & SPI_S_SPTEF_MASK) ? 1 : 0;
}

static inline uint32_t SPI_Readable(uint8_t spi) {
	SPI_Type* spi_ptr[] = SPI_BASE_PTRS;
  return (spi_ptr[spi]->S & SPI_S_SPRF_MASK) ? 1 : 0;
}

uint32_t SPI_MasterWrite(uint8_t spi, uint8_t value) {
	SPI_Type* spi_ptr[] = SPI_BASE_PTRS;
	/* Wait tx buffer empty */
	while(!SPI_Writeable(spi));
	spi_ptr[spi]->D = (value & 0xff);

	/* Wait rx buffer full */
	while (!SPI_Readable(spi));
	return spi_ptr[spi]->D & 0xff;
}

uint32_t SPI_MasterRead(uint8_t spi) {
	SPI_Type* spi_ptr[] = SPI_BASE_PTRS;
	/* Wait tx buffer empty */
	while(!SPI_Writeable(spi));
	spi_ptr[spi]->D = (0xFF);

	/* Wait rx buffer full */
	while (!SPI_Readable(spi));
	return spi_ptr[spi]->D & 0xff;
}

uint32_t SPI_MasterBlockWrite(uint8_t spi, const char *tx_buffer, uint32_t tx_length, char *rx_buffer, uint32_t rx_length) {
	uint32_t total = (tx_length > rx_length) ? tx_length : rx_length;

	for (uint32_t i = 0; i < total; i++) {
			char out = (i < tx_length) ? tx_buffer[i] : 0xFF;
			char in = SPI_MasterWrite(spi, out);
			if (i < rx_length) {
					rx_buffer[i] = in;
			}
	}

	return total;
}
