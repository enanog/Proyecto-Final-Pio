 /*
 * -----------------------
 * i2c.c
 * -----------------------
 * 
 * Materias:
 * Sistemas Electrónicos Embebidos
 * 
 * 6to Año Técnico en Electrónica
 * Colegio Pio IX
 * 
 * Revisión 1 Tomas Zurán
 * Revisión 2 Matías Marfisi
 * Año 2023
 * 
 * Profesores: Matías Marfisi, Tomás Zuran, Federico Diograzia
 * 
 */

#include "i2c.h"

#define i2c_DisableAck()       I2C0_C1 |= I2C_C1_TXAK_MASK

#define i2c_RepeatedStart()    I2C0_C1 |= I2C_C1_RSTA_MASK;

#define i2c_Start()            I2C0_C1 |= I2C_C1_TX_MASK;\
                               I2C0_C1 |= I2C_C1_MST_MASK

#define i2c_Stop()             I2C0_C1 &= ~I2C_C1_MST_MASK;\
                               I2C0_C1 &= ~I2C_C1_TX_MASK

#define i2c_EnterRxMode()      I2C0_C1 &= ~I2C_C1_TX_MASK;\
                               I2C0_C1 &= ~I2C_C1_TXAK_MASK      

#define i2c_write_byte(data)   I2C0_D = data

#define MWSR                   0x00  /* Master write slave read  */
#define MRSW                   0x01  /* Master read slave write */

static void I2C0_Wait(void);
static void I2C0_StartTransmission (uint8_t SlaveID, uint8_t Mode);

/*******************************************************************/
/*!
 * Inicializacion del modulo I2C
 * Configuro baudrate y prendo el modulo
 * Se configura en aproximadamente 20KHz con PTC8 como SCL y PTC9 como SDA
 */

void I2C0_Init(void) {
	/* MODIFICAR ACORDE A PIN A UTILIZAR */
	
//	SIM_SCGC5 = SIM_SCGC5_PORTC_MASK; // Habilito el puerto de los pines de comunicacion que voy a usar
//	PORTC_PCR8 = PORT_PCR_MUX(2);
//	PORTC_PCR9 = PORT_PCR_MUX(2);
	
	/* PTE24 y PTE25 */
	SIM_SCGC5 = SIM_SCGC5_PORTE_MASK; // Habilito el puerto de los pines de comunicacion que voy a usar
	PORTE_PCR24 = PORT_PCR_MUX(5);
	PORTE_PCR25 = PORT_PCR_MUX(5);

	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK; // Prendo el clock en el modulo I2C que voy a usar
	
	/* MODIFICAR ACORDE A FRECUENCIA A UTILIZAR, hoja 706 REFERENCE MANUAL */
	I2C0_F = I2C_F_ICR(0x20) | I2C_F_MULT(0b0); // Aproximadamente 20KHz       
	
	/* Habilito I2C */
	I2C0_C1 = I2C_C1_IICEN_MASK;
}

/*******************************************************************/
/*
 * Start I2C Transmision
 * El parametro I2C_Address es la direccion de 7 bits del esclavo al que me quiero comunicar
 * El parametro Mode define si voy a leer (MRSW) o escribir (MWSR)
 */
static void I2C0_StartTransmission(uint8_t I2C_Address, uint8_t Mode) {
	uint8_t MasterTransmission;
	
	if (Mode == MWSR) {
		/* configuro modo de transmision */
		MasterTransmission = MWSR;
	} else {
		/* configuro modo de transmision */
		MasterTransmission = MRSW;
	}

	/* Mando bit de start */
	i2c_Start();

	/* mando la I2C address con su bit de lectura o escritura */
	i2c_write_byte(I2C_Address << 1 | MasterTransmission);
}

/*******************************************************************/
/*
 * Rutina de pausa
 */
void Pause(void) {
	int n;
	for (n = 1; n < 50; n++) {
		__ASM("nop");
	}
}

/*******************************************************************/
/*
 * Lee un registro de un dispositivo I2C
 * El parametro I2C_Address es la direccion de 7 bits del esclavo al que me quiero comunicar
 * El parámetro u8RegisterAddress es la dirección del registro que quiero leer
 * La función devuelve el dato guardado en el registro
 */
