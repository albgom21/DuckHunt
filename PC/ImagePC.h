#pragma once
#include "checkML.h"
#include <vector>

class SDL_Surface;
class SDL_Texture;

class ImagePC {
private:
	int32_t w, h;				// Ancho y alto
	uint32_t* matrizColores;	// Información de los colores de la imágen en formato matriz lineal
	SDL_Texture* texture;		// Textura creada a través de la matriz de colores
public:
	ImagePC(uint32_t w_, uint32_t h_, uint32_t* matrizColores_);
	~ImagePC();

	// Crear una textura a través de la matriz de colores
	void createTexture();

	// Getters
	SDL_Texture* getTexture();
	int32_t getWidth();
	int32_t getHeight();
	uint32_t getColor(int i, int j);
	uint32_t* getColorMatrix();
};