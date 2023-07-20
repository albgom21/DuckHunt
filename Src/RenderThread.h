#pragma once
#include "ConcurrentQueue.h"
#include "Sprite.h"
#include<atomic>

enum CommandType { RENDER, CLEAR, END_FRAME, MARCADOR}; // Tipos de comando, renderizar, limpiar y acabar frame

struct RenderParams { // Atributos para el comando de render
	Sprite* sprite;
	int tx1, ty1, tx2, ty2;
};

struct ClearParams { // Atributos para el comando de clear
	uint32_t color;
};

struct MarcadorParams { // Atributos para el comando marcador
	uint32_t numero;
	Sprite** sprites;
	int x, y;
	int w, h;
	int nDigitos;
};

union CommandParams { // Posibles atributos de los comandos hecho union
	RenderParams render;
	ClearParams clear;
	MarcadorParams marcador;
};

struct Command {  // Estructura de un comando su tipo y parametros
	CommandType type;
	CommandParams params;
};

class RenderThread {
public:
	RenderThread();
	void start();
	void stop();
	void addCommand(Command c);

	int getPendingFrames();

private:
	ConcurrentQueue<Command> queue;
	std::thread* threadRender;
	volatile bool end; 
	std::atomic<int> pendingFrames; // Frames pendientes

	// Bucle principal del hilo
	void loopFunc();
};