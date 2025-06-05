
/*
 * ------------------------------
 * main.c (para el MKL25Z)
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
/* ------------------------------- ACLARACION --------------------------------
	
	El control de los motores se realiza con un A4988. El motor 'X' es el del perfil inferior y el 'Y' el del superior.
	El progrma esta ajustado para que realice 1/4 de paso cada 1ms.
	El control se realiza a trasvés de comandos definidos por la aplicación de InkSpace.
	Se envían por UART desde el QT 101 comandos y cada vez que queden 50 comandos pide que le manden 50 más, comando que se envía "$ K #".
	La Uart está definida en 115200 baudrate, por interrupciones, cíclica y se almacenan en un vector con buffer de 1000 caracteres.
	Los comandos son:
		- "$ T XXX YYY #" (tamaño de la imagen).
		- "$ S XXX YYY #" (moverse bordando).
		- "$ J XXX YYY #" (moverse sin bordando).
		- "$ H CC #" 			(cambiar color de hilo).
		- "$ W #" 				(apagar el motor para dejar de bordar por un JUMP o un END).
		- "$ E #" 				(termino de bordar).
	Se utiliza un teclado matricial para movimiento manual del pantografo, para setear el  mov. a las coordenadas 0;0 moverse cada 1 mm y cada 1cm
	Tambien parar la máquina y continuar, setear un inicio de bordado y cancelar el bordado.
	La lectura del teclado se hará por interrupciones para poder frenar la maquina en cualquier momento.
	Se utiliza el CNY70 para identificar cuando la aguja esta arriba.
	Se utiliza el TRIAC para ajustar la velocidad del motor de la máquina de bordar, para subir la aguja y apagar el motor.
*/

/* ---------------------------------- PINES ---------------------------------- 
	ILI9341:
		PTC4 	-> CS ALT2
		PTC5 	-> SCK ALT2
		PTC6 	-> MOSI ALT2
		PTC7 	-> MISO ALT2
		PTC3 	-> DC/RS
		PTC0 	-> RST
		
	CNY70:
		PTE29 -> output CNY70
		
	TRIAC:
		PTD6  -> output TRIAC
		PTD7	-> CRUCE_POR_CEROS
		
	NEMA 17:
		PTE5  -> DIR motor X (D2)
		PTE4  -> STEP motor X (D4)
		PTE3  -> DIR motor Y (D3)
		PTE2  -> STEP motor Y (D5)
		
	FIN DE CARRERA:
		PTB9  -> fin de carrera X (posición 0)
		PTB8  -> fin de carrera Y (posición 0)
		
	TECLADO:
		PTD2  -> fila 1
		PTD3  -> fila 2
		PTD1  -> fila 3
		PTE0  -> fila 4
		PTD0  -> columna 1
		PTD5  -> columna 2
		PTC9  -> columna 3
		PTC8  -> columna 4
*/

#include "CNY70.h"
#include "TPM.h"
#include "TFT.h"
#include "ILI9341.h"
#include "Inicializacion.h"
#include "SysTick.h"
#include "Comandos.h"
#include "Teclado.h" 
#include "Bordado.h" 
#include "TRIAC.h"
#include "UART.h"

uint8_t movePrescaler = MM;
uint8_t bobinado = 0;
uint8_t auxCheckStop = 0;

