#include "Marcador.h"
#include "Renderer.h"

Marcador::Marcador(Sprite** sprt, float x_, float y_, float scaleFactor_, uint32_t* marcadorPuntos_, int nDigitos_) {
	sprites = sprt;
	scaleFactor = scaleFactor_;
	marcadorPuntos = marcadorPuntos_;
	x = x_;
	y = y_;
	nDigitos = nDigitos_;
	// Escalar el sprite
	w = scaleFactor * sprites[0]->getWSprite();
	h = scaleFactor * sprites[0]->getHSprite();
}

Marcador::~Marcador()
{}

void Marcador::render(RenderThread& rt) {
	
	Command c;
	c.type = MARCADOR;
	c.params.marcador.x = x;
	c.params.marcador.y = y;
	c.params.marcador.w = w;
	c.params.marcador.h = h;
	c.params.marcador.nDigitos = nDigitos;
	c.params.marcador.numero = *marcadorPuntos;
	c.params.marcador.sprites = sprites;
	rt.addCommand(c);
}