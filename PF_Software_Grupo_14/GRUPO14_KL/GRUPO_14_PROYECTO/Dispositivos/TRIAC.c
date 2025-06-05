/*
 * ------------------------------
 * TRIAC.C (para el MKL25Z)
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
 * Alumnos: Matías ALMARAZ, Tomás BRULA, Dylan FRIGERIO, AgustíN VARGAS
 * 
 */


#include "TRIAC.h"
		
uint8_t ME_Triac = 0;
uint32_t flanco_Triac = 0;
uint32_t velocidad_Aguja = 0;
uint8_t habilitacion_Motor = 0;
uint8_t semiciclos = 0;

void motor_Maquina(int velocidad_Aguja_Requerida){
	
	char velocidad = 0;
	
	velocidad = velocidad_Aguja_Requerida;
	
	switch(velocidad){
	
		case 0:
			velocidad_Aguja = 8;
			break;
		
		case 1:
			velocidad_Aguja = 7;
			break;
		
		case 2:
			velocidad_Aguja = 6;
			break;
		
		case 3:
			velocidad_Aguja = 5;
			break;

		case 4:
			velocidad_Aguja = 4;
			break;

		case 5:
			velocidad_Aguja = 3;
			break;
		
		case 6:
			velocidad_Aguja = 2;
			break;
		
		case 7:
			velocidad_Aguja = 1;
			break;
	}
	cruce_Por_Ceros_Y_Velocidad();
}

void control_Inercia(int velocidad_requerida){
	
		if(CNY70_Read() == 1){
			motor_Maquina(7);
		}
		
		else{
			motor_Maquina(velocidad_requerida);
		}
}

void cruce_Por_Ceros_Y_Velocidad(){
	
	switch(ME_Triac){
	
			case ESPERA_HIGH:
				if(flanco_Triac == 1){
					ME_Triac = ON;
					tiempo_Triac = 0;
					habilitacion_Triac = 1;
				}
				break;
				
			case ON:
				if(tiempo_Triac >= velocidad_Aguja){
					GPIO_Set(GPIOD, 6, HIGH);
					if(tiempo_Triac >= (velocidad_Aguja + 1)){
						ME_Triac = OFF;
						GPIO_Set(GPIOD, 6, LOW);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
						habilitacion_Triac = 0;
					}
				}
				break;
				
			case OFF:
				if(flanco_Triac == 0){
					ME_Triac = ESPERA_LOW;
					tiempo_Triac = 0;
					habilitacion_Triac = 1;
				}
				break;
				
			case ESPERA_LOW:
				if(tiempo_Triac >= velocidad_Aguja){
					GPIO_Set(GPIOD, 6, HIGH);
					if(tiempo_Triac >= (velocidad_Aguja + 1)){
						ME_Triac = ESPERA_HIGH;
						GPIO_Set(GPIOD, 6, LOW);
						habilitacion_Triac = 0;
					}
				}
				break;
				
			default:
				ME_Triac = ESPERA_HIGH;
				break;
		}
}

void subir_Aguja(){
	while(1){
		if(t_CNY70 >= 1500){
			t_CNY70 = 0;
			flag_t_CNY70 = 0;
			break;
		}
		
		if(GPIO_Read(CNY70) == 1){
			motor_Maquina(1);
			flag_t_CNY70 = 0;
			t_CNY70 = 0;
		}
		else
			flag_t_CNY70 = 1;
	}	
}

void PORTD_IRQHandler(void){	
		
		flanco_Triac = GPIO_Read(GPIOD, 7);
		if(semiciclos++ > 14)
			semiciclos = 0;
		PORTD_PCR7 |= PORT_PCR_ISF_MASK;
}
