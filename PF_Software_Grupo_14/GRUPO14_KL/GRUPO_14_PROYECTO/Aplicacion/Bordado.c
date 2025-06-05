/*
 * ------------------------------
 * Bordado.c (para el MKL25Z)
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
 
#include "Bordado.h"
#include "UART.h"

uint8_t start = 0;
uint8_t auxCheck = 0;

void startEmbroider(char *ptr, uint8_t aux){
	if(UART_Diff_EscLec()){
		switch(estadoRxTx){
			case RX:
				start = 1;
				protocolo_me(0, wUart[rLength]);
				rLength++;
				
				if(rLength >= LENGTH_UART)
					rLength = 0;
				
				if(auxTX >= 50){
					estadoRxTx = 1;
					rLength--;
				}
				break;
			case TX:
				envioTramas(ptr);
				
				auxTX = 0;
				estadoRxTx = 0;
				rLength++;
				break;
			case ENDRX:
				if(aux){
					for(uint16_t i = 0; i <= LENGTH_UART; i++)
						wUart[i] = 0;
					start = 0;
					rLength = 0;
					wLength = 0;
					auxTX = 0;
					endWarning();
					screenMain();
					position.x += position.offsetx;
					position.y += position.offsety;
					needle(48+(position.x*80/730), 43+(position.y*80/730), ILI9341_RED);
					drawFrame(48, 43, 105, 105, ILI9341_BLACK, 0, 5);
					estadoRxTx = 0;
					cant_JS = 0;
				}
				save = 0;
				cant_comandos = 0;
				auxCheck = 1;
				for(uint8_t i = 0; i < 20; i++)
					coloresImagen[i] = -1;
				
				for(uint16_t i = 0; i < RAM/2; i++)
					bufferDisplay[i] = 0XFFFF;
				break;
				
			default:
				estadoRxTx = RX;
				break;
		}
	}
}

void envioTramas(char* ptr){
	auxRX = wLength;
	while(wLength == auxRX){
		for(char i = 0; i < 3; i++){
			if(UART_TX_Ready(0))
				UART0_D = ptr[i];
			else 
				i--;
		}
		for(uint16_t i = 0; i < 10000; i++){
			if(wLength != auxRX)
				break;
		}
	}
}

void screenMain(){
	ILI9341_printPixel(0, 0,ILI9341_WHITE, 240, 320);
	drawFrame(10, 10, 180, 220, ILI9341_BLACK, 0, -1);
	drawCircle(100, 95, 80, ILI9341_BLACK, 0);
	drawFrame(10, 190, 180, 40, ILI9341_BLACK, 0, -1);
	ILI9341_drawString(165, 13, ILI9341_BLACK, "x1", 4, 3);
	ILI9341_drawString(165, 155, ILI9341_BLACK, "mm", 4, 3);
	ILI9341_drawString(198, 30, ILI9341_BLACK, "X:00", 4, 3);
	ILI9341_drawString(198, 115, ILI9341_BLACK, "Y:00", 4, 3);
	
	drawFrame(200, 10, 110, 40, ILI9341_BLACK, 0, -1);
	ILI9341_drawString(18, 215, ILI9341_BLACK, "OK", 4, 3);
	drawCheckbox(265, 15, 30, 30, ILI9341_BLACK, ILI9341_DARKGREEN, 0);
	
	drawFrame(200, 55, 110, 175, ILI9341_BLACK, 0, -1);
}

void needle(uint16_t x, uint16_t y, uint16_t color){
	static xy_t positionAnt = {0,0};
	
	ILI9341_drawPixel(positionAnt.y, positionAnt.x, ILI9341_WHITE);

	for(uint8_t i = 1; i < 8; i++){
		ILI9341_drawPixel(positionAnt.y+i, positionAnt.x, ILI9341_WHITE);
		ILI9341_drawPixel(positionAnt.y-i, positionAnt.x, ILI9341_WHITE);
		ILI9341_drawPixel(positionAnt.y, positionAnt.x+i, ILI9341_WHITE);
		ILI9341_drawPixel(positionAnt.y, positionAnt.x-i, ILI9341_WHITE);
	}
	
	char ptrxAnt [6]= {'X',':', 39, 39, '\0'};
	char ptryAnt [6]= {'Y',':', 39, 39, '\0'};
	char ptrx [5] = {'X',':', position.x/100+48, (position.x%100)/10+48, '\0'};
	char ptry [5] = {'Y',':', position.y/100+48, (position.y%100)/10+48, '\0'};
	
	if(x != positionAnt.x){
		ILI9341_drawString(198, 30, ILI9341_WHITE, ptrxAnt, 4, 3);
		ILI9341_drawString(198, 30, ILI9341_BLACK, ptrx, 4, 3);
	}
	if(y != positionAnt.y){
		ILI9341_drawString(198, 115, ILI9341_WHITE, ptryAnt, 4, 3);
		ILI9341_drawString(198, 115, ILI9341_BLACK, ptry, 4, 3);
		
	}
	
	drawFrame(48, 43, 105, 105, ILI9341_BLACK, 0, 5);
	
	
	ILI9341_drawPixel(y, x, color);
	
	for(uint8_t i = 2; i < 8; i++){
		ILI9341_drawPixel(y+i, x, color);
		ILI9341_drawPixel(y-i, x, color);
		ILI9341_drawPixel(y, x+i, color);
		ILI9341_drawPixel(y, x-i, color);
	}
	positionAnt.x = x;
	positionAnt.y = y;
}

void changeColorWarning(uint16_t color){
	save = 0;
	display.centerx = 0;
	display.centery = 0;
	drawFrame(110, 100, 100, 40, ILI9341_BLACK, 0, -1);
	drawFrame(111, 101, 98, 38, ILI9341_BLACK, 0, -1);
	drawFrame(112, 102, 96, 36, ILI9341_BLACK, 0, -1);
	drawFrame(113, 103, 94, 34, ILI9341_WHITE, 1, -1);
	drawTriangle(124, 105, 30, ILI9341_BLACK, 0);
	drawTriangle(125, 106, 28, ILI9341_YELLOW, 1);
	ILI9341_drawLetter(108, 135, ILI9341_BLACK, '!', 4, 3);
	
	drawCircle(180, 119, 15, ILI9341_BLACK, 0);
	drawCircle(180, 119, 14, color, 1);
	warningColor = 1000;
	while(teclado_sr != OK){
		if(warningColor == 999){
			drawFrame(110, 100, 100, 40, ILI9341_RED, 0, -1);
			drawFrame(111, 101, 98, 38, ILI9341_RED, 0, -1);
			drawFrame(112, 102, 96, 36, ILI9341_RED, 0, -1);
		}
		else if(warningColor == 500){
			drawFrame(110, 100, 100, 40, ILI9341_BLACK, 0, -1);
			drawFrame(111, 101, 98, 38, ILI9341_BLACK, 0, -1);
			drawFrame(112, 102, 96, 36, ILI9341_BLACK, 0, -1);
		}
		else if(warningColor == 0)
			warningColor = 1000;
	}
}

void endWarning(){
	subir_Aguja();
	save = 0;
	display.centerx = 0;
	display.centery = 0;
	drawFrame(110, 100, 100, 40, ILI9341_BLACK, 0, -1);
	drawFrame(111, 101, 98, 38, ILI9341_BLACK, 0, -1);
	drawFrame(112, 102, 96, 36, ILI9341_BLACK, 0, -1);
	drawFrame(113, 103, 94, 34, ILI9341_WHITE, 1, -1);
	drawTriangle(114, 105, 30, ILI9341_BLACK, 0);
	drawTriangle(115, 106, 28, ILI9341_YELLOW, 1);
	ILI9341_drawLetter(108, 125, ILI9341_BLACK, '!', 4, 3);
	
	ILI9341_drawString(110, 150, ILI9341_BLACK, "FIN", 4, 4);
	warningColor = 1000;
	while(teclado_sr != OK){
		if(warningColor == 999){
			drawFrame(110, 100, 100, 40, ILI9341_RED, 0, -1);
			drawFrame(111, 101, 98, 38, ILI9341_RED, 0, -1);
			drawFrame(112, 102, 96, 36, ILI9341_RED, 0, -1);
		}
		else if(warningColor == 500){
			drawFrame(110, 100, 100, 40, ILI9341_BLACK, 0, -1);
			drawFrame(111, 101, 98, 38, ILI9341_BLACK, 0, -1);
			drawFrame(112, 102, 96, 36, ILI9341_BLACK, 0, -1);
		}
		else if(warningColor == 0)
			warningColor = 1000;
	}
}

void rangeWarning(){
	save = 0;
	ILI9341_printPixel(0, 0,ILI9341_WHITE, 240, 320);
	display.centerx = 0;
	display.centery = 0;
	drawFrame(10, 10, 300, 220, ILI9341_BLACK, 0, -1);
	drawFrame(11, 11, 298, 218, ILI9341_BLACK, 0, -1);
	drawFrame(12, 12, 296, 216, ILI9341_BLACK, 0, -1);
	drawFrame(13, 13, 294, 214, ILI9341_BLACK, 0, -1);
	drawFrame(14, 14, 292, 212, ILI9341_BLACK, 0, -1);
	
	drawTriangle(35, 70, 100, ILI9341_BLACK, 0);
	drawTriangle(36, 71, 98, ILI9341_YELLOW, 1);
	ILI9341_drawLetter(80, 73, ILI9341_BLACK, '!', 14, 8);
	
	ILI9341_drawString(80, 170, ILI9341_BLACK, "FUERA", 4, 4);
	ILI9341_drawString(110, 170, ILI9341_BLACK, "DE", 4, 4);
	ILI9341_drawString(140, 170, ILI9341_BLACK, "ESCALA", 4, 4);
	warningColor = 1000;
	while(teclado_sr != OK){
		if(warningColor == 999){
			drawFrame(10, 10, 300, 220, ILI9341_RED, 0, -1);
			drawFrame(11, 11, 298, 218, ILI9341_RED, 0, -1);
			drawFrame(12, 12, 296, 216, ILI9341_RED, 0, -1);
			drawFrame(13, 13, 294, 214, ILI9341_RED, 0, -1);
			drawFrame(14, 14, 292, 212, ILI9341_RED, 0, -1);
		}
		else if(warningColor == 500){
			drawFrame(10, 10, 300, 220, ILI9341_BLACK, 0, -1);
			drawFrame(11, 11, 298, 218, ILI9341_BLACK, 0, -1);
			drawFrame(12, 12, 296, 216, ILI9341_BLACK, 0, -1);
			drawFrame(13, 13, 294, 214, ILI9341_BLACK, 0, -1);
			drawFrame(14, 14, 292, 212, ILI9341_BLACK, 0, -1);
		}
		else if(warningColor == 0)
			warningColor = 1000;
	}
	
	ILI9341_printPixel(0, 0,ILI9341_WHITE, 240, 320);
	screenMain();
	needle(48+(position.x*80/730), 43+(position.y*80/730), ILI9341_RED);
}

void cancelWarning(){
	uint8_t aux = 0;
	drawFrame(80, 86, 160, 62, ILI9341_BLACK, 0, -1);
	drawFrame(81, 87, 158, 60, ILI9341_WHITE, 1, -1);
	
	ILI9341_drawString(91, 85, ILI9341_BLACK, "SEGURO QUE DESEA", 2, 2);
	ILI9341_drawString(107, 85, ILI9341_BLACK, "BORRAR LOS DATOS", 2, 2);
	
	drawFrame(85, 122, 90, 20, ILI9341_BLACK, 0, -1);
	drawFrame(86, 123, 88, 18, ILI9341_LIGHTGREY, 1, -1);
	
	drawFrame(180, 122, 40, 20, ILI9341_BLACK, 0, -1);
	drawFrame(181, 123, 38, 18, ILI9341_LIGHTGREY, 1, -1);
	
	ILI9341_drawString(127, 90, ILI9341_BLACK, "CANCEL", 2, 2);
	ILI9341_drawString(127, 190, ILI9341_BLACK, "OK", 2, 2);
	
	while(1){
		if(teclado_sr == OK){
			aux = 0;
			break;
		}
		if(teclado_sr == CANCEL){
			aux = 1;
			break;
		}
	}
	
	if(aux){
		screenMain();
		needle(48+(position.x*80/730), 43+(position.y*80/730), ILI9341_RED);
		
		uint8_t auxColores = 0;
		for(uint8_t i = 0; coloresImagen[i] != -1; i++)
			auxColores++;
	
		uint8_t aux = 0;
		for(uint8_t i = 0; coloresImagen[i] != -1; i++){
			drawFrame(205, 60 + aux, 100, (173-10*auxColores)/auxColores, ILI9341_BLACK, 0, -1);
			drawFrame(206, 61 + aux, 98, ((173-10*auxColores)/auxColores)-2, coloresImagen[i], 1, -1);
			aux += (173-10*auxColores)/auxColores + 10;
		}
		
		drawCheckbox(265, 15, 30, 30, ILI9341_BLACK, ILI9341_DARKGREEN, 1);
	}else{
		screenMain();
		needle(48+(position.x*80/730), 43+(position.y*80/730), ILI9341_RED);
		
		start = 0;
			
		habilitacion_Motor = 0;
		for(uint16_t i = 0; i < LENGTH_UART; i++)
			wUart[i] = 0;
		
		rLength = 0;
		wLength = 0;
		
		cant_comandos = 0;
		for(uint8_t i = 0; coloresImagen[i] != -1; i++)
			coloresImagen[i] = -1;
		
		envioTramas("$Y#");
	}
}