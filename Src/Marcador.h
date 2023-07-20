#pragma once
#include "InputState.h"
#include "RenderThread.h"

class Marcador {
private:
	float w, h, x, y;	// Ancho, alto y coordenas x y
	Sprite** sprites;	// Referencia al sprite que muestra
	float scaleFactor;	// Factor de escala por el que multiplicar el ancho y alto del sprite original,
						// viene determinado por el tam de la ventana
	int nDigitos;
	uint32_t* marcadorPuntos;
public:
	Marcador(Sprite** sprt, float x_, float y_, float scaleFactor_, uint32_t* marcador, int nDigitos);
	~Marcador();
	void render(RenderThread& rt);
};