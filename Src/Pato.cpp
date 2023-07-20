#include "Pato.h"
#include "Renderer.h"
#include "Platform.h"
#include <iostream>

Pato::Pato(std::vector<Sprite*>& sprite_, std::vector<Sprite*>& puntos_, int posIniY_, int posReboteY_) {
	contFrames = frameAct = lastChangeFrame = offsetAnim = 0;
	posIniY = posIniY_;
	posReboteY = posReboteY_;
	spritePatos = sprite_;
	spritePuntos = puntos_;
	alive = true;
	destroy = estampado = cayendo = false;
	timeSinceStart = 0.0;
	angle = Platform::Instance()->getRandom(PI + (PI / 6), 2 * PI - (PI / 6)); // en grados 210, 150

	wWidth = Renderer::Instance()->getWidth();
	wHeight = Renderer::Instance()->getHeight();

	int r = Platform::Instance()->getRandom(0, 3);
	if (r == 0) { //NEGRO
		tipo = NEGRO;
		timeToChangeSprite = CHANGE_FRAME_NEGRO;
		offsetColor = 1;
		speed = (50.0 / 320.0) * wWidth; // 50 pxNES/s		320 es el ancho completo del juego
		puntos = 500;
	}
	else if (r == 1) { //AZUL
		tipo = AZUL;
		timeToChangeSprite = CHANGE_FRAME_AZUL;
		offsetColor = 0;
		speed = (85.0 / 320.0) * wWidth;   // 85 pxNES/s
		puntos = 1000;
	}
	else { //ROJO
		tipo = ROJO;
		timeToChangeSprite = CHANGE_FRAME_ROJO;
		offsetColor = 2;
		speed = (120.0 / 320.0) * wWidth;  // 120 pxNES/s
		puntos = 1500;
	}

	int fAct = frameAct + (3 * offsetAnim) + (15 * offsetColor);
	scaleFactor = wWidth / 10.0 / spritePatos[fAct]->getHSprite();
	w = scaleFactor * spritePatos[fAct]->getWSprite();
	h = scaleFactor * spritePatos[fAct]->getHSprite();

	wPuntos = scaleFactor * spritePuntos[tipo]->getWSprite();
	hPuntos = scaleFactor * spritePuntos[tipo]->getHSprite();

	int minX = 0;
	int maxX = wWidth - w;

	x = Platform::Instance()->getRandom(minX, maxX);
	y = posIniY; // Salen un poco mas arriba

	setAnim();
}

bool Pato::checkCollision(int posX, int posY)
{
	if (estampado || cayendo) // Si ya está muerto no contar con el en la colision
		return false;
	return (posX > x&& posX < x + w && posY > y&& posY < y + h);
}

void Pato::update(double deltaTime) {
	timeSinceStart += deltaTime;
	if (!estampado && !cayendo) {
		// Cambiar el frame 1 vez cada 1/3 segundos
		if (timeSinceStart > lastChangeFrame + timeToChangeSprite) {
			contFrames++;
			frameAct = anim[contFrames % 4];
			lastChangeFrame = timeSinceStart;
		}

		bool bounce = false;
		// Lógica de rebotes
		if (alive) {
			if (x + w >= wWidth) //DERECHA  // 120         // 240
			{
				angle = Platform::Instance()->getRandom((PI / 2) + (PI / 6), 3 * (PI / 2) - (PI / 6));
				x = wWidth - w; bounce = true;
			}
			else if (x <= 0) //IZQUIERDA // 60          // 300
			{
				angle = Platform::Instance()->getRandom((PI / 2) - (PI / 6), 3 * (PI / 2) + (PI / 6));
				x = 1; bounce = true;
			}
			if (y + h >= posReboteY && (sin(angle) * speed * deltaTime) > 0) //ABAJO // 210         // 330
			{
				angle = Platform::Instance()->getRandom(PI + (PI / 6), 2 * PI - (PI / 6));
				y = posReboteY - (h / 2); bounce = true;
			}
			else if (y <= 0) //ARRIBA   // 30          //150
			{
				angle = Platform::Instance()->getRandom((PI / 6), PI - (PI / 6));
				y = 1; bounce = true;
			}
		}
		// Si se ha salido de pantalla marcar para destruir
		else if (x >= wWidth || x <= -w || y >= posReboteY || y <= -h) destroy = true;

		x += cos(angle) * speed * deltaTime;
		y += sin(angle) * speed * deltaTime;

		if (bounce) {
			setAnim();
			bounce = false;
		}
		alive = TIME_TO_LIVE > timeSinceStart;
	}
	else if (estampado) { // Si ha sido alcanzado cambiar el estado para cambiar la animación
		if (timeSinceStart > lastChangeFrame + 1) {
			estampado = false;
			cayendo = true;
			lastChangeFrame = timeSinceStart;
			frameAct = 0;
		}
	}
	else if (cayendo) { // Cambiar alcanzado cambiar el estado para cambiar la animación
		if (timeSinceStart > lastChangeFrame + CHANGE_FRAME_DEATH) {
			if (frameAct == 0)
				frameAct = 1;
			else frameAct = 0;
			lastChangeFrame = timeSinceStart;
		}
		y += wHeight/100;
		if (x >= wWidth || x <= -w || y >= posReboteY || y <= -h) destroy = true;
	}
}

