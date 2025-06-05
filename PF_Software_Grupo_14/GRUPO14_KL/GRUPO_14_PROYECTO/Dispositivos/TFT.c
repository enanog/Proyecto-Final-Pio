/*
 * ------------------------------
 * Inicialización.c (para el MKL25Z)
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
 
#include "TFT.h"
#include "SPI.h"
#include "GPIO.h"
#include "Inicializacion.h"
#include "ILI9341.h"
#include "Fonts.h"

xy_t display = {0};
uint16_t bufferDisplay[RAM/2] = {0};
uint8_t save = 0;

/*
 * Función: writeCommand
 * ----------------------
 * Genero la escritura correspondiente para enviar un comando al ILI9341.
 *
 * Parámetros:
 * cmd: Comando que se envía en un octeto (escrito en hexa 0xXX).
 * 
 * return: nada. 
 */
void writeCommand(uint8_t cmd){
	GPIO_Set(DCRS, LOW);
	SPI_MasterWrite(0, cmd);
	GPIO_Set(DCRS, HIGH);
}


/*
 * Función: writeData
 * -------------------
 * Genero la escritura correspondiente para enviar un dato al ILI9341.
 *
 * Parámetros:
 * data: Data que se envía en un octeto (escrito en hexa 0xXX).
 * 
 * return: nada. 
 */
void writeData(uint8_t data){
	GPIO_Set(DCRS, HIGH);
	SPI_MasterWrite(0, data);
}


/*
 * Función: pause_ms
 * ------------------
 * Genero un delay para la inicialización.
 *
 * Parámetros:
 * ms: cantidad de ms.
 * 
 * return: nada. 
 */
void pause_ms(uint32_t ms)
{
	for(uint32_t i = 0; i < 30000 * ms; i++);
}


void ILI9341_Init(){
	SPI_Init(0, 0, 0);
	
	GPIO_Set(RESET_TFT, HIGH);
	pause_ms(1);
	GPIO_Set(RESET_TFT, LOW);
	pause_ms(1);
	GPIO_Set(RESET_TFT, HIGH);
	pause_ms(1);
	
	writeCommand(0xCF);
	writeData(0x00);
	writeData(0XC1);
	writeData(0X30);

	writeCommand(0xED);
	writeData(0x64);
	writeData(0x03);
	writeData(0X12);
	writeData(0X81);

	writeCommand(0xE8);
	writeData(0x85);
	writeData(0x00);
	writeData(0x78);

	writeCommand(0xCB);
	writeData(0x39);
	writeData(0x2C);
	writeData(0x00);
	writeData(0x34);
	writeData(0x02);

	writeCommand(0xF7);
	writeData(0x20);

	writeCommand(0xEA);
	writeData(0x00);
	writeData(0x00);

	writeCommand(ILI9341_PWCTR1); //Power control
	writeData(0x10); //VRH[5:0]

	writeCommand(ILI9341_PWCTR2); //Power control
	writeData(0x00); //SAP[2:0];BT[3:0]

	writeCommand(ILI9341_VMCTR1); //VCM control
	writeData(0x30);
	writeData(0x30);

	writeCommand(ILI9341_VMCTR2); //VCM control2
	writeData(0xB7); //--

	writeCommand(ILI9341_PIXFMT);
	writeData(0x55);

	writeCommand(0x36); // Memory Access Control
	writeData(0x18); // Rotation 0 (portrait mode)

	writeCommand(ILI9341_FRMCTR1);
	writeData(0x00);
	writeData(0x18);

	writeCommand(ILI9341_DFUNCTR); // Display Function Control
	writeData(0x08);
	writeData(0x82);
	writeData(0x27);

	writeCommand(0xF2); // 3Gamma Function Disable
	writeData(0x00);

	writeCommand(0x26); //Gamma curve selected
	writeData(0x01);

	writeCommand(0xE0); //Set Gamma
	writeData(0x0F);
	writeData(0x2A);
	writeData(0x28);
	writeData(0x08);
	writeData(0x0E);
	writeData(0x08);
	writeData(0x54);
	writeData(0xA9);
	writeData(0x43);
	writeData(0x0A);
	writeData(0x0F);
	writeData(0x00);
	writeData(0x00);
	writeData(0x00);
	writeData(0x00);

	writeCommand(0XE1); //Set Gamma
	writeData(0x00);
	writeData(0x15);
	writeData(0x17);
	writeData(0x07);
	writeData(0x11);
	writeData(0x06);
	writeData(0x2B);
	writeData(0x56);
	writeData(0x3C);
	writeData(0x05);
	writeData(0x10);
	writeData(0x0F);
	writeData(0x3F);
	writeData(0x3F);
	writeData(0x0F);

	writeCommand(0x2B);
	writeData(0x00);
	writeData(0x00);
	writeData(0x01);
	writeData(0x3f);

	writeCommand(0x2A);
	writeData(0x00);
	writeData(0x00);
	writeData(0x00);
	writeData(0xef);

	writeCommand(ILI9341_SLPOUT); //Exit Sleep

	writeCommand(ILI9341_DISPON); //Display on
}


