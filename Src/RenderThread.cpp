#include "RenderThread.h"
#include "Renderer.h"

RenderThread::RenderThread() {
	end = true;
	threadRender = nullptr;
	pendingFrames = 0;
}

void RenderThread::start() { //Crear un hilo nuevo con la función loop
	end = false;
	threadRender = new std::thread(&RenderThread::loopFunc, this);
}

void RenderThread::stop() { // Finalización del hilo
	end = true;

	Command c; // evitar interbloqueo
	c.type = END_FRAME;
	queue.push(c);

	if (threadRender != nullptr)
		threadRender->join(); // Esperar a que termine el hilo

	// Eliminar la instancia
	delete threadRender;
	threadRender = nullptr;
}

void RenderThread::addCommand(Command c) {
	if (c.type == END_FRAME) // Si el comando que se añade es de tipo END_FRAME, añadir un frame pendiente
		pendingFrames++;

	queue.push(c); // Añadir a la cola
}

void RenderThread::loopFunc() {
	while (!end) {
		Command c = queue.pop(); // Obtener comando de la cola
		while (c.type != END_FRAME && !end) {
			if (c.type == RENDER) { // Lógica del renderizado
				Sprite* aux = c.params.render.sprite;
				//programacion defensiva, el puntero no sea nullptr
				aux->drawSprite(c.params.render.tx1, c.params.render.ty1, c.params.render.tx2, c.params.render.ty2); // rect dest
			}
			else if (c.type == CLEAR) {  // Lógica del limpiado
				Renderer::Instance()->clear(c.params.clear.color);
			}
			else if (c.type == MARCADOR) {  // Lógica de dibujar numeros del marcador
				uint32_t n = c.params.marcador.numero;
				int nDigitos = c.params.marcador.nDigitos;

				char numeroTexto[10];
				std::string s = "%0" + std::to_string(nDigitos) + "d";
				sprintf(numeroTexto, s.c_str(), n);
				Sprite* aux;
				for (int i = 0; i < nDigitos; i++) {
					int xActual = c.params.marcador.x + (c.params.marcador.w * i);
					aux = c.params.marcador.sprites[numeroTexto[i] - '0'];
					if (aux != nullptr) //programacion defensiva, el puntero no sea nullptr
						aux->drawSprite(xActual, c.params.marcador.y,
							xActual + c.params.marcador.w, c.params.marcador.y + c.params.marcador.h);
				}
			}
			c = queue.pop(); // Obtener comando de la cola
		}
		Renderer::Instance()->present(); // Si el frame se ha terminado mostrarlo por pantalla
		pendingFrames--; // Eliminar el frame dibujado como frame pendiente
	}
}

int RenderThread::getPendingFrames() {
	return pendingFrames;
}
