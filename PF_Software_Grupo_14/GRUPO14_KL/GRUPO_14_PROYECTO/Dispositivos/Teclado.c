/*
 * ------------------------------
 * Teclado.c (para el MKL25Z)
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
 
#include "Teclado.h" 

uint8_t estadoTeclado = C1;
uint16_t teclado = 0;
uint16_t teclado_ant = 0;
uint16_t teclado_sr = 0;
uint8_t estadoAntTeclado = 0;

void buttonPushed(){
	switch (estadoTeclado) {
		case C1:
			GPIO_Set(COLUMN1, LOW);		//columna1
			GPIO_Set(COLUMN2, HIGH);	//columna2
			GPIO_Set(COLUMN3, HIGH);	//columna3
			GPIO_Set(COLUMN4, HIGH);	//columna4
			
			if(GPIO_Read(ROW1) == 0)	// ZEROS
				teclado = ZEROS;
			else
				teclado &= ~ZEROS;
			if(GPIO_Read(ROW2) == 0)	// MM
				teclado = MM;
			else
				teclado &= ~MM;
			if(GPIO_Read(ROW3) == 0)	// CM
				teclado = CM;
			else
				teclado &= ~CM;
			if(GPIO_Read(ROW4) == 0)	// CM
				teclado = RST;
			else
				teclado &= ~RST;
							
			
			break;
			
		case C2:
			GPIO_Set(COLUMN1, HIGH);	//columna1
			GPIO_Set(COLUMN2, LOW);		//columna2
			GPIO_Set(COLUMN3, HIGH);	//columna3
			GPIO_Set(COLUMN4, HIGH);	//columna4
			
			if(GPIO_Read(ROW2) == 0)	// LEFT
				teclado = LEFT;
			else
				teclado &= ~LEFT;
			if(GPIO_Read(ROW4) == 0)	// CANCEL
				teclado = CANCEL;
			else
				teclado &= ~CANCEL;
	
			break;
			
		case C3:
			GPIO_Set(COLUMN1, HIGH);	//columna1
			GPIO_Set(COLUMN2, HIGH);	//columna2
			GPIO_Set(COLUMN3, LOW);		//columna3
			GPIO_Set(COLUMN4, HIGH);	//columna4
			
			if(GPIO_Read(ROW1) == 0)	// UP
				teclado = UP;
			else
				teclado &= ~UP;
			if(GPIO_Read(ROW2) == 0)	// OK
				teclado = OK;
			else
				teclado &= ~OK;
			if(GPIO_Read(ROW3) == 0)	// DOWN
				teclado = DOWN;
			else
				teclado &= ~DOWN;
			
			if(GPIO_Read(ROW4) == 0)	// STOP
				teclado = STOP;
			else
				teclado &= ~STOP;
			
			break;
			
		case C4:
			GPIO_Set(COLUMN1, HIGH);	//columna1
			GPIO_Set(COLUMN2, HIGH);	//columna2
			GPIO_Set(COLUMN3, HIGH);	//columna3
			GPIO_Set(COLUMN4, LOW);		//columna4
		
		if(GPIO_Read(ROW2) == 0)	// RIGTH
				teclado = RIGHT;
		else
				teclado &= ~RIGHT;
		if(GPIO_Read(ROW4) == 0)	// GO_ON
				teclado = GO_ON;
		else
				teclado &= ~GO_ON;
			
			break;
	}
	
	switch(estadoAntTeclado){
		case 0:
			teclado_sr = 0;
			if (teclado_ant != teclado){
				tiempo_teclado = 20;
				teclado_ant = teclado;
				estadoAntTeclado = 1;
			}
			break;
		case 1:
			if(tiempo_teclado == 0)
				estadoAntTeclado = 2;
			break;
		case 2:
			if(teclado_ant == teclado){
				teclado_sr = teclado;
				estadoAntTeclado = 0;
			}
			else
				estadoAntTeclado = 0;
			break;
	}
}