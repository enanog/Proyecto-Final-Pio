/*
 * ------------------------------
 * Fonts.c (para el MKL25Z)
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
 
#include "ILI9341.h"
#include "TFT.h"
#include "Fonts.h"

#define letterWidth 	5
#define letterHeight 	6

#define pixelSize 		3

// eje x
#define row1 		(1*pixelWidth)
#define row2 		(2*pixelWidth)
#define row3 		(3*pixelWidth)
#define row4 		(4*pixelWidth)
#define row5 		(5*pixelWidth)
// eje y
#define line1 	(1*pixelHeight)
#define line2 	(2*pixelHeight)
#define line3 	(3*pixelHeight)
#define line4 	(4*pixelHeight)
#define line5 	(5*pixelHeight)



const uint16_t letterA[letterHeight][letterWidth] = {
    {0, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0}
};

const uint16_t letterB[letterHeight][letterWidth] = {
    {1, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {1, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
		{1, 1, 1, 0, 0}
};

const uint16_t letterC[letterHeight][letterWidth] = {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
		{0, 1, 1, 1, 0}
};

const uint16_t letterD[letterHeight][letterWidth] = {
    {1, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
		{1, 1, 1, 0, 0}
};

const uint16_t letterE[letterHeight][letterWidth] = {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
		{0, 1, 1, 1, 0}
};

const uint16_t letterF[letterHeight][letterWidth] = {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
		{0, 0, 0, 0, 0}
};

const uint16_t letterG[letterHeight][letterWidth] = {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 1, 1, 0},
    {1, 0, 0, 1, 0},
		{0, 1, 1, 0, 0}
};

const uint16_t letterH[letterHeight][letterWidth] = {
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 1, 1, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
		{1, 0, 0, 1, 0}
};

const uint16_t letterI[letterHeight][letterWidth] = {
    {1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
		{1, 1, 1, 0, 0}
};

const uint16_t letterJ[letterHeight][letterWidth] = {
    {0, 1, 1, 1, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 1, 0, 0},
		{1, 1, 0, 0, 0}
};

const uint16_t letterK[letterHeight][letterWidth] = {
    {1, 0, 0, 1, 0},
    {1, 0, 1, 0, 0},
    {1, 1, 0, 0, 0},
    {1, 0, 1, 0, 0},
    {1, 0, 0, 1, 0},
		{1, 0, 0, 1, 0}
};

const uint16_t letterL[letterHeight][letterWidth] = {
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0}
};

const uint16_t letterM[letterHeight][letterWidth] = {
    {1, 0, 0, 0, 1},
    {1, 1, 0, 1, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1}
};

const uint16_t letterN[letterHeight][letterWidth] = {
    {1, 0, 0, 1, 0},
    {1, 1, 0, 1, 0},
    {1, 1, 0, 1, 0},
    {1, 0, 1, 1, 0},
    {1, 0, 1, 1, 0},
		{0, 0, 0, 1, 0}
};

const uint16_t letterO[letterHeight][letterWidth] = {
    {0, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
		{0, 1, 1, 0, 0}
};

const uint16_t letterP[letterHeight][letterWidth] = {
    {0, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 1, 1, 0, 0},
    {1, 0, 0, 0, 0},
		{1, 0, 0, 0, 0}
};

const uint16_t letterQ[letterHeight][letterWidth] = {
    {0, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 1, 1, 0},
		{0, 1, 1, 1, 1}
};

const uint16_t letterR[letterHeight][letterWidth] = {
    {0, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
		{1, 0, 0, 1, 0}
};

const uint16_t letterS[letterHeight][letterWidth] = {
    {0, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0},
    {1, 0, 0, 1, 0},
		{0, 1, 1, 0, 0}
};

const uint16_t letterT[letterHeight][letterWidth] = {
    {1, 1, 1, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
		{0, 1, 0, 0, 0}
};

const uint16_t letterU[letterHeight][letterWidth] = {
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
		{1, 1, 1, 1, 0}
};

const uint16_t letterW[letterHeight][letterWidth] = {
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1},
		{0, 1, 0, 1, 0}
};

const uint16_t letterX[letterHeight][letterWidth] = {
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {0, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
		{1, 0, 0, 1, 0}
};

const uint16_t letterY[letterHeight][letterWidth] = {
    {1, 0, 0, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 0, 1, 0},
    {0, 0, 1, 0, 0},
    {0, 0, 1, 0, 0},
		{0, 0, 1, 1, 0}
};

const uint16_t letterZ[letterHeight][letterWidth] = {
    {1, 1, 1, 1, 0},
    {0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0},
    {1, 0, 0, 0, 0},
		{1, 1, 1, 1, 0}
};

const uint16_t letter0[letterHeight][letterWidth] = {
    {0, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 1, 1, 0},
    {1, 1, 0, 1, 0},
    {1, 0, 0, 1, 0},
		{0, 1, 1, 0, 0}
};

const uint16_t letter1[letterHeight][letterWidth] = {
    {0, 1, 0, 0, 0},
    {1, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0},
		{1, 1, 1, 0, 0}
};

/*
void printA(uint16_t x, uint16_t y, uint16_t color){
	printPixel(x, y, color);
	printPixel(x, y+line1, color);
	printPixel(x, y+line2, color);
	printPixel(x, y+line3, color);
	printPixel(x, y+line4, color);

	printPixel(x+row1, y+line2, color);
	printPixel(x+row1, y+line5, color);

	printPixel(x+row2, y+line2, color);
	printPixel(x+row2, y+line5, color);

	printPixel(x+row3, y, color);
	printPixel(x+row3, y+line1, color);
	printPixel(x+row3, y+line2, color);
	printPixel(x+row3, y+line3, color);
	printPixel(x+row3, y+line4, color);
}

void printB(uint16_t x, uint16_t y, uint16_t color){
	printPixel(x , y, color);
	printPixel(x , y+line1, color);
	printPixel(x , y+line2, color);
	printPixel(x , y+line3, color);
	printPixel(x , y+line4, color);
	printPixel(x , y+line5, color);

	printPixel(x+row1 , y, color);
	printPixel(x+row1 , y+line3, color);
	printPixel(x+row1 , y+line5, color);

	printPixel(x+row2 , y, color);
	printPixel(x+row2 , y+line3, color);
	printPixel(x+row2 , y+line5, color);

	printPixel(x+row3 , y+line1, color);
	printPixel(x+row3 , y+line2, color);
	printPixel(x+row3 , y+line4, color);
}

void printC(uint16_t x, uint16_t y, uint16_t color){
	printPixel(x , y+line1, color);
	printPixel(x , y+line2, color);
	printPixel(x , y+line3, color);
	printPixel(x , y+line4, color);

	printPixel(x+row1 , y, color);
	printPixel(x+row1 , y+line5, color);

	printPixel(x+row2 , y, color);
	printPixel(x+row2 , y+line5, color);

	printPixel(x+row3 , y+line1, color);
	printPixel(x+row3 , y+line4, color);
}

void printD(uint16_t x, uint16_t y, uint16_t color){
	printPixel(x , y, color);
	printPixel(x , y+line1, color);
	printPixel(x , y+line2, color);
	printPixel(x , y+line3, color);
	printPixel(x , y+line4, color);
	printPixel(x , y+line5, color);
 
	printPixel(x+row1 , y, color);
	printPixel(x+row1 , y+line5, color);
 
	printPixel(x+row2 , y, color);
	printPixel(x+row2 , y+line5, color);
 
	printPixel(x+row3 , y+line1, color);
	printPixel(x+row3 , y+line2, color);
	printPixel(x+row3 , y+line3, color);
	printPixel(x+row3 , y+line4, color);
}

void printE(uint16_t x, uint16_t y, uint16_t color){
	printPixel(x , y, color);
	printPixel(x , y+line1, color);
	printPixel(x , y+line2, color);
	printPixel(x , y+line3, color);
	printPixel(x , y+line4, color);
	printPixel(x , y+line5, color);
 
	printPixel(x+row1 , y, color);
	printPixel(x+row1 , y+line3, color);
	printPixel(x+row1 , y+line5, color);
 
	printPixel(x+row2 , y, color);
	printPixel(x+row2 , y+line3, color);
	printPixel(x+row2 , y+line5, color);
 
	printPixel(x+row3 , y, color);
	printPixel(x+row3 , y+line5, color);
}

void printF(uint16_t x, uint16_t y, uint16_t color){
	printPixel(x , y, color);
	printPixel(x , y+line1, color);
	printPixel(x , y+line2, color);
	printPixel(x , y+line3, color);
	printPixel(x , y+line4, color);
	printPixel(x , y+line5, color);
 
	printPixel(x+row1 , y+line3, color);
	printPixel(x+row1 , y+line5, color);
 
	printPixel(x+row2 , y+line3, color);
	printPixel(x+row2 , y+line5, color);
 
	printPixel(x+row3 , y+line5, color);
}

void printG(uint16_t x, uint16_t y, uint16_t color){
	printPixel(x , y+line1, color);
	printPixel(x , y+line2, color);
	printPixel(x , y+line3, color);
	printPixel(x , y+line4, color);

	printPixel(x+row1 , y, color);
	printPixel(x+row1 , y+line5, color);
 
	printPixel(x+row2 , y, color);
	printPixel(x+row2 , y+line2, color);
	printPixel(x+row2 , y+line5, color);
 
	printPixel(x+row3 , y+line1, color);
	printPixel(x+row3 , y+line2, color);
	printPixel(x+row3 , y+line4, color);
}

void printH(uint16_t x, uint16_t y, uint16_t color){
	printPixel(x , y+line1, color);
	printPixel(x , y+line2, color);
	printPixel(x , y+line3, color);
	printPixel(x , y+line4, color);
	printPixel(x , y+line5, color);

	printPixel(x+row1 ,  y, color);
	printPixel(x+row1 , y+line5, color);
 
	printPixel(x+row2 ,  y, color);
	printPixel(x+row2 , y+line2, color);
	printPixel(x+row2 , y+line5, color);
 
	printPixel(x+row3 , y+line1, color);
	printPixel(x+row3 , y+line2, color);
	printPixel(x+row3 , y+line4, color);
}

void printI(uint16_t x, uint16_t y, uint16_t color){
}

void printJ(uint16_t x, uint16_t y, uint16_t color){
}

void printK(uint16_t x, uint16_t y, uint16_t color){
}

void printL(uint16_t x, uint16_t y, uint16_t color){
}

void printM(uint16_t x, uint16_t y, uint16_t color){
}

void printN(uint16_t x, uint16_t y, uint16_t color){
}

void printO(uint16_t x, uint16_t y, uint16_t color){
}

void printP(uint16_t x, uint16_t y, uint16_t color){
}

void printQ(uint16_t x, uint16_t y, uint16_t color){
}

void printR(uint16_t x, uint16_t y, uint16_t color){
}

void printS(uint16_t x, uint16_t y, uint16_t color){
}

void printT(uint16_t x, uint16_t y, uint16_t color){
}

void printU(uint16_t x, uint16_t y, uint16_t color){
}

void printV(uint16_t x, uint16_t y, uint16_t color){
}

void printW(uint16_t x, uint16_t y, uint16_t color){
}

void printX(uint16_t x, uint16_t y, uint16_t color){
}

void printY(uint16_t x, uint16_t y, uint16_t color){
}

void printZ(uint16_t x, uint16_t y, uint16_t color){
}

void print0(uint16_t x, uint16_t y, uint16_t color){
}

void print1(uint16_t x, uint16_t y, uint16_t color){
}

void print2(uint16_t x, uint16_t y, uint16_t color){
}

void print3(uint16_t x, uint16_t y, uint16_t color){
}

void print4(uint16_t x, uint16_t y, uint16_t color){
}

void print5(uint16_t x, uint16_t y, uint16_t color){
}

void print6(uint16_t x, uint16_t y, uint16_t color){
}

void print7(uint16_t x, uint16_t y, uint16_t color){
}

void print8(uint16_t x, uint16_t y, uint16_t color){
}

void print9(uint16_t x, uint16_t y, uint16_t color){
}

void printB(uint16_t x, uint16_t y, uint16_t color){
}*/