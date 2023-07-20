#pragma once
#include "InputState.h"
#include "RenderThread.h"

class Cursor {
private:
	float w, h, x, y;		// Ancho, alto y coordenas x y
	int limiteX, limiteY;	// Limites de pantalla en X e Y a los que puede llegar el cursor (= tamaño de pantalla)
	Sprite* sprite;			// Referencia al sprite que muestra
	float scaleFactor;		// Factor de escala dependiendo del tamaño de ventana
	bool disparado;			// Si ya se ha disparado
	bool release;			// Si se han soltado los botones de disparo
	float speed;			// Velocidad del cursor
public:
	Cursor(Sprite* sprt, float x_, float y_, float scaleFactor_, int limiteX_, int limiteY_);
	~Cursor();

	void render(RenderThread& rt);
	void update(InputState iS, double deltaTime);
	
	// Getters
	bool getDisparado();
	int getX();
	int getY();
};