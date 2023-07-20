#include "Renderer.h"
#include <cassert>
#include "checkML.h"
#include <iostream>
#include <vector>
#include <SDL.h>

RendererPC* RendererPC::instance_ = nullptr;

// Constructor, start, destructor and Instance getter methods---------------------------------------------------
RendererPC::RendererPC() {
	width = height = 0;
	window = nullptr; 
	renderer = nullptr;
}

// Inicialización en dos pasos
bool RendererPC::Init(int w, int h, std::string name) {
	assert(instance_ == nullptr);
	instance_ = new RendererPC(); // Objeto neutro
	if (!instance_->auxInit(w, h, name)) {	// Inicialización
		instance_->Release();	 // En caso de fallo eliminar el objeto
		return false;
	}
	return true;
}

bool RendererPC::auxInit(int w, int h, std::string name) {
	width = w; height = h;
	window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << ("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else renderer = SDL_CreateRenderer(window, -1, VSYNC_MODE); // 0 disable vsync <-> 1 enable vsync SDL_RENDERER_PRESENTVSYNC
	return true;
}

RendererPC::~RendererPC() {
	// Liberar renderer y window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;
}

void RendererPC::Release() {
	delete instance_;
	instance_ = nullptr;
}

RendererPC* RendererPC::Instance() {
	assert(instance_ != nullptr);
	return instance_;
}


//Render methods-----------------------------------------------------------------------------------------------
void RendererPC::present() {
	SDL_RenderPresent(renderer);
}

// Limpiar renderer con un color
void RendererPC::clear(uint32_t color) {
	// Establecer el color 
	SDL_SetRenderDrawColor(renderer, (color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8) & 0xFF, (color) & 0xFF); //RGBAhexToRGBA

	// Llenar el renderer del color
	SDL_RenderClear(renderer);
}

// Dibujar un rectángulo en de tam w*h en la pos x,y de un color
void RendererPC::drawRect(int x, int y, int width, int height, uint32_t color) {
	// Crear un rect de SDL con los parámetros
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = width;
	r.h = height;

	// Establecer el color 
	SDL_SetRenderDrawColor(renderer, (color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8) & 0xFF, (color) & 0xFF); //RGBAhexToRGBA

	// Pintar el rectángulo
	SDL_RenderFillRect(renderer, &r);
}

// Dibujar un pixel en un pos x,y de un color
void RendererPC::putPixel(int x, int y, uint32_t color) {
	// Establecer el color 
	SDL_SetRenderDrawColor(renderer, (color >> 24) & 0xFF, (color >> 16) & 0xFF, (color >> 8) & 0xFF, (color) & 0xFF); //RGBAhexToRGBA
	
	// Pintar el pixel
	SDL_RenderDrawPoint(renderer, x, y);
}

// Dibujar una imagen a partir de una textura, con un rect src sobre la textura que establece que parte se va a ser y un rect dst que es donde se muestra en pantalla
void RendererPC::drawImage(Image* image, int sx1, int sy1, int sx2, int sy2, int tx1, int ty1, int tx2, int ty2) {
	// Creación de los rect source y destino
	SDL_Rect src = { sx1, sy1, sx2 - sx1, sy2 - sy1 };
	SDL_Rect dst = { tx1, ty1, tx2 - tx1, ty2 - ty1 };

	// Copiar la porción de textura(src) en pantalla(dst)
	SDL_RenderCopy(renderer, image->getTexture(), &src, &dst);
}

// Crear una textura vacía de tam w*h
SDL_Texture* RendererPC::createTexture(int w, int h) {
	return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, w, h);
}

// Crear una imagen a partir de una matriz de colores y su tam w*h
Image* RendererPC::newImage(uint32_t w_, uint32_t h_, uint32_t* matrizColores_)
{
	return new ImagePC(w_, h_, matrizColores_);
}

// Getters------------------------------------------------------------------------------------------------------
int RendererPC::getWidth() { return width; }

int RendererPC::getHeight() { return height; }

// Private converts methods-------------------------------------------------------------------------------------
void RendererPC::ARGBhexToRGBA(uint32_t hexColor, int* rgba) {
	rgba[3] = (hexColor >> 24) & 0xFF;  //a
	rgba[0] = (hexColor >> 16) & 0xFF;  //r
	rgba[1] = (hexColor >> 8) & 0xFF;   //g
	rgba[2] = (hexColor) & 0xFF;        //b
}

void RendererPC::RGBAhexToRGBA(uint32_t hexColor, int* rgba) {
	rgba[0] = (hexColor >> 24) & 0xFF;  //r
	rgba[1] = (hexColor >> 16) & 0xFF;  //g
	rgba[2] = (hexColor >> 8) & 0xFF;   //b
	rgba[3] = (hexColor) & 0xFF;        //a
}