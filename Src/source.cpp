#include "Renderer.h"
#include "RenderThread.h"
#include "Platform.h"
#include "Input.h"
#include "Endianess.h"
#include <stdio.h>
#include <stdexcept>
#include <iostream>

#include "Pato.h"
#include "BackgroundElement.h"
#include <time.h>
#include <list>
#include "Cursor.h"
#include "Marcador.h"
#include "source.h"

#define RGBA_TO_HEX(r, g, b, a) ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + ((a & 0xff)) // Conversión de RGBA a DECIMAL

const int N_PATOS = 5;				// Número de patos en la escena
const int N_IMAGENES_TOTAL = 15;	// Número de imágenes de patos en total (en movimeinto + alcanzados)
const int N_IMAGENES_ANIM_MOV = 9;  // Número de imágenes de patos en moviento
const int N_ENTIDADES = 4;		    // Número de entidades disntintas a patos dentro de la escena (elem fondo + cursor)
const int N_ELEM_FONDO = 3;			// Número de entidades que conforman el fondo de la escena
const int N_SPRITES = 54;			// Número total de sprites que conforman los patos

int main(int argc, const char* argv[]) {
#ifndef ENDIANNESS_IS_LE
	std::cout << "Big endian" << endl;
#endif
	// Comprobar memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Establecer semilla random
	srand(time(nullptr));

	// Iniciar los modulos: platform, renderer e input
	if (!Platform::Init())
		return 1;
	if (!Renderer::Init(320 * 6, 180 * 6, "DuckHunt")) { //320*6, 180*6 <-> 1920, 1080
		Platform::Release();
		return 1;
	}
	if (!Input::Init()) {
		Renderer::Release();
		Platform::Release();
		return 1;
	}

	RenderThread rt; // Hebra para el renderizado

	// scaleFactor para el escalado del cursor y elementos del fondo
	float scaleFactor = Renderer::Instance()->getWidth() / 320.0f;  // 320 medida del suelo
	float tile = 8 * scaleFactor;									// 8 es de la res original 256/32
	//-------------------------------------------CARGA DE IMÁGENES--------------------------------------------
	Image* imgs[N_IMAGENES_TOTAL];
	std::vector<Sprite*> sprites(N_SPRITES);
	std::string rutas[] = { "assets/azul_0.rgba","assets/azul_45.rgba","assets/azul_90.rgba",
					   "assets/negro_0.rgba","assets/negro_45.rgba","assets/negro_90.rgba",
					   "assets/rojo_0.rgba","assets/rojo_45.rgba","assets/rojo_90.rgba",
					   "assets/azul_-90.rgba","assets/azul_dado.rgba",
					   "assets/negro_-90.rgba","assets/negro_dado.rgba",
					   "assets/rojo_-90.rgba","assets/rojo_dado.rgba" };
	int nFilas[] = { 2,2,1 };
	int nCols[] = { 3,3,3 };
	int nSprites[] = { 6,6,3 };

	int nCols2[] = { 2,1 };
	int nSprites2[] = { 2,1 };

	int imgActual = 0;
	int spriteSig = 0;

	// IMÁGENES DE PATOS
	for (int n = 0; n < N_IMAGENES_TOTAL; n++) {
		FILE* imagen = Platform::Instance()->openBinaryFile(rutas[n].c_str());
		int32_t w = Platform::Instance()->readInt(imagen);
		int32_t h = Platform::Instance()->readInt(imagen);

#ifdef ENDIANNESS_IS_LE		//Sabemos que viene en big endian los datos
		w = CHANGEENDIAN(w);
		h = CHANGEENDIAN(h);
#endif
		uint32_t* matrizColores;
		int tam = w * h;
		matrizColores = new uint32_t[tam];
		for (int i = 0; i < w * h; i++) {
			uint32_t r = Platform::Instance()->readByte(imagen);
			uint32_t g = Platform::Instance()->readByte(imagen);
			uint32_t b = Platform::Instance()->readByte(imagen);
			uint32_t a = Platform::Instance()->readByte(imagen);
			matrizColores[i] = RGBA_TO_HEX(r, g, b, a);
#ifndef ENDIANNESS_IS_LE
			matrizColores[i] = CHANGEENDIAN(matrizColores[i]);
#endif			
		}
		imgActual = n;

		imgs[n] = Renderer::Instance()->newImage(w, h, matrizColores);
		if (n < N_IMAGENES_ANIM_MOV) { // sprites movimiento del pato
			for (int s = 0; s < nSprites[n % 3]; s++) {
				sprites[spriteSig] = new Sprite(imgs[imgActual], nFilas[n % 3], nCols[n % 3], s / 3, s % 3);
				spriteSig++;
			}
		}
		else { //sprites pato disparado
			for (int s = 0; s < nSprites2[(n + 1) % 2]; s++) {
				sprites[spriteSig] = new Sprite(imgs[imgActual], 1, nCols2[(n + 1) % 2], 0, s);
				spriteSig++;
			}
		}
		Platform::Instance()->closeBinaryFile(imagen);
	}
	//----------------------------------------FIN CARGA DE IMÁGENES--------------------------------------------

	//----------------------------------------CARGA DEL FONDO + CURSOR--------------------------------------------------
	Image* imgFondo[N_ENTIDADES];
	std::string rutasFondo[] = { "assets/arbol.rgba","assets/arbusto.rgba", "assets/suelo.rgba" ,"assets/cursor.rgba" };
	std::vector<Sprite*> fondo(N_ENTIDADES);
	int cont = 0;
	for (int n = 0; n < N_ENTIDADES; n++) {
		FILE* imagen = Platform::Instance()->openBinaryFile(rutasFondo[n].c_str());
		int32_t w = Platform::Instance()->readInt(imagen);
		int32_t h = Platform::Instance()->readInt(imagen);

#ifdef ENDIANNESS_IS_LE		//Sabemos que viene en big endian los datos
		w = CHANGEENDIAN(w);
		h = CHANGEENDIAN(h);
#endif
		uint32_t* matrizColores2;
		int tam = w * h;
		matrizColores2 = new uint32_t[tam];
		for (int i = 0; i < w * h; i++) {
			uint32_t r = Platform::Instance()->readByte(imagen);
			uint32_t g = Platform::Instance()->readByte(imagen);
			uint32_t b = Platform::Instance()->readByte(imagen);
			uint32_t a = Platform::Instance()->readByte(imagen);
			matrizColores2[i] = RGBA_TO_HEX(r, g, b, a);
#ifndef ENDIANNESS_IS_LE
			matrizColores[i] = CHANGEENDIAN(matrizColores[i]);
#endif			
		}
		imgFondo[cont] = Renderer::Instance()->newImage(w, h, matrizColores2);
		fondo[cont] = new Sprite(imgFondo[cont], 1, 1, 0, 0);
		cont++;
		Platform::Instance()->closeBinaryFile(imagen);
	}
	//----------------------------------------FIN CARGA DE FONDO + CURSOR--------------------------------------------

	Image* imgPuntos;
	std::string rutaPuntos = "assets/puntos.rgba";
	std::vector<Sprite*> puntosSprites(3);
	FILE* imagenP = Platform::Instance()->openBinaryFile(rutaPuntos.c_str());
	int32_t w = Platform::Instance()->readInt(imagenP);
	int32_t h = Platform::Instance()->readInt(imagenP);

#ifdef ENDIANNESS_IS_LE		//Sabemos que viene en big endian los datos
	w = CHANGEENDIAN(w);
	h = CHANGEENDIAN(h);
#endif
	uint32_t* matrizColorespuntos;
	int tam = w * h;
	matrizColorespuntos = new uint32_t[tam];
	for (int i = 0; i < w * h; i++) {
		uint32_t r = Platform::Instance()->readByte(imagenP);
		uint32_t g = Platform::Instance()->readByte(imagenP);
		uint32_t b = Platform::Instance()->readByte(imagenP);
		uint32_t a = Platform::Instance()->readByte(imagenP);
		matrizColorespuntos[i] = RGBA_TO_HEX(r, g, b, a);
#ifndef ENDIANNESS_IS_LE
		matrizColorespuntos[i] = CHANGEENDIAN(matrizColorespuntos[i]);
#endif			
	}
	imgPuntos = Renderer::Instance()->newImage(w, h, matrizColorespuntos);
	puntosSprites[0] = new Sprite(imgPuntos, 4, 2, 0, 0);
	puntosSprites[1] = new Sprite(imgPuntos, 4, 2, 1, 0);
	puntosSprites[2] = new Sprite(imgPuntos, 4, 2, 1, 1);
	Platform::Instance()->closeBinaryFile(imagenP);

	//
	Image* imgDigitos;
	std::string rutaDigitos = "assets/digitosVerdes.rgba";
	Sprite** digitosSprites = new Sprite * [10];
	FILE* imagenD = Platform::Instance()->openBinaryFile(rutaDigitos.c_str());
	w = Platform::Instance()->readInt(imagenD);
	h = Platform::Instance()->readInt(imagenD);

#ifdef ENDIANNESS_IS_LE		//Sabemos que viene en big endian los datos
	w = CHANGEENDIAN(w);
	h = CHANGEENDIAN(h);
#endif
	uint32_t* matrizColoresDigitos;
	tam = w * h;
	matrizColoresDigitos = new uint32_t[tam];
	for (int i = 0; i < w * h; i++) {
		uint32_t r = Platform::Instance()->readByte(imagenD);
		uint32_t g = Platform::Instance()->readByte(imagenD);
		uint32_t b = Platform::Instance()->readByte(imagenD);
		uint32_t a = Platform::Instance()->readByte(imagenD);
		matrizColoresDigitos[i] = RGBA_TO_HEX(r, g, b, a);
#ifndef ENDIANNESS_IS_LE
		matrizColoresDigitos[i] = CHANGEENDIAN(matrizColoresDigitos[i]);
#endif			
	}
	imgDigitos = Renderer::Instance()->newImage(w, h, matrizColoresDigitos);
	for (int i = 0; i < 10; i++)
		digitosSprites[i] = new Sprite(imgDigitos, 1, 10, 0, i);
	Platform::Instance()->closeBinaryFile(imagenD);

	//----------------------------------CREACION Y POSICIONADO DE ELEMENTOS FONDO, CURSOR Y PATOS---------------
	// FONDO
	std::vector<BackgroundElement*> elemFondo(N_ELEM_FONDO);
	// Posiciones calculadas por tiles, con la imagen con grid del enunciado
	elemFondo[0] = new BackgroundElement(fondo[0], tile * 4, tile / 2, scaleFactor);				// arbol
	elemFondo[1] = new BackgroundElement(fondo[1], tile * 28, 11 * tile + (tile / 2), scaleFactor); // arbusto
	elemFondo[2] = new BackgroundElement(fondo[2], 0, 14 * tile + (tile / 2), scaleFactor);         // suelo

	// CURSOR
	Cursor* cursor = new Cursor(fondo[3],													// cursor
		Renderer::Instance()->getWidth() / 2 - (fondo[3]->getWSprite() / 2) * scaleFactor,
		Renderer::Instance()->getHeight() / 2 - (fondo[3]->getHSprite() / 2) * scaleFactor, scaleFactor,
		Renderer::Instance()->getWidth(), Renderer::Instance()->getHeight());

	// PATOS
	int posIniPatoY = (14 * tile + (tile / 2)) + sprites[0]->getHSprite(); // 14,5 tiles hacia abajo + mitad alto pato
	int posRebotePatoY = 14 * tile + (tile / 2);						   // 14,5 tiles hacia abajo 
	std::vector<Pato*> patos(N_PATOS);
	for (int i = 0; i < N_PATOS; i++)
		patos[i] = new Pato(sprites, puntosSprites, posIniPatoY, posRebotePatoY);

	double frame_time = 0;
	int fps_count = 0;
	Command c;

	bool patoAlcanzado;
	uint32_t contadorPuntos = 0;
	uint32_t contadorPatosM = 0;
	Marcador* marcadorPuntos = new Marcador(digitosSprites, 17 * tile, 20 * tile + (tile / 2.0f), scaleFactor, &contadorPuntos,6);
	Marcador* marcadorPatos = new Marcador(digitosSprites, 30 * tile, 20 * tile + (tile / 2.0f), scaleFactor, &contadorPatosM,3);
	//---------------------------------------------BUCLE PRINCIPAL---------------------------------------------
	rt.start(); // Se lanza la hebra de renderizado
	while (Platform::Instance()->tick()) {
		// Limpiar el fondo con color azul claro
		c.type = CLEAR;
		c.params.clear.color = 0x3CBCFCFF;
		rt.addCommand(c);

		// Recibir input
		Input::Instance()->tick();

		// Mover el cursor
		cursor->update(Input::Instance()->getInputState(), Platform::Instance()->getDeltaTime());
		int cursorX = cursor->getX();
		int cursorY = cursor->getY();

		// Lógica de los patos
		patoAlcanzado = false;

		auto it = patos.begin();
		while (it != patos.end()) { // Recorrido de todos los patos en escena

			// Si un pato es alcanzado el disparo no puede alcanzar a otro
			if (!patoAlcanzado && cursor->getDisparado() && (*it)->checkCollision(cursorX, cursorY)) {
				patoAlcanzado = true;
				(*it)->setEstampado(true);
				contadorPuntos +=(*it)->getPoints();
				contadorPatosM++;
			}

			(*it)->update(Platform::Instance()->getDeltaTime()); // Ejecutar paso de la simulación
			(*it)->render(rt);						             // Comando para dibujado nuevo frame

			if ((*it)->readyToErase()) { // Eliminar los patos que ya no seguiran en escena (por time out o alcanzado)
				delete* it;
				it = patos.erase(it);
			}
			else it++;
		}

		// Añadir nuevos patos si es que hay menos de los esperados
		while (patos.size() < N_PATOS)
			patos.push_back(new Pato(sprites, puntosSprites, posIniPatoY, posRebotePatoY));

		//Renderizar el fondo
		auto it2 = elemFondo.begin();
		while (it2 != elemFondo.end()) {
			(*it2)->render(rt);
			it2++;
		}

		marcadorPuntos->render(rt);
		//marcadorPatos->render(rt);
		//renderMarcador(c, tile, digitosSprites, scaleFactor, marcadorPuntos, rt);

		// Renderizar el cursor
		cursor->render(rt);


		// Final del del frame
		c.type = END_FRAME;
		rt.addCommand(c);

		// Cálculo de los FPS cada 5 segs
		if (Platform::Instance()->getTime() <= frame_time + 5.0)
			fps_count++;
		else {
			fps_count++;
			std::cout << "FPS: " << std::round((float)(fps_count / 5.0f)) << std::endl;
			fps_count = 0;
			frame_time = Platform::Instance()->getTime();
		}

		// Espera activa
		while (rt.getPendingFrames() >= 2); // 2 es el num de buffers
	}

	// Borrar patos
	for (auto p : patos) {
		delete p;
		p = nullptr;
	}

	// Esperar a que no haya frames pendientes
	rt.stop();

	// Borrar resto de entidades
	delete cursor;
	cursor = nullptr;

	delete marcadorPuntos;
	marcadorPuntos = nullptr;

	delete marcadorPatos;
	marcadorPatos = nullptr;

	delete imgPuntos;
	imgPuntos = nullptr;

	delete imgDigitos;
	imgDigitos = nullptr;

	for (int i = 0; i < 10; i++) {
		delete digitosSprites[i];
		digitosSprites[i] = nullptr;
	}

	delete digitosSprites;
	digitosSprites = nullptr;

	for (auto i : puntosSprites) {
		delete i;
		i = nullptr;
	}

	for (auto i : elemFondo) {
		delete i;
		i = nullptr;
	}

	for (auto s : fondo) {
		delete s;
		s = nullptr;
	}

	for (auto i : imgFondo) {
		delete i;
		i = nullptr;
	}

	for (auto s : sprites) {
		delete s;
		s = nullptr;
	}

	for (auto i : imgs) {
		delete i;
		i = nullptr;
	}

	// Release de los módulos
	Input::Release();
	Renderer::Release();
	Platform::Release();

	return 0;
}