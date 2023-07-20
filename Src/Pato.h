#pragma once
#include "Sprite.h"
#include "RenderThread.h"

int const TIME_TO_LIVE = 4;
double const CHANGE_FRAME_NEGRO = 1.0 / 3.0;  // Tiempo para cambiar cada frame en las animaciones de los patos negros
double const CHANGE_FRAME_AZUL = 1.0 / 6.0;   // Tiempo para cambiar cada frame en las animaciones de los patos azules
double const CHANGE_FRAME_ROJO = 1.0 / 12.0;  // Tiempo para cambiar cada frame en las animaciones de los patos rojos
double const CHANGE_FRAME_DEATH = 0.1;	// Tiempo para cambiar cada frame en la animación de muerte
#define PI 3.14159265

enum Tipo { NEGRO, AZUL, ROJO };
class Pato {
private:
	double x, y;			    // Pos x, y del pato
	int posIniY;			    // Pos Y del pato al inicio 
	int posReboteY;				// Pos Y donde el pato debe rebotar
	float speed;				// Velocidad del pato
	double w, h,				// Ancho y alto del sprite del pato
		   angle,				// Angulo de rebote
		   lastChangeFrame,		// Tiempo del último cambio de frame en las animaciones
		   timeSinceStart;		// Tiempo pasado desde que ha nacido el pato
	bool alive;			        // Pato aun dentro de su TIME_TO_LIVE
	bool destroy;		        // Destruir el pato tras pasar TIME_TO_LIVE
	bool estampado;		        // Estado del pato justo al ser disparado
	bool cayendo;		        // Estado del pato cayendo despues de ser disparado
	int anim[4] = { 0,1,2,1 };
	int contFrames;							// Contador de frames para la animación de volar
	int offsetColor, offsetAnim, frameAct;  //0 1 2 1 0 ...
	std::vector<Sprite*> spritePatos;		// Sprites de los patos
	std::vector<Sprite*> spritePuntos;		// Sprites de los puntos
	double scaleFactor;						// Escalado del sprite según el tamaño de ventana

	int wWidth, wHeight;		// Tamaño ventana

	int xPuntos, yPuntos;		// Posicion donde se muestran los puntos = donde muere el pato
	int wPuntos, hPuntos;
	Tipo tipo;
	uint32_t puntos;

	double timeToChangeSprite;

	void setAnim();				// Establecer la anim de vuelo según el ángulo
public:
	Pato(std::vector<Sprite*>& sprite_, std::vector<Sprite*>& puntos_, int posIniY, int posReboteY_);
	bool readyToErase() { return destroy; };
	bool checkCollision(int posX, int posY);
	void update(double deltaTime);
	void render(RenderThread& rt);
	void setEstampado(bool e);
	uint32_t getPoints();
};