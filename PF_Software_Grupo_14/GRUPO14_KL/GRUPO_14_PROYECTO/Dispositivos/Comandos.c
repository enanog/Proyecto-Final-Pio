/*
 * ------------------------------
 * Comando.c (para el MKL25Z)
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
 
#include "Comandos.h"

uint8_t motorBordadora = 1;

uint16_t cmd = ILI9341_RED;
int coloresImagen[20];
uint16_t comandosMax = 0; 

uint8_t auxColores = 0;

coordenadas_t sizeimagen = {0};

uint8_t toDec(char dato){
	if(dato >= '0' && dato <= '9')
		dato = dato - '0';
	else if(dato >= 'A' && dato <= 'F')
		dato = dato - 'A' + 10;
	
	return dato;
}


void lecturaDisplay(char *datos){
	uint16_t longitud;
	coordenadas_t inicio;
	coordenadas_t fin;
	
	// Convierte los datos de UART (en hexa) a decimales
	inicio.x = toDec(datos[0])*16*16 + toDec(datos[1])*16 + toDec(datos[2]);
	inicio.y= toDec(datos[3])*16 + + toDec(datos[4]);
	
	fin.x = toDec(datos[5])*16*16 + toDec(datos[6])*16 + toDec(datos[7]);
	fin.y = toDec(datos[8])*16 + + toDec(datos[9]);
	
	// Calcula la longitud de la línea a imprimir
	longitud = fin.x - inicio.x + 1;
	
	// Llama a una función para imprimir píxeles en el display
	ILI9341_printPixel(inicio.y, inicio.x, cmd, 1, longitud); // (y, x, color, pixel, longitud del pixel
}


void switchColorDisplay(char * datos){
	cmd = toDec(datos[0])*16*16*16 + toDec(datos[1])*16*16 + toDec(datos[2])*16 + toDec(datos[3]);
}

void sizeImage(char *datos){
	sizeimagen.x = toDec(datos[0])*16*16 + toDec(datos[1])*16 + toDec(datos[2]);
	sizeimagen.y = toDec(datos[3])*16*16 + toDec(datos[4])*16 + toDec(datos[5]);
}


// Mover la CNC sin bordar
void moveJump(char *datos){
	uint16_t penX, penY, penCant;
	
	coordenadas_t move, moveTo;
	
	// Se calculan las coordenadas de destino
	moveTo.x = toDec(datos[0])*16*16 + toDec(datos[1])*16 + toDec(datos[2]);
	moveTo.y = toDec(datos[3])*16*16 + toDec(datos[4])*16 + toDec(datos[5]);
	
	move = NEMA17_toPosition(moveTo.x, moveTo.y);
	habilitacion_Motor = 0;
	subir_Aguja();
	
	// Se calculan los valores de 'penX' y 'penY' para determinar la relación de pasos en X e Y
	if(move.x > move.y){
		if(move.y == 0){
			penY = 0;
			penX = 1;
			penCant = move.x;
		}
		else{
			penY = 1;
			penX = move.x/move.y;
			penCant = move.y;
			// Se realiza el resto para llegar al destino con un ángulo entero
			if(move.x%move.y != 0)
				NEMA17_moveContinuous(X, move.x%move.y);
		}
	}
	else if(move.x < move.y){
		if(move.x == 0){
 			penX = 0;
			penY = 1;
			penCant = move.y;
		}
		else{
			penX = 1;		
			penY = move.y/move.x;
			penCant = move.x;
			// Se realiza el resto para llegar al destino con un ángulo entero
			if(move.y%move.x != 0)
				NEMA17_moveContinuous(Y, move.y%move.x);
		}
	}
	else{
		penX = 1;
		penY = 1;
		penCant = move.x;
	}
	
	// Se realiza el movimiento en el eje X e Y en simultaneo
	for(uint16_t i = 0; i < penCant; i++){
		if(move.x != 0)
			NEMA17_moveContinuous(X, penX);
		if(move.y != 0)
			NEMA17_moveContinuous(Y, penY);
	}
	
	tiempoEndMove = MOTOR_VELOCIDAD*2;
	while(tiempoEndMove != 0);
	
	motorBordadora = 1;
	
	// Se actualiza la posición actual del objeto a las coordenadas de destino
	position.x = moveTo.x;
	position.y = moveTo.y;
	
	float a = 0;
	float b = 0;
	
	if(sizeimagen.x > sizeimagen.y){
		a = 1;
		b = (float)((sizeimagen.y * 1.0)/(sizeimagen.x * 1.0));
		display.centery = (234 - (234*b))/2;
		display.centerx = 3;
	}
	else if(sizeimagen.x < sizeimagen.y){
		a = (float)((sizeimagen.x * 1.0)/(sizeimagen.y * 1.0));
		b = 1;
		display.centerx = (234 - (234*a))/2;
		display.centery = 3;
	}
	else if(sizeimagen.x == sizeimagen.y){ 
		a = 1;
		b = 1;
		display.centerx = 3;
		display.centery = 3;
	}
	
	//ILI9341_printLine(display.x, display.y, cmd, (uint16_t)(moveTo.x*236*a/sizeimagen.x), (uint16_t)(moveTo.y*236*b/sizeimagen.y));
	
	display.x = (uint16_t)((moveTo.x*234*a)/(sizeimagen.x));
	display.y = (uint16_t)((moveTo.y*234*b)/(sizeimagen.y));
}

// Mover la CNC bordando
void moveStitch(char *datos){
	uint16_t penX, penY, penCant;
	
	coordenadas_t move, moveTo;
	
	// Se calculan las coordenadas de destino
	moveTo.x = toDec(datos[0])*16*16 + toDec(datos[1])*16 + toDec(datos[2]);
	moveTo.y = toDec(datos[3])*16*16 + toDec(datos[4])*16 + toDec(datos[5]);
	
	move = NEMA17_toPosition(moveTo.x, moveTo.y);
	
	habilitacion_Motor = 1;	
	
	// Se calculan los valores de 'penX' y 'penY' para determinar la relación de pasos en X e Y
	if(move.x > move.y){
		if(move.y == 0){
			penY = 0;
			penX = 1;
			penCant = move.x;
		}
		else{
			penY = 1;
			penX = move.x/move.y;
			penCant = move.y;
			// Se realiza el resto para llegar al destino con un ángulo entero
			if(move.x%move.y != 0)
				NEMA17_moveContinuous(X, move.x%move.y);
		}
	}
	else if(move.x < move.y){
		if(move.x == 0){
 			penX = 0;
			penY = 1;
			penCant = move.y;
		}
		else{
			penX = 1;		
			penY = move.y/move.x;
			penCant = move.x;
			// Se realiza el resto para llegar al destino con un ángulo entero
			if(move.y%move.x != 0)
				NEMA17_moveContinuous(Y, move.y%move.x);
		}
	}
	else{
		penX = 1;
		penY = 1;
		penCant = move.x;
	}
	// Se realiza el movimiento en el eje X e Y en simultaneo
	for(uint16_t i = 0; i < penCant; i++){
		if(move.x != 0)
			NEMA17_moveContinuous(X, penX);
		
		if(move.y != 0)
			NEMA17_moveContinuous(Y, penY);
	}
	// Se actualiza la posición actual del objeto a las coordenadas de destino
	
	tiempoEndMove = MOTOR_VELOCIDAD*2;
	while(tiempoEndMove != 0);
	
	position.x = moveTo.x;
	position.y = moveTo.y;			
	
	float a = 0;
	float b = 0;
	
	if(sizeimagen.x > sizeimagen.y){
		a = 1;
		b = (float)((sizeimagen.y * 1.0)/(sizeimagen.x * 1.0));
		display.centery = (234 - (234*b))/2;
		display.centerx = 3;
	}
	else if(sizeimagen.x < sizeimagen.y){
		a = (float)((sizeimagen.x * 1.0)/(sizeimagen.y * 1.0));
		b = 1;
		display.centerx = (234 - (234*a))/2;
		display.centery = 3;
	}
	else if(sizeimagen.x == sizeimagen.y){ 
		a = 1;
		b = 1;
		display.centerx = 3;
		display.centery = 3;
	}
	
	ILI9341_printLine(display.x, display.y, cmd, (uint16_t)(moveTo.x*234*a/sizeimagen.x), (uint16_t)(moveTo.y*234*b/sizeimagen.y));
	drawVerticalProgressBar(250, 5, 200, 60, cant_JS, comandosMax, cmd, 0);
	
	display.x = (uint16_t)((moveTo.x*234*a)/(sizeimagen.x));
	display.y = (uint16_t)((moveTo.y*234*b)/(sizeimagen.y));
}

void endEmbroider(){
	habilitacion_Motor = 0;
	estadoRxTx = ENDRX;
}

void switchColorEmbroider(char *datos){
	static uint8_t aux = 0;
	habilitacion_Motor = 0;
	subir_Aguja();
	switch(datos[0]){
		case 'A':
			if(datos[1] == 'M')
				cmd = ILI9341_YELLOW;
			else if(datos[1]== 'Z')
				cmd = ILI9341_BLUE;
			else if(datos[1]== 'O')
				cmd = ILI9341_NAVY;
			break;
		case 'B':
			if(datos[1]== 'L')
				cmd = ILI9341_WHITE;
			else if(datos[1]== '0')
				cmd = ILI9341_MAROON;
			break;
		case 'C':
			if(datos[1]== 'N')
				cmd = ILI9341_CYAN;
			else if(datos[1]== 'O')
				cmd = ILI9341_DARKCYAN;
			break;
		case 'G':
			if(datos[1]== 'C')
				cmd = ILI9341_LIGHTGREY;
			else if(datos[1]== 'O')
				cmd = ILI9341_DARKGREY;
			break;
		case 'M':
			if(datos[1]== 'G')
				cmd = ILI9341_MAGENTA;
			else if(datos[1]== 'N')
				cmd = ILI9341_BROWN;
			break;
		case 'N':
			if(datos[1]== 'G')
				cmd = ILI9341_BLACK;
			else if(datos[1]== 'J')
				cmd = ILI9341_ORANGE;
			break;
		case 'O':
			if(datos[1]== 'L')
				cmd = ILI9341_OLIVE;
			break;
		case 'P':
			if(datos[1]== 'R')
				cmd = ILI9341_PURPLE;
			break;
		case 'R':
			if(datos[1]== 'J')
				cmd = ILI9341_RED;
			else if(datos[1]== 'S')
				cmd = ILI9341_PINK;
			break;
		case 'V':
			if(datos[1]== 'D')
				cmd = ILI9341_GREEN;
			else if(datos[1]== 'A')
				cmd = ILI9341_GREENYELLOW;
			else if(datos[1]== 'O')
				cmd = ILI9341_DARKGREEN;
			break;
		
		/* Comando inexistente */
		default:
			break;
	}
	changeColorWarning(coloresImagen[aux]);
	
	ILI9341_printBuffer();
	save = 1;
	aux++;
	
	if(auxColores == aux)
		aux = 0;
}

