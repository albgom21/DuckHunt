#pragma once
#include <cassert>
#include <stack>
#include "checkML.h"
#include "PlatformPC.h"
#include "IObserver.h"
#include "InputState.h"

class _SDL_GameController;
class InputPC : public IObserver {
public:
	InputPC();
	~InputPC();
	static bool Init();
	static void Release();
	static InputPC* Instance();

	//Update
	bool tick();

	// IObserver methods
	virtual void ListenEvents(SDL_Event event) override;

	//Getters
	InputState getInputState();
private:
	static InputPC* instance_;
	InputState currentInputState, newInputState; // Estado del input	
	_SDL_GameController* gameController;		 // Referencia a un mando

	bool auxInit();
};