int main(void){
	init_NEMAPin();
	init_FINESPin();
	init_TFTPin();
	ILI9341_Init();
	init_TRIACPin();
	init_CNY70Pin();
	init_TecladoPin();
		
	UART_initOverflow(0,115200);
		
	SysTick_Init(1);
	TPM_InitOverflow(0, 1000, 5);
	TPM_InitOverflow(1, 10000, 0);
	UART_RX_OverflowON(0);
	
	ILI9341_printPixel(0, 0,ILI9341_WHITE, 240, 320);
	
	envioTramas("$I#");
	while(teclado_sr != OK)
		startEmbroider("$P#", 0);
	
	NEMA17_positionStart();
		
	start = 0;
	auxTX = 0;
	for(uint16_t i = 0; i <= LENGTH_UART; i++)
		wUart[i] = 0;
	rLength = 0;
	wLength = 0;
	estadoRxTx = 0;
	
	screenMain();
	needle(48+(position.x*80/730), 43+(position.y*80/730), ILI9341_RED);
	
	for(uint16_t i = 0; i < RAM/2; i++)
			bufferDisplay[i] = 0XFFFF;
	
	for(;;){
		
		if(teclado_sr == ZEROS && start != 1){
			NEMA17_positionStart();
			needle(48+(position.x*80/730), 43+(position.y*80/730), ILI9341_RED);
		}
		
		if(teclado_sr == MM && movePrescaler != MM && start != 1){
			movePrescaler = MM;
			ILI9341_drawString(165, 13, ILI9341_WHITE, "x10", 4, 3);
			ILI9341_drawString(165, 13, ILI9341_BLACK, "x1", 4, 3);
		}
		
		else if(teclado_sr == CM && movePrescaler != CM && start != 1){
			movePrescaler = CM;
			ILI9341_drawString(165, 13, ILI9341_BLACK, "x10", 4, 3);
		}
		
		if(teclado_sr == LEFT && start != 1){
			NEMA17_movManual(LEFT, movePrescaler);
			needle(48+(position.x*80/730), 43+(position.y*80/730), ILI9341_RED);
		}
		else if(teclado_sr == RIGHT && start != 1){
			NEMA17_movManual(RIGHT, movePrescaler);
			needle(48+(position.x*80/730), 43+(position.y*80/730), ILI9341_RED);
		}
		else if(teclado_sr == UP && start != 1){
			NEMA17_movManual(UP, movePrescaler);
			needle(48+(position.x*80/730), 43+(position.y*80/730), ILI9341_RED);
		}
		else if(teclado_sr == DOWN && start != 1){
			NEMA17_movManual(DOWN, movePrescaler);
			needle(48+(position.x*80/730), 43+(position.y*80/730), ILI9341_RED);
		}
		if((teclado_sr == OK) && (wLength != 0) && (start != 1)){
			for(uint8_t i = 0; i < 9; i++){
				protocolo_me(0, wUart[rLength]);
				rLength++;
			}	
			
			if(NEMA17_newPositionStart(position.x, position.y, sizeimagen.x, sizeimagen.y)){
				ILI9341_printPixel(0, 0,ILI9341_WHITE, 240, 320);
				save = 1;
				drawFrame(1, 1, 238, 238, ILI9341_BLACK, 0, -1);
				drawVerticalProgressBar(250, 5, 200, 60, 0, 1, ILI9341_WHITE, 1);
				subir_Aguja();
				estadoRxTx = RX; 
				start = 1;
			}
			else{
				rangeWarning();
				auxCheck = 1;
				rLength = 0;
				cant_comandos = 0;
				if(movePrescaler == MM){
					ILI9341_drawString(165, 13, ILI9341_WHITE, "x10", 4, 3);
					ILI9341_drawString(165, 13, ILI9341_BLACK, "x1", 4, 3);
				}
				else
					ILI9341_drawString(165, 13, ILI9341_BLACK, "x10", 4, 3);
				
				char ptrxAnt [6]= {'X',':', 39, 39, '\0'};
				char ptryAnt [6]= {'Y',':', 39, 39, '\0'};
				char ptrx [5] = {'X',':', position.x/100+48, (position.x%100)/10+48, '\0'};
				char ptry [5] = {'Y',':', position.y/100+48, (position.y%100)/10+48, '\0'};

				ILI9341_drawString(198, 30, ILI9341_WHITE, ptrxAnt, 4, 3);
				ILI9341_drawString(198, 30, ILI9341_BLACK, ptrx, 4, 3);

				ILI9341_drawString(198, 115, ILI9341_WHITE, ptryAnt, 4, 3);
				ILI9341_drawString(198, 115, ILI9341_BLACK, ptry, 4, 3);
			}
		}
		
		if(teclado_sr == CANCEL && wLength >= 500){
			cancelWarning();
			auxCheck = 1;
			if(movePrescaler == MM){
				ILI9341_drawString(165, 13, ILI9341_WHITE, "x10", 4, 3);
				ILI9341_drawString(165, 13, ILI9341_BLACK, "x1", 4, 3);
			}
			else
				ILI9341_drawString(165, 13, ILI9341_BLACK, "x10", 4, 3);
			char ptrxAnt [6]= {'X',':', 39, 39, '\0'};
			char ptryAnt [6]= {'Y',':', 39, 39, '\0'};
			char ptrx [5] = {'X',':', position.x/100+48, (position.x%100)/10+48, '\0'};
			char ptry [5] = {'Y',':', position.y/100+48, (position.y%100)/10+48, '\0'};

			ILI9341_drawString(198, 30, ILI9341_WHITE, ptrxAnt, 4, 3);
			ILI9341_drawString(198, 30, ILI9341_BLACK, ptrx, 4, 3);

			ILI9341_drawString(198, 115, ILI9341_WHITE, ptryAnt, 4, 3);
			ILI9341_drawString(198, 115, ILI9341_BLACK, ptry, 4, 3);
		}
		
		if(start == 1)
			startEmbroider("$K#", 1);
		
		if(teclado_sr == GO_ON){
			subirStop = 0;
			start = 1;
		}
		
		if(wLength >= 500 && cant_comandos < 2){
			if(auxCheck){
				drawCheckbox(265, 15, 30, 30, ILI9341_BLACK, ILI9341_DARKGREEN, 1);
				envioTramas("$O#");
				auxCheck = 0;
			}
			protocolo_me(0, wUart[rLength]);
			rLength++;
		}
	}
}