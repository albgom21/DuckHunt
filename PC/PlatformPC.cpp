#include "Platform.h"
#include <cassert>
#include "checkML.h"
#include "IObserver.h"
#include <iostream>
#include <SDL.h>

PlatformPC* PlatformPC::instance_ = nullptr;

// Constructor, start, destructor and Instance getter methods---------------------------------------------------
PlatformPC::PlatformPC() {
	time_ = std::chrono::high_resolution_clock::now();
	deltaTime_ = 0;
	initTime_ = std::chrono::high_resolution_clock::now();
}

// Inicialización en dos pasos
bool PlatformPC::Init() {
	assert(instance_ == nullptr);
	instance_ = new PlatformPC();
	if (!instance_->auxInit()) {
		instance_->Release();
		return false;
	}
	return true;
}

bool PlatformPC::auxInit() {
	if ((SDL_Init(SDL_INIT_EVERYTHING) != 0)) {
		std::cout << ("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

void PlatformPC::Release() {
	SDL_Quit();
	delete instance_;
	instance_ = nullptr;
}

PlatformPC* PlatformPC::Instance() {
	assert(instance_ != nullptr);
	return instance_;
}

//Update-------------------------------------------------------------------------------------------------------
bool PlatformPC::tick() {
	SDL_Event event;
	// Recibir eventos de SDL
	while (SDL_PollEvent(&event)) {
		// Si se pulsa escape o el aspa de la ventana cerrar el juego
		if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || event.type == SDL_QUIT) return false;

		// Enviar a los listener el evento de SDL (inputPC)
		for (auto listener : list_observer_)
			listener->ListenEvents(event);
	}
	// Obtener tiempos
	std::chrono::time_point<std::chrono::high_resolution_clock> timeNow = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = timeNow - time_;
	deltaTime_ = elapsed.count();
	time_ = timeNow;

	return true;
}


//Reading methods----------------------------------------------------------------------------------------------
FILE* PlatformPC::openBinaryFile(const char* fileName, const char* mode) { return fopen(fileName, mode); }

void PlatformPC::closeBinaryFile(FILE* fileName) { fclose(fileName); }

int32_t PlatformPC::readInt(FILE* fileName) {
	int32_t num;
	fread(&num, sizeof(int), 1, fileName); // Read 4 Bytes
	return num;
}

int32_t PlatformPC::readByte(FILE* fileName) {
	int32_t num;
	fread(&num, 1, 1, fileName);
	return num;
}

//Random generator method--------------------------------------------------------------------------------------
float PlatformPC::getRandom(float min_, float max_) {
	return float(min_ + rand() % (((int)max_ + 1) - ((int)min_)));
}

//IObserver methods--------------------------------------------------------------------------------------------
void PlatformPC::attach(IObserver* observer)
{
	list_observer_.push_back(observer); // Meter en la cola un observador
}

void PlatformPC::removeObserver(IObserver* observer)
{
	// Buscar en la lista el observer proporcionado y si existe eliminarlo de la lista
	auto it = std::find(list_observer_.begin(), list_observer_.end(), observer);
	if (it != list_observer_.end())
		list_observer_.erase(it);
}


//Getters------------------------------------------------------------------------------------------------------
double PlatformPC::getTime() {
	std::chrono::time_point<std::chrono::high_resolution_clock> currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = currentTime - initTime_;
	return (double)elapsed.count();
}

double PlatformPC::getDeltaTime() {
	return deltaTime_;
}