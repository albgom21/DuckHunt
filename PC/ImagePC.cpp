#include "Image.h"
#include <iostream>
#include <SDL.h>
#include "Renderer.h"

ImagePC::ImagePC(uint32_t w_, uint32_t h_, uint32_t* matrizColores_) {
	w = w_;
	h = h_;
	matrizColores = matrizColores_;
	createTexture();
}

// Crear textura mediante una matriz lineal de colores
void ImagePC::createTexture() {
	texture = Renderer::Instance()->createTexture(w, h);     // Crear una textura de tam w*h

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);   // Activar en la textura el componente alpha
	SDL_UpdateTexture(texture, nullptr, matrizColores, w * 4); // Poner en la textura los colores, width*4(int) es el numero de bytes en una fila de la matriz
}

// Liberar la textura y matriz de colores
ImagePC::~ImagePC() {
	SDL_DestroyTexture(texture);
	texture = nullptr;

	delete matrizColores;
	matrizColores = nullptr;
}

// Getters
uint32_t* ImagePC::getColorMatrix() { return matrizColores; }

SDL_Texture* ImagePC::getTexture() { return texture; }

int32_t ImagePC::getWidth() { return w; }

int32_t ImagePC::getHeight() { return h; }

uint32_t ImagePC::getColor(int i, int j)
{
	return matrizColores[w * i + j];
}