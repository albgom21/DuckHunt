#pragma once
#include <string>
#include "Image.h"

class SDL_Renderer;
class SDL_Window;
class SDL_Surface;

#define VSYNC_MODE SDL_RENDERER_PRESENTVSYNC //0 disable vsync //SDL_RENDERER_PRESENTVSYNC enable vsync

class RendererPC {
public:	
	RendererPC();
	~RendererPC();
	static bool Init(int w, int h, std::string name);
	static void Release();
	static RendererPC* Instance();

	// Render methods
	void present();
	void clear(uint32_t color);
	void drawRect(int x, int y, int width, int height, uint32_t color);
	void putPixel(int x, int y, uint32_t color);
	void drawImage(Image* image, int sx1, int sy1, int sx2, int sy2, int tx1, int ty1, int tx2, int ty2);
	SDL_Texture* createTexture(int w, int h);
	Image* newImage(uint32_t w_, uint32_t h_, uint32_t* matrizColores_);
	
	// Getters
	int getWidth();
	int getHeight();

private:
	static RendererPC* instance_;
	SDL_Window* window;
	SDL_Renderer* renderer;
	int width, height;			// ancho y alto de la ventana

	bool auxInit(int w, int h, std::string name);

	// Conversores de color
	void ARGBhexToRGBA(uint32_t hexColor, int * rgba);
	void RGBAhexToRGBA(uint32_t hexColor, int* rgba);
};