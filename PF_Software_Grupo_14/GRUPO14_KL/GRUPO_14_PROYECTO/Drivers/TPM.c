/*
 * ----------------------
 * TPM.c (para el MKL25Z)
 * ----------------------
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

#include "TPM.h"

static const uint8_t tpm_prescaler[] = {
	1,
	2,
	4,
	8,
	16,
	32,
	64,
	128
};

/*
 * Funcion: TPM_InitOverflow
 * ----------------------------
 *	Inicializa un modulo TPM. Revisar si el valor de prescaler se ajusta a las necesidades de frecuencia.
 *
 *	     48MHz	
 *	---------------- < 65535 (maximo 16 bits)
 *	freq * prescaler
 *
 *	Parametros:
 *	tpm: número de TPM
 *	freq: frecuencia de interrupcion deseada
 *
 *	returns: nada
 */
void TPM_InitOverflow(uint8_t tpm, uint32_t freq, uint8_t prescaler)
{
	// Utilizo vector de punteros en MKL25Z4.h para seleccionar el registro de TPM correspondiente
	TPM_Type *tpm_ptr[] = TPM_BASE_PTRS;
	/*
	 tpm_ptr[0] -> TPM0
	 tpm_ptr[1] -> TPM1
	 tpm_ptr[2] -> TPM2
	 */
	// Habilito módulo TPM
	SIM->SCGC6 |= 1 << (SIM_SCGC6_TPM0_SHIFT + tpm);
	// Selecciono fuente de clock para TPM
	SIM->SOPT2 = (SIM->SOPT2 & (~SIM_SOPT2_TPMSRC_MASK)) | SIM_SOPT2_TPMSRC(1);
	// Habilito interrupciones, selecciono incrementos de clock y fijo prescaler en valor necesario (ajustar)
	TPM_SC_REG(tpm_ptr[tpm]) = TPM_SC_TOIE_MASK | TPM_SC_CMOD(1) | TPM_SC_PS(prescaler);
	// Fijo el modulo necesario a la frecuencia deseada
	TPM_MOD_REG(tpm_ptr[tpm]) = CLOCK_FREQ / (freq * tpm_prescaler[prescaler]);
	// Habilito interrupcion
	NVIC_EnableIRQ(TPM0_IRQn + tpm);
}

/*
 * Funcion: TPM_InitPWM
 * ----------------------------
 *	Inicializa un modulo TPM y un canal en PWM. Revisar si el valor de prescaler se ajusta a las necesidades de frecuencia.
 *	
 *	Parametros:
 *	tpm: número de TPM
 *	freq: frecuencia de interrupcion deseada
 *
 *	returns: nada
 */
void TPM_InitPWM(uint8_t tpm, uint8_t channel, uint32_t freq, uint8_t prescaler, uint8_t duty)
{
	TPM_Type *tpm_ptr[] = TPM_BASE_PTRS;

	// Habilito módulo TPM
	SIM->SCGC6 |= 1 << (SIM_SCGC6_TPM0_SHIFT + tpm);
	// Selecciono fuente de clock para TPM
	SIM->SOPT2 = (SIM->SOPT2 & (~SIM_SOPT2_TPMSRC_MASK)) | SIM_SOPT2_TPMSRC(1);
	// Selecciono incrementos de clock y fijo prescaler en valor necesario (ajustar)
	TPM_SC_REG(tpm_ptr[tpm]) = TPM_SC_CMOD(1) | TPM_SC_PS(prescaler);
	// Fijo el modulo necesario a la frecuencia deseada
	TPM_MOD_REG(tpm_ptr[tpm]) = CLOCK_FREQ / (freq * tpm_prescaler[prescaler]);
	// Configuro como Edge-Aligned PWM
	TPM_CnSC_REG(tpm_ptr[tpm], channel) = (1 * TPM_CnSC_MSB_MASK) 	| (0 * TPM_CnSC_MSA_MASK)
																	|(1 * TPM_CnSC_ELSB_MASK) | (0 * TPM_CnSC_ELSA_MASK);
	// Configuro duty
	TPM_CnV_REG(tpm_ptr[tpm], channel) = (TPM_MOD_REG(tpm_ptr[tpm]) * duty) / 100;
}


/*
 * Funcion: TPM_ChangeDuty
 * ----------------------------
 *	Permite modificar el valor de duty de un canal en particular
 *
 *	Parametros:
 *	tpm: número de TPM (verificar alternativa de pin)
 *	channel: canal de TPM (verificar alternativa de pin)
 *	duty: nuevo duty deseado
 *
 *	returns: nada
 */
void TPM_ChangeDuty(uint8_t tpm, uint8_t channel, uint8_t duty)
{
	TPM_Type *tpm_ptr[] = TPM_BASE_PTRS;
	// Configuro duty
	TPM_CnV_REG(tpm_ptr[tpm], channel) = (TPM_MOD_REG(tpm_ptr[tpm]) * duty) / 100;
}

int velocidadX = MOTOR_VELOCIDAD + 1;
int velocidadY = MOTOR_VELOCIDAD + 1;
int tiempoEndMove = 0;
int tiempoMotor = 0;
uint8_t subirStop = 0;

void TPM0_IRQHandler(void)
{
	// Atiendo la interrupcion
	TPM0->SC |= TPM_SC_TOF_MASK;	
	
	if(velocidadX < MOTOR_VELOCIDAD/2)
		GPIO_Set(STEPX, LOW);
	else if(velocidadX >= MOTOR_VELOCIDAD/2 && velocidadX < MOTOR_VELOCIDAD)
		GPIO_Set(STEPX, HIGH);
	else if(velocidadX > MOTOR_VELOCIDAD + 5)
		velocidadX = MOTOR_VELOCIDAD + 1;

	if(velocidadY < MOTOR_VELOCIDAD/2)
		GPIO_Set(STEPY, LOW);
	else if(velocidadY >= MOTOR_VELOCIDAD/2 && velocidadY < MOTOR_VELOCIDAD)
		GPIO_Set(STEPY, HIGH);
	else if(velocidadY > MOTOR_VELOCIDAD + 5)
		velocidadY = MOTOR_VELOCIDAD + 3; 
	
	if(tiempoEndMove > 0)
		tiempoEndMove--;
	
	velocidadX++;
	velocidadY++;
}

void TPM1_IRQHandler(void)
{
	// Atiendo la interrupcion
	TPM1->SC |= TPM_SC_TOF_MASK;
	if(semiciclos < 4 && habilitacion_Motor == 1 ){
		motor_Maquina(5);
	}
	
	if(teclado_sr == STOP)
			subirStop = 1;
	
	if(teclado_sr == RST)
		NVIC_SystemReset();
}

void TPM2_IRQHandler(void)
{
	// Atiendo la interrupcion
	TPM2->SC |= TPM_SC_TOF_MASK;
}