/*
 * ------------------------------
 * protocolo.c (para el MKL25Z)
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
#include "protocolo.h"

uint8_t cant_comandos = 0;
uint32_t cant_JS = 0;

/* Configuracion de la trama */
#define CARACTER_INICIO      	'$'
#define CARACTER_FIN         	'#'
#define LARGO_DATOS 					45

/* Comandos */


/* Control de habilitación de verificacion */
//#define VERIFICACION_RECEPCION

/* Funciones locales del archivo */
void funcion_comandos(char comando, char * datos);
void envio_trama(char comando, char * datos);

/* Funcion de verificacion mediante XOR de todos los datos ingresados */
char checksum_verif(char * data, int largo) // Paso el vector de datos
{
	char chksum = 0;
  // Hago la XOR de cada uno de los bytes de la trama
  for(int i = 0; i < largo; i++)
		chksum = chksum ^ data[i]; // XOR Byte a byte
    
	return chksum; // Devuelvo el byte de verificacion hecho localmente
}

/*
	Defino un protocolo arbitrario
	$ab...bc#
	$: caracter de inicio
	a: comando (1 byte), vale cualquier caracter
	b...b: datos (largo indefinido)
	c: verificacion
	#: caracter de fin
*/

enum{
    REPOSO = 0,
    COMANDO,
    DATOS,
    VERIFICACION
};


/* Variables maquina de recepcion */
int estado_rx = REPOSO;	
char comando_rx = 0; // Comando recibido por serie
char datos_rx[LARGO_DATOS]; // Vector donde recibo los datos de la trama
int indice_rx = 0; // Indice para vector de datos de comando

/* Maquina de estados de recepcion, diagrama en apunte */
void protocolo_me(uint8_t uart, char caracter_recibido)
{
	char verificacion = 0; // Verificacion local de los datos
	char verificacion_recibida = 0; // Verificacion recibida por serial
	
	switch(estado_rx)
	{
		case REPOSO:
			// Espero a que llegue un caracter de inicio
			if(caracter_recibido == CARACTER_INICIO){
				estado_rx = COMANDO;
			}
			break;
		case COMANDO: // El proximo byte que llegue despues del inicio es el comando
			// Recibo el comando e inmediatamente paso a recibir los datos,
			// luego se evaluará si el comando es válido
			comando_rx = caracter_recibido;
			estado_rx = DATOS;
			break;
		case DATOS:
			// Espero a que llegue el caracter de fin para dejar de recibir datos
			if(caracter_recibido == CARACTER_FIN)
			{
				// Evaluo si mi trama llego ok, hago la verificacion
				// Utilizo un define para evaluar si hago o no la verificacion
				indice_rx--;
				#ifdef VERIFICACION_RECEPCION
						verificacion = checksum_verif(datos_rx, indice_rx);
				#else
						verificacion = datos_rx[indice_rx]; // No controlo verificacion
				#endif
				// datos[indice] sera el byte de verificacion recibido
				verificacion_recibida = datos_rx[indice_rx];
				if(verificacion != verificacion_recibida)
				{
						// Si falla la verificacion no guardo los datos
						comando_rx = 0;
						// Limpio el vector
						//    Vector    Valor   Largo
						memset(datos_rx,     0,    indice_rx);
				}else{
						// Si la verificacion es correcta ejecuto el comando
						funcion_comandos(comando_rx, datos_rx);
				}
				indice_rx = 0; // Reseteo el indice cuando termino de recibir
				estado_rx = REPOSO;
				break;
			}
			// Recibo datos y aumento el indice
			datos_rx[indice_rx++] = caracter_recibido;
			break;
		default:
			estado_rx = REPOSO;
			break;
	}   
}

/* Esta funcion será la encargada de seleccionar que funcion debe ejecutarse
	 acorde al comando recibido */
void funcion_comandos(char comando, char * datos)
{
	switch(comando){
		case 'C':
			auxTX++;
			switchColorDisplay(datos);
			break;
		case 'D':
			auxTX++;
			lecturaDisplay(datos);
			break;
		case 'T':
			sizeImage(datos);
			break;
		case 'J':
			auxTX++;
			cant_JS++;
			moveJump(datos);
			break;
		case 'S':
			auxTX++;
			cant_JS++;
			moveStitch(datos);
			break;
		case 'H':
			auxTX++;
			switchColorEmbroider(datos);
			break;
		case 'P':
			cant_comandos++;
			colorEmbroider(datos);
			break;
		case 'W':
			cant_comandos++;
			commandMax(datos);
			break;
		case 'E':
			auxTX++;
			endEmbroider();
			break;
		/* Comando inexistente */
		default:
			break;
	}
	if(subirStop == 1){
		start = 0;
		habilitacion_Motor = 0;
		subir_Aguja();
	}
}

// $ab...bc#
void envio_trama(char comando, char * datos)
{
	char buffer[50];
	char verificacion = 0;
	char index = 0;
	/*			  
	sprintf nos "imprime" al buffer como si fuera un printf,
	solo que en vez de imprimir a la pantalla, escribe el string
	que le indiquemos. El valor que retorna será la cantidad de caracteres
	que logro escribir.		   $ a Datos*/
	index += sprintf(buffer, "%c%c%s", CARACTER_INICIO, comando, datos);
	/* Hago la verificacion para agregarla a la trama */
	 verificacion = checksum_verif(datos, strlen(datos));
	/* Comienzo a escribir desde "index" para no pisar lo ya escrito sobre buffer 
	   y comenzar a escribir después de la ultima posición escrita anteriormente */
	index += sprintf(&buffer[index], "%c%c", verificacion, CARACTER_FIN);
	/* Envio trama */
	for(int i = 0; i < index; i++){
		UART_Transmit(0, buffer[i]);
		while(!UART_TX_Ready(0)); // Espero a que este listo para transmitir
	}
}

