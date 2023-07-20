#include "BackgroundElement.h"
#include "Renderer.h"

BackgroundElement::BackgroundElement(Sprite* sprt, float x_, float y_, float scaleFactor_) {
	sprite = sprt;
	scaleFactor = scaleFactor_;
	x = x_;
	y = y_;

	// Escalar el sprite
	w = scaleFactor * sprite->getWSprite();
	h = scaleFactor * sprite->getHSprite();
}

BackgroundElement::~BackgroundElement()
{}

void BackgroundElement::render(RenderThread& rt) {
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