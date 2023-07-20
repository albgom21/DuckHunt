#pragma once
#include "Image.h"
//#include "checkML.h" // Comentado ya esta clase la usa PS4 y PC
class Sprite {
public:
	Sprite(Image* image_, int nF, int nC, int fSelec, int cSelec);
	~Sprite();

	// Dibuja el sprite en el rect dest definido por los argumentos
	void drawSprite(int tx1, int ty1, int tx2, int ty2);

	// Getters
	float getWSprite();
	float getHSprite();
	Image* getImage();
private:
	Image* image;
	float wSprite, hSprite;   // tamaño del sprite de la imagen
	int numFil, numCol;       // número de sprites de la imagen
	int filaSelec, colSelec;  // sprite seleccionado
	int sx1, sy1, sx2, sy2;   // rect source
};