uint8_t I2C0_ReadRegister(uint8_t I2C_Address, uint8_t u8RegisterAddress) {
	uint8_t result;

	/* Mando slave adress */
	I2C0_StartTransmission(I2C_Address, MRSW);
	I2C0_Wait();


	/* Mando cual registro quiero leer */
	I2C0_D = u8RegisterAddress;
	I2C0_Wait();

	/* Mando otro start */
	I2C0_C1 |= I2C_C1_RSTA_MASK;
	/* Mando slave adress */
	I2C0_D = (I2C_Address << 1) | 0x01; //read address
	I2C0_Wait();

	/* Configuro modo lectura */
	I2C0_C1 &= (~I2C_C1_TX_MASK);

	/* Desactivo ACK ya que ya se leyó la penultima lectura */
	I2C0_C1 |= I2C_C1_TXAK_MASK;

	/* Dummy read */
	result = I2C0_D;
	I2C0_Wait();

	/* Mando stop */
	i2c_Stop();

	/* Leo el dato */
	result = I2C0_D;

	return result;
}

/*******************************************************************/
/*
 * Escribe un byte de datos en el registro especificado
 * El parametro I2C_Address es la direccion de 7 bits del esclavo al que me quiero comunicar
 * El parámetro u8RegisterAddress es la dirección del registro que quiero escribir
 * El parámetro u8Data es el dato que se va a escribir
 */
void I2C0_WriteRegister(uint8_t I2C_Address, uint8_t u8RegisterAddress, uint8_t u8Data) {
	/* Mando los datos al slave */
	I2C0_StartTransmission(I2C_Address, MWSR);
	I2C0_Wait();

	I2C0_D = u8RegisterAddress;
	I2C0_Wait();

	I2C0_D = u8Data;
	I2C0_Wait();

	i2c_Stop();

	Pause();
}

/*******************************************************************/
/*
 * Escribe varios bytes de datos en el registro especificado
 * El parametro I2C_Address es la direccion de 7 bits del esclavo al que me quiero comunicar
 * El parámetro u8RegisterAddress es la dirección del registro que quiero escribir
 * El parámetro u8Data es el vector de datos que se va a escribir
 * El parámetro n_bytes es la cantidad de bytes que se van a escribir
 */

void I2C0_Write(uint8_t I2C_Address, uint8_t u8RegisterAddress, uint8_t * u8Data, uint8_t n_bytes) {
	/* Mando los datos al slave */
	I2C0_StartTransmission(I2C_Address, MWSR);
	I2C0_Wait();

	I2C0_D = u8RegisterAddress;
	I2C0_Wait();
	
	int i;
	for(i = 0; i < n_bytes; i++)
	{
		I2C0_D = u8Data[i];
		I2C0_Wait();
	}
	

	i2c_Stop();

	Pause();
}

/*******************************************************************/
/*
 * Lee un registro de más de un byte de tamaño de un dispositivo I2C
 * El parametro I2C_Address es la direccion de 7 bits del esclavo al que me quiero comunicar
 * El parámetro u8RegisterAddress es la dirección del registro que quiero leer
 * El parámetro datos es el vector de datos que se va a escribir
 * La función devuelve el dato guardado en el registro
 */
void I2C0_Read(uint8_t I2C_Address, uint8_t u8RegisterAddress, uint8_t * datos, uint8_t n_bytes) {
	uint8_t n = n_bytes;
	int i;

	/* Send Slave Address */
	I2C0_StartTransmission(I2C_Address, MWSR);
	I2C0_Wait();

	/* Write Register Address */
	I2C0_D = u8RegisterAddress;
	I2C0_Wait();

	/* Do a repeated start */
	I2C0_C1 |= I2C_C1_RSTA_MASK;

	/* Send Slave Address */
	I2C0_D = (I2C_Address << 1) | 0x01; //read address
	I2C0_Wait();

	/* Put in Rx Mode */
	I2C0_C1 &= (~I2C_C1_TX_MASK);

	/* Ensure TXAK bit is 0 */
	I2C0_C1 &= ~I2C_C1_TXAK_MASK;

	/* Dummy read */
	datos[0] = I2C0_D;
	I2C0_Wait();

	for (i = 0; i < n - 2; i++) {
		/* Read first byte */
		datos[i] = I2C0_D;
		I2C0_Wait();
	}
	/* Turn off ACK since this is second to last read*/
	I2C0_C1 |= I2C_C1_TXAK_MASK;

	/* Read second byte */
	datos[i++] = I2C0_D;
	I2C0_Wait();

	/* Send stop */
	i2c_Stop();

	/* Read third byte */
	datos[i++] = I2C0_D;

	Pause();
}

static void I2C0_Wait(void)
{
	//int espera = 1000000;
	while(((I2C0_S & I2C_S_IICIF_MASK)==0) /*&& espera != 0*/){/*espera--;*/}
	I2C0_S |= I2C_S_IICIF_MASK;
}
