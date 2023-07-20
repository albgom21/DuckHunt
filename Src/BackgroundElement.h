#pragma once
#include "InputState.h"
#include "RenderThread.h"

class BackgroundElement {
private:
	float w, h, x, y;	// Ancho, alto y coordenas x y
	Sprite* sprite;		// Referencia al sprite que muestra
	float scaleFactor;	// Factor de escala por el que multiplicar el ancho y alto del sprite original,
						// viene determinado por el tam de la ventana
public:
	BackgroundElement(Sprite* sprt, float x_, float y_, float scaleFactor_);
	~BackgroundElement();
	void render(RenderThread& rt);
};