static void SPI_WRITE16(uint16_t data)
{
	SPI_MasterWrite(0, data >> 8);
	SPI_MasterWrite(0, data & 0xFF);
}

void ILI9341_setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h) {
  static uint16_t old_x1 = 0xffff, old_x2 = 0xffff;
  static uint16_t old_y1 = 0xffff, old_y2 = 0xffff;

  uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);
  if (x1 != old_x1 || x2 != old_x2) {
    writeCommand(ILI9341_CASET); // Column address set
    SPI_WRITE16(x1);
    SPI_WRITE16(x2);
    old_x1 = x1;
    old_x2 = x2;
  }
  if (y1 != old_y1 || y2 != old_y2) {
    writeCommand(ILI9341_PASET); // Row address set
    SPI_WRITE16(y1);
    SPI_WRITE16(y2);
    old_y1 = y1;
    old_y2 = y2;
  }
  writeCommand(ILI9341_RAMWR); // Write to RAM
}
																		 
void ILI9341_drawPixel(int16_t x, int16_t y, uint16_t color) {
  // Clip first...
  if ((x >= 0) && (x < ILI9341_TFTWIDTH) && (y >= 0) && (y < ILI9341_TFTHEIGHT)) {
    // THEN set up transaction (if needed) and draw...
    ILI9341_setAddrWindow(x, y, 1, 1);
    SPI_WRITE16(color);
  }
	if((y >= 110 && 210 > y) && (x >= 100 && 140 > x) && save == 1){
		bufferDisplay[(40*(y-110)+ (x-100))] = color;
	}
}

void ILI9341_printPixel(uint16_t x, uint16_t y, uint16_t color, uint16_t pixelWidth, uint16_t pixelHeight){
	for(uint16_t j = 0; j < pixelHeight; j++){
		for(uint16_t i = 0; i < pixelWidth; i++)
			ILI9341_drawPixel(x+i, y+j, color);
	}
}

void ILI9341_drawLetter(uint16_t x,  uint16_t y, uint16_t color, char letra, uint8_t pixelWidth, uint8_t pixelHeight){
	xy_t point;
	uint8_t c;
	
	point.x = x;
	point.y = y;
	
	if(('a' <= letra) && (letra <= 'z') && (letra != 'm') && (letra != 'x'))
		c = letra - 'a';
	else if('A' <= letra && letra <= 'Z')
		c = letra - 'A';
	else if('0' <= letra && letra <= '9')
		c = letra - '0' + ('Z' - 'A') + 1;
	else if(letra == 39)
		c = letra;
	else if(letra == ':')
		c = 38;
	else if(letra == '%')
		c = 37;
	else if(letra == 'm')
		c = 40;
	else if(letra == 'x')
		c = 41;
	else if(letra == '!')
		c = 42;
	
	for(uint8_t j = 0; j < letterWidth; j++){
		for(uint8_t i = 0; i < letterHeight; i++){
			if(letter[c][i][j] == 1){
				ILI9341_printPixel(point.x, point.y, color, pixelWidth, pixelHeight);
			}
			point.x += pixelWidth;
		}
		point.x = x;
		point.y += pixelHeight;
	}
}

void ILI9341_drawString(uint16_t x, uint16_t y, uint16_t color, char* ptr, uint8_t pixelWidth, uint8_t pixelHeight){
	xy_t point;
	
	point.x = x;
	point.y = y;
	
	for(uint8_t i = 0; ptr[i] != 0; i++){
		if(ptr[i] == 32){
			point.y += 2* pixelHeight;
			continue;
		}
		ILI9341_drawLetter(point.x, point.y, color, ptr[i], pixelWidth, pixelHeight);
		if(ptr[i] == 'M' || ptr[i] == 'm' || ptr[i] == 'W' || ptr[i] == 'w' || ptr[i] == 'Y' || ptr[i] == 'y' ||
			 ptr[i] == 'Q' || ptr[i] == 'q' || ptr[i] == 'T' || ptr[i] == 't' || ptr[i] == 'V' || ptr[i] == 'v')
			point.y += 6* pixelHeight;
		else if(ptr[i] == '1' || ptr[i] == 'I' || ptr[i] == 'i' || ptr[i] == ':' || ptr[i] == 'x')
			point.y += 4* pixelHeight;
		else
			point.y += 5* pixelHeight;
	}
}

