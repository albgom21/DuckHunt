#pragma once
union SDL_Event;

// Interfaz para crear el patrón de observador
class IObserver {
public:
	virtual void ListenEvents(const SDL_Event event) = 0;
};