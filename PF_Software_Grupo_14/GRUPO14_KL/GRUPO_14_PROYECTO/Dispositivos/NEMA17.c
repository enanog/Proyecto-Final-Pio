/*
 * ------------------------------
 * NEMA17.c (para el MKL25Z)
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
 
#include "NEMA17.h"

coordenadas_t position = {0,0};	// Medidas en mm x10^-1.

void NEMA17_changeDIR(uint8_t axis, uint8_t direction){
	if(axis == X)
		GPIO_Set(DIRX, direction);
	else if(axis == Y)
		GPIO_Set(DIRY, direction);
}

void NEMA17_moveContinuous(uint8_t axis, uint16_t cantMove){
	if(axis == X){
		for(uint16_t i = 0; i < cantMove; i++){
			while(velocidadX < MOTOR_VELOCIDAD);
			velocidadX = 0;
		}
	}
	else if(axis == Y){
		for(uint16_t i = 0; i < cantMove; i++){
			while(velocidadY < MOTOR_VELOCIDAD);
			velocidadY = 0; 
		}
	}
}

coordenadas_t NEMA17_toPosition(uint16_t moveTo_x, uint16_t moveTo_y){
	coordenadas_t moveToPosition;
	
	if(moveTo_x > position.x){
		moveToPosition.x = (uint16_t)((moveTo_x - position.x) / STEP);
		NEMA17_changeDIR(X, SENTIDO_ANTIHORARIO);
	}
	else{
		moveToPosition.x = (uint16_t)((position.x - moveTo_x) / STEP);
		NEMA17_changeDIR(X, SENTIDO_HORARIO);
	}
	
	if(moveTo_y > position.y){
		moveToPosition.y = (uint16_t)((moveTo_y - position.y) / STEP);
		NEMA17_changeDIR(Y, SENTIDO_HORARIO);
	}
	else{
		moveToPosition.y = (uint16_t)((position.y - moveTo_y) / STEP);
		NEMA17_changeDIR(Y , SENTIDO_ANTIHORARIO);
	}
	
	return moveToPosition;
}

void NEMA17_positionStart(){
	NEMA17_changeDIR(Y, SENTIDO_ANTIHORARIO);
	NEMA17_changeDIR(X, SENTIDO_HORARIO);
	
	subir_Aguja();
	
	while(GPIO_Read(FINX) != 0){
		NEMA17_moveContinuous(X, 1);
	}
	
	while(GPIO_Read(FINY) != 0){
		NEMA17_moveContinuous(Y, 1);
	}
	
	position.x = 0;
	position.y = 0;
}

uint8_t NEMA17_newPositionStart(uint16_t x, uint16_t y, uint16_t sizex, uint8_t sizey){
	if(x + sizeimagen.x < 900 && y + sizeimagen.y < 900){
		position.x = 0;
		position.y = 0;
		position.offsetx = x;
		position.offsety = y;
		return 1;
	}
	else
		return 0;
}

uint8_t NEMA17_movManual(uint16_t button, uint8_t movePrescaler){
	if((LEFT == button) && (GPIO_Read(FINY) != 0) && ((movePrescaler == MM && position.x >= 20) || (movePrescaler == CM && position.x >= 120))){
		NEMA17_changeDIR(X, SENTIDO_HORARIO);
		if(movePrescaler == MM){
			NEMA17_moveContinuous(X, MOVE_MM);
			position.x -= 10;
		}
		else if(movePrescaler == CM){
			NEMA17_moveContinuous(X, MOVE_CM);
			position.x -= 100;
		}
	}
	else if((RIGHT == button) && ((movePrescaler == MM && position.x <= 880) || (movePrescaler == CM && position.x <= 780))){
		NEMA17_changeDIR(X, SENTIDO_ANTIHORARIO);
		if(movePrescaler == MM){
			NEMA17_moveContinuous(X, MOVE_MM);
			position.x += 10;
		}
		else if(movePrescaler == CM){
			NEMA17_moveContinuous(X, MOVE_CM);
			position.x += 100;
		}
	}
	else if((UP == button) && (GPIO_Read(FINY) != 0) && ((movePrescaler == MM && position.y >= 20) || (movePrescaler == CM && position.y >= 110))){
		NEMA17_changeDIR(Y, SENTIDO_ANTIHORARIO);
		if(movePrescaler == MM){
			NEMA17_moveContinuous(Y, MOVE_MM);
			position.y -= 10;
		}
		else if(movePrescaler == CM){
			NEMA17_moveContinuous(Y, MOVE_CM);
			position.y -= 100;
		}
	}
	else if((DOWN == button) && ((movePrescaler == MM && position.y <= 880) || (movePrescaler == CM && position.y <= 780))){
		NEMA17_changeDIR(Y, SENTIDO_HORARIO);
		if(movePrescaler == MM){
			NEMA17_moveContinuous(Y, MOVE_MM);
			position.y += 10;
		}
		else if(movePrescaler == CM){
			NEMA17_moveContinuous(Y, MOVE_CM);
			position.y += 100;
		}
	}
	else
		return 0;
	return 1;
} 