void Pato::render(RenderThread& rt) {
	int fAct;
	if (estampado) { // Anim estampado
		fAct = 47 + (3 * offsetColor); // 47 azul  50 negro  53 rojo 
		w = scaleFactor * spritePatos[fAct]->getWSprite();
		h = scaleFactor * spritePatos[fAct]->getHSprite();
	}
	else if (cayendo) { // Anim cayendo
		fAct = 45 + frameAct + (3 * offsetColor);
		w = scaleFactor * spritePatos[fAct]->getWSprite();
		h = scaleFactor * spritePatos[fAct]->getHSprite();
	}
	else { // Anim volar
		fAct = frameAct + (3 * offsetAnim) + (15 * offsetColor); // animación + offset orientación + offset color
		w = scaleFactor * spritePatos[fAct]->getWSprite();
		h = scaleFactor * spritePatos[fAct]->getHSprite();
	}

	// Renderizar puntos si esta cayendo el pato
	if (cayendo) {
		Command c;
		c.type = RENDER;
		c.params.render.sprite = spritePuntos[tipo];
		c.params.render.tx1 = xPuntos + (wPuntos / 2);
		c.params.render.ty1 = yPuntos + (hPuntos / 2);
		c.params.render.tx2 = xPuntos + (wPuntos / 2) + wPuntos;
		c.params.render.ty2 = yPuntos + (hPuntos / 2) + hPuntos;

		rt.addCommand(c); //MANDAR COMANDO RENDER
	}

	Command c;
	c.type = RENDER;
	c.params.render.sprite = spritePatos[fAct];
	c.params.render.tx1 = x;
	c.params.render.ty1 = y;
	c.params.render.tx2 = x + w;
	c.params.render.ty2 = y + h;

	rt.addCommand(c); //MANDAR COMANDO RENDER
}

void Pato::setAnim() {
	double angleInDegrees = (angle * 180) / PI;

	if (angleInDegrees >= 0 && angleInDegrees < 30)         offsetAnim = 0;	//ANIM DR HOR
	else if (angleInDegrees >= 30 && angleInDegrees < 90)   offsetAnim = 2;	//ANIM DR DIAG
	else if (angleInDegrees >= 90 && angleInDegrees < 150)  offsetAnim = 3; //ANIM IZ DIAG
	else if (angleInDegrees >= 150 && angleInDegrees < 210) offsetAnim = 1; //ANIM IZ HOR
	else if (angleInDegrees >= 210 && angleInDegrees < 240) offsetAnim = 3; //ANIM IZ DIAG
	else if (angleInDegrees >= 240 && angleInDegrees < 300) offsetAnim = 4; //ANIM ARRIBA
	else if (angleInDegrees >= 300 && angleInDegrees < 330) offsetAnim = 2; //ANIM DR DIAG
	else if (angleInDegrees >= 330 && angleInDegrees < 360) offsetAnim = 0; //ANIM DR HOR
}

void Pato::setEstampado(bool e)
{
	estampado = e;
	xPuntos = x;
	yPuntos = y;
}

uint32_t Pato::getPoints()
{
	return puntos;
}