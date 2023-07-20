#include "Sprite.h"
#include "Renderer.h"

Sprite::Sprite(Image* image_, int nF, int nC, int fSelec, int cSelec) : image(image_), numFil(nF), numCol(nC), filaSelec(fSelec), colSelec(cSelec) {
	wSprite = image->getWidth() / numCol;
	hSprite = image->getHeight() / numFil;

	sx1 = colSelec * wSprite;
	sy1 = filaSelec * hSprite;
	sx2 = sx1 + wSprite;
	sy2 = sy1 + hSprite;
}

Sprite::~Sprite()
{}

void Sprite::drawSprite(int tx1, int ty1, int tx2, int ty2) {
	// Dibujar la imagen del sprite en el rect t
	Renderer::Instance()->drawImage(image, sx1, sy1, sx2, sy2, tx1, ty1, tx2, ty2);
}

float Sprite::getWSprite() { return wSprite; }

float Sprite::getHSprite() { return hSprite; }

Image* Sprite::getImage() {	return image; }