void colorEmbroider(char *datos){
	for(uint8_t i = 0; i < 20; i++)
		coloresImagen[i] = -1;
	
	for(uint8_t i = 0; i < 20; i++){
		switch(datos[0 + 2*i]){
			case 'A':
				if(datos[1 + 2*i] == 'M')
					coloresImagen[i] = ILI9341_YELLOW;
				else if(datos[1 + 2*i]== 'Z')
					coloresImagen[i] = ILI9341_BLUE;
				else if(datos[1 + 2*i]== 'O')
					coloresImagen[i] = ILI9341_NAVY;
				break;
			case 'B':
				if(datos[1 + 2*i]== 'L')
					coloresImagen[i] = ILI9341_WHITE;
				else if(datos[1 + 2*i]== 'O')
					coloresImagen[i] = ILI9341_MAROON;
				break;
			case 'C':
				if(datos[1 + 2*i]== 'N')
					coloresImagen[i] = ILI9341_CYAN;
				else if(datos[1 + 2*i]== 'O')
					coloresImagen[i] = ILI9341_DARKCYAN;
				break;
			case 'G':
				if(datos[1 + 2*i]== 'C')
					coloresImagen[i] = ILI9341_LIGHTGREY;
				else if(datos[1 + 2*i]== 'O')
					coloresImagen[i] = ILI9341_DARKGREY;
				break;
			case 'M':
				if(datos[1 + 2*i]== 'G')
					coloresImagen[i] = ILI9341_MAGENTA;
				else if(datos[1 + 2*i]== 'N')
					coloresImagen[i] = ILI9341_BROWN;
				break;
			case 'N':
				if(datos[1 + 2*i]== 'G')
					coloresImagen[i] = ILI9341_BLACK;
				else if(datos[1 + 2*i]== 'J')
					coloresImagen[i] = ILI9341_ORANGE;
				break;
			case 'O':
				if(datos[1 + 2*i]== 'L')
					coloresImagen[i] = ILI9341_OLIVE;
				break;
			case 'P':
				if(datos[1 + 2*i]== 'R')
					coloresImagen[i] = ILI9341_PURPLE;
				break;
			case 'R':
				if(datos[1 + 2*i]== 'J')
					coloresImagen[i] = ILI9341_RED;
				else if(datos[1 + 2*i]== 'S')
					coloresImagen[i] = ILI9341_PINK;
				break;
			case 'V':
				if(datos[1 + 2*i]== 'D')
					coloresImagen[i] = ILI9341_GREEN;
				else if(datos[1 + 2*i]== 'A')
					coloresImagen[i] = ILI9341_GREENYELLOW;
				else if(datos[1 + 2*i]== 'O')
					coloresImagen[i] = ILI9341_DARKGREEN;
				break;
			
			/* Comando inexistente */
			default:
				break;
		}
	}
	
	auxColores = 0;
	for(uint8_t i = 0; coloresImagen[i] != -1; i++)
		auxColores++;
	
	uint8_t aux = 0;
	for(uint8_t i = 0; coloresImagen[i] != -1; i++){
		drawFrame(205, 60 + aux, 100, (173-10*auxColores)/auxColores, ILI9341_BLACK, 0, -1);
		drawFrame(206, 61 + aux, 98, ((173-10*auxColores)/auxColores)-2, coloresImagen[i], 1, -1);
		aux += (173-10*auxColores)/auxColores + 10;
	}
}

void commandMax(char *datos){
	comandosMax = toDec(datos[0])*16*16*16 + toDec(datos[1])*16*16 + toDec(datos[2])*16 + toDec(datos[3]);
}