void ILI9341_printLine(uint16_t xi, uint16_t yi, uint16_t color, uint16_t xf, uint16_t yf) {
	
	xi += display.centerx;
	xf += display.centerx;
	yi += display.centery;
	yf += display.centery;
	
	float m = (float)(yf - yi) / (xf - xi);

	float b = yi - m * xi;

	int stepx = (xi > xf) ? -1 : 1;
	int stepy = (yi > yf) ? -1 : 1;
	
	if(xi != xf){
		for (int x = xi; x != xf + stepx; x += stepx) {
			// Calcula la coordenada y usando la ecuación de la línea y redondea hacia abajo
			uint16_t y = (uint16_t)(m * x + b);

			// Dibuja el píxel en las coordenadas calculadas
			ILI9341_drawPixel(y, x, color);
		}
	}else{	
		for (int y = yi; y != yf + stepy; y += stepy) {
			// Dibuja el píxel en las coordenadas calculadas
			ILI9341_drawPixel(y, xi, color);
		}
	}
}

void ILI9341_warning(){
	ILI9341_printPixel(0, 0, ILI9341_LIGHTGREY, 240, 320);
	ILI9341_drawString(35, 45, ILI9341_BLACK, "IMAGEN FUERA", 4, 4);
	ILI9341_drawString(65, 70, ILI9341_BLACK, "DE ESCALA", 4, 4);
	
	ILI9341_drawLetter(110, 100, ILI9341_BLACK, '=', 12, 12);
	ILI9341_drawLetter(110, 100, ILI9341_YELLOW, '#', 12, 12);
}

void drawFrame(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color, uint8_t fill, int8_t dash_length) {
	if(fill == 0){
		if(dash_length != -1){
			// Dibuja los lados del cuadrado
			for (uint16_t x = x0; x < x0 + width; x++) {
				if (x % (2 * dash_length) < dash_length) {
					ILI9341_drawPixel(y0, x, color); // Top side
					ILI9341_drawPixel(y0 + height - 1, x, color); // Bottom side
				}
			}

			// Dibuja los bordes verticales con líneas punteadas
			for (uint16_t y = y0; y < y0 + height; y++) {
				if (y % (2 * dash_length) < dash_length) {
					ILI9341_drawPixel(y, x0, color); // Left side
					ILI9341_drawPixel(y, x0 + width - 1, color); // Right side
				}
			}
		}else{
			// Dibuja los bordes horizontales
			for (uint16_t x = x0; x < x0 + width; x++) {
				ILI9341_drawPixel(y0, x, color); // Top side
				ILI9341_drawPixel(y0 + height - 1, x, color); // Bottom side
			}

			// Dibuja los bordes verticales
			for (uint16_t y = y0; y < y0 + height; y++) {
				ILI9341_drawPixel(y, x0, color); // Left side
				ILI9341_drawPixel(y, x0 + width - 1, color); // Right side
			}
		}
	} else{
		for (uint16_t y = y0; y < y0 + height; y++) {
			for (uint16_t x = x0; x < x0 + width; x++) {
				ILI9341_drawPixel(y, x, color);
			}
    }
	}
}

void drawVerticalProgressBar(uint16_t x, uint16_t y, uint16_t height, uint16_t width, uint16_t progress, uint16_t total, uint16_t fillColor, uint8_t aux){
	static uint8_t positionBar = 0;
	
	if(aux){
		// Dibuja el fondo blanco y el borde negro
		ILI9341_printPixel(y , x, ILI9341_BLACK, height, 1);
		ILI9341_printPixel(y , x, ILI9341_BLACK, 1, width);
		ILI9341_printPixel(y + height, x, ILI9341_BLACK, 1, width);
		ILI9341_printPixel(y , x + width, ILI9341_BLACK, height, 1);
		ILI9341_drawString(y + height + 10, x + 6, ILI9341_BLACK, "00 %", 3, 3);
		positionBar = 0;
	}

	// Calcula la altura de la barra de progreso
	uint16_t progressBarHeight = (height * progress) / total;
	if(positionBar != progressBarHeight){
		if(progressBarHeight != height)
			ILI9341_printPixel(y + height - progressBarHeight, x + 1, fillColor, progressBarHeight - positionBar, width-1);  // Relleno con el color de la barra de progreso
		
		drawFrame(x + 2, y + height + 10, 53, 20, ILI9341_WHITE, 1, -1);
		// Dibuja el texto con el porcentaje
		if((progress*100/total) != 100){
			char percentageText[5] = {(progress*100/total)/10+48, ((progress*100/total)%10+48),' ','%'};
			ILI9341_drawString(y + height + 10, x + 6, ILI9341_BLACK, percentageText, 3, 3);
		} else
			ILI9341_drawString(y + height + 10, x + 1, ILI9341_BLACK, "100 %", 3, 3);
		
		positionBar = progressBarHeight;
	}
}

void drawCircle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color, uint8_t fill) {
	if(fill == 0){
		int16_t x = radius;
		int16_t y = 0;
		int16_t err = 1 - x;

		while (x >= y) {
			// Dibuja píxeles en las ocho secciones del círculo
			ILI9341_drawPixel(y0 + y, x0 + x, color); // Top right
			ILI9341_drawPixel(y0 + x, x0 + y, color); // Upper top right
			ILI9341_drawPixel(y0 - x, x0 + y, color); // Upper top left
			ILI9341_drawPixel(y0 - y, x0 + x, color); // Top left
			ILI9341_drawPixel(y0 - x, x0 - y, color); // Bottom left
			ILI9341_drawPixel(y0 - y, x0 - x, color); // Bottom bottom left
			ILI9341_drawPixel(y0 + y, x0 - x, color); // Bottom bottom right
			ILI9341_drawPixel(y0 + x, x0 - y, color); // Bottom right

			if (err <= 0) {
					y += 1;
					err += 2 * y + 1;
			}
			if (err > 0) {
					x -= 1;
					err -= 2 * x + 1;
			}
		}
	}else{
		for (int16_t y = -radius; y <= radius; y++) {
			for (int16_t x = -radius; x <= radius; x++) {
				if (x * x + y * y <= radius * radius) {
						ILI9341_drawPixel(y0 + y, x0 + x, color);
				}
			}
    }
	}
}

void drawCheckbox(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t boxColor, uint16_t checkmarkColor, uint8_t isChecked) {
	drawFrame(x, y, width, height, boxColor, 0, -1);
	if(isChecked){
		for(uint8_t i = 0; i < 4; i++){
			ILI9341_printLine(x+3+i, y+16, checkmarkColor, x+11, y+24-i);
			ILI9341_printLine(x+11, y+24-i, checkmarkColor, x+26-i, y+9);
			ILI9341_printLine(x+26-i, y+9, checkmarkColor, x+23, y+6+i);
			ILI9341_printLine(x+23, y+6+i, checkmarkColor, x+11, y+18+i);
			ILI9341_printLine(x+11, y+18+i, checkmarkColor, x+6, y+13+i);
			ILI9341_printLine(x+6, y+13+i, checkmarkColor, x+3+i, y+16);
		}
	}else{
		for(uint8_t i = 0; i < 4; i++){
			ILI9341_printLine(x+3+i, y+16, ILI9341_WHITE, x+11, y+24-i);
			ILI9341_printLine(x+11, y+24-i, ILI9341_WHITE, x+26-i, y+9);
			ILI9341_printLine(x+26-i, y+9, ILI9341_WHITE, x+23, y+6+i);
			ILI9341_printLine(x+23, y+6+i, ILI9341_WHITE, x+11, y+18+i);
			ILI9341_printLine(x+11, y+18+i, ILI9341_WHITE, x+6, y+13+i);
			ILI9341_printLine(x+6, y+13+i, ILI9341_WHITE, x+3+i, y+16);
		}
	}
}

void drawTriangle(uint16_t x, uint16_t y, uint16_t size, uint16_t color, uint8_t fill) {
	if(fill){
		for(uint8_t i = 0; i < size/2; i++){
			ILI9341_printLine(x+i, y+size-i, color, x+size-i, y+size-i);
			ILI9341_printLine(x+size-i, y+size-i, color, x+size/2, y+i);
			ILI9341_printLine(x+size/2, y+i, color, x+i, y+size-i);
		}
	}else{
		ILI9341_printLine(x, y+size, color, x+size, y+size);
		ILI9341_printLine(x+size, y+size, color, x+size/2, y);
		ILI9341_printLine(x+size/2, y, color, x, y+size);
	}
}

void ILI9341_printBuffer(){
	for(uint16_t i = 0; i < 40; i++){
		for(uint16_t j = 0; j < 100; j++)
			ILI9341_drawPixel(i+100, j+110, bufferDisplay[j*40+i]);
	}
}