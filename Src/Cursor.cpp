#include "Cursor.h"
#include <iostream>

Cursor::Cursor(Sprite* sprt, float x_, float y_, float scaleFactor_, int limiteX_, int limiteY_)
{
	sprite = sprt;
	scaleFactor = scaleFactor_;

	x = x_;
	y = y_;

	w = scaleFactor * sprite->getWSprite();
	h = scaleFactor * sprite->getHSprite();
	limiteX = limiteX_;
	limiteY = limiteY_;

	// Cálculo de la velocidad para que sea relativa al tamaño de pantalla
	speed = (20.0 / 320.0) * limiteX;

	release = true;
}

Cursor::~Cursor()
{}

void Cursor::render(RenderThread& rt)
{
	Command c;

	// Configuración del comando render con sus atributos
	c.type = RENDER;
	c.params.render.sprite = sprite;
	c.params.render.tx1 = x;
	c.params.render.ty1 = y;
	c.params.render.tx2 = x + w;
	c.params.render.ty2 = y + h;

	// Añadir comando a la cola
	rt.addCommand(c);
}


void Cursor::update(InputState iS, double deltaTime) {
	disparado = false;

	// Comprobar que se han soltado los botones de disparo
	if (!iS.buttonO && !iS.buttonX && iS.l2 == 0 && iS.r2 == 0) 
		release = true;

	if (release && (iS.buttonO || iS.buttonX || iS.l2 > 0 || iS.r2 > 0)) { //Ver los valores de los gatillos
		disparado = true;
		release = false;
	}

	x += iS.stickX * 5 * speed * deltaTime;
	y += iS.stickY * 5 * speed * deltaTime;

	x += iS.velAngX * speed * deltaTime;
	y += iS.velAngY * speed * deltaTime;


	// Reposicionar el cusor si excede los límites
	if (x < -w / 2)				   //IZQUIERDA
		x = -w / 2;
	else if (x + w / 2 >= limiteX) //DERECHA
		x = limiteX - w / 2;
	if (y < -h / 2)				   //ARRIBA
		y = -h / 2;
	else if (y + h / 2 >= limiteY) //ABAJO
		y = limiteY - h / 2;
}

//--------------- Getters ---------------

bool Cursor::getDisparado()
{
	return disparado;
}

int Cursor::getX() 
{
	// Centrado en el medio del sprite
	return x + (w / 2);
}

int Cursor::getY() 
{
	// Centrado en el medio del sprite
	return y + (h / 2);
}