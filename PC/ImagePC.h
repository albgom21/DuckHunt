#pragma once
#include "checkML.h"
#include <vector>

class SDL_Surface;
class SDL_Texture;

class ImagePC {
private:
	int32_t w, h;				// Ancho y alto
	uint32_t* matrizColores;	// Informaci�n de los colores de la im�gen en formato matriz lineal
	SDL_Texture* texture;		// Textura creada a trav�s de la matriz de colores
public:
	ImagePC(uint32_t w_, uint32_t h_, uint32_t* matrizColores_);
	~ImagePC();

	// Crear una textura a trav�s de la matriz de colores
	void createTexture();

	// Getters
	SDL_Texture* getTexture();
	int32_t getWidth();
	int32_t getHeight();
	uint32_t getColor(int i, int j);
	uint32_t* getColorMatrix();
};