#include "Input.h"
#include <iostream>
#include <SDL.h>
#include <SDL_gamecontroller.h>

#define JOYSTICK_DEAD_ZONE  8000
#define MAX_JOYSTICK_VALUE 32767.0

InputPC* InputPC::instance_ = nullptr;

// Constructor, start, destructor and Instance getter methods---------------------------------------------------
InputPC::InputPC() {
	gameController = nullptr;
}

// Inicialización en dos pasos
bool InputPC::Init() {
	assert(instance_ == nullptr);
	instance_ = new InputPC();
	if (!instance_->auxInit()) {
		instance_->Release();
		return false;
	}
	return true;
}

bool InputPC::auxInit() {
	// Establecer en SDL el modo relativo del ratón
	if (SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE) != SDL_TRUE) return false;
	if (SDL_SetRelativeMouseMode(SDL_TRUE) != 0) return false;
	PlatformPC::Instance()->attach(instance_);
	return true;
}

InputPC::~InputPC() {
	// Eliminar de la lista de listeners el inputPC
	PlatformPC::Instance()->removeObserver(this);

	// Liberar mando
	if (SDL_NumJoysticks() > 0)
		SDL_GameControllerClose(gameController);
	gameController = nullptr;
}

void InputPC::Release() {
	delete instance_;
	instance_ = nullptr;
}

InputPC* InputPC::Instance() {
	assert(instance_ != nullptr);
	return instance_;
}

// Update-------------------------------------------------------------------------------------------------------
bool InputPC::tick()
{
	currentInputState = newInputState;
	newInputState.velAngX = newInputState.velAngY = 0;
	return true;
}

// IObserver methods--------------------------------------------------------------------------------------------
void InputPC::ListenEvents(SDL_Event event_) { // Procesar evento del input de SDL
	//------- GAMEPAD------
	if (event_.type == SDL_JOYBUTTONDOWN) { // Pulsar boton del mando
		switch (event_.jbutton.button) {
		case SDL_CONTROLLER_BUTTON_A:
		case SDL_CONTROLLER_BUTTON_B:
			newInputState.buttonO = true;
			newInputState.buttonX = true;
			break;
		}
	}
	if (event_.type == SDL_JOYBUTTONUP) { // Soltar boton del mando
		switch (event_.jbutton.button)
		{
		case SDL_CONTROLLER_BUTTON_A:
		case SDL_CONTROLLER_BUTTON_B:
			newInputState.buttonO = false;
			newInputState.buttonX = false;
			break;
		}
	}

	if (event_.type == SDL_CONTROLLERAXISMOTION) { // Movimiento joystick izquierdo del mando
		if (event_.jaxis.axis == 0) // EJE X 
		{
			//Dead zone
			if (std::abs(event_.jaxis.value) > JOYSTICK_DEAD_ZONE)
				newInputState.stickX = (float)(event_.jaxis.value / MAX_JOYSTICK_VALUE);
			else
				newInputState.stickX = 0;
		}
		if (event_.jaxis.axis == 1) // EJE Y 
		{
			//Dead zone
			if (std::abs(event_.jaxis.value) > JOYSTICK_DEAD_ZONE)
				newInputState.stickY = (float)(event_.jaxis.value / MAX_JOYSTICK_VALUE);
			else
				newInputState.stickY = 0;
		}
		if (event_.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) //l2
			newInputState.l2 = event_.caxis.value;

		if (event_.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) //r2
			newInputState.r2 = event_.caxis.value;
	}

	//------- MOUSE------
	if (event_.type == SDL_MOUSEBUTTONDOWN && (event_.button.button == SDL_BUTTON_LEFT || event_.button.button == SDL_BUTTON_RIGHT)) { // Pulsar boton der o izq
		newInputState.buttonO = true;
		newInputState.buttonX = true;
		newInputState.l2 = 1;
		newInputState.r2 = 1;
	}
	if (event_.type == SDL_MOUSEBUTTONUP && (event_.button.button == SDL_BUTTON_LEFT || event_.button.button == SDL_BUTTON_RIGHT)) { // Soltar boton der o izq
		newInputState.buttonO = false;
		newInputState.buttonX = false;
		newInputState.l2 = 0;
		newInputState.r2 = 0;
	}
	if (event_.type == SDL_MOUSEMOTION) {	// Movimiento
		newInputState.velAngX = (float)event_.motion.xrel;
		newInputState.velAngY = (float)event_.motion.yrel;
	}
	else if (event_.type != SDL_MOUSEMOTION) {
		newInputState.velAngX = 0;
		newInputState.velAngY = 0;
	}

	//-------TECLADO DOWN-------
	if (event_.type == SDL_KEYDOWN) {
		switch (event_.key.keysym.sym) {
		case SDLK_RIGHT:			 // Flecha derecha
			newInputState.stickX = 1;
			break;
		case SDLK_LEFT:			     // Flecha izquierda
			newInputState.stickX = -1;
			break;
		case SDLK_UP:				 // Flecha arriba
			newInputState.stickY = -1;
			break;
		case SDLK_DOWN:				 // Flecha abajo
			newInputState.stickY = 1;
			break;
		case SDLK_RSHIFT:			 // Shift y control derechos
		case SDLK_RCTRL:
			newInputState.buttonX = true;
			newInputState.buttonO = true;
			newInputState.l2 = 1;
			newInputState.r2 = 1;
			break;
		default:
			break;
		}
	}
	//------ TECLADO UP -----
	if (event_.type == SDL_KEYUP) {
		switch (event_.key.keysym.sym)
		{
		case SDLK_RIGHT:					 // Flecha derecha e izquierda
		case SDLK_LEFT:
			newInputState.stickX = 0;
			break;
		case SDLK_UP:						 // Flecha arriba y abajo
		case SDLK_DOWN:
			newInputState.stickY = 0;
			break;
		case SDLK_RSHIFT:					 // Shift y control derechos
		case SDLK_RCTRL:
			newInputState.buttonO = false;
			newInputState.buttonX = false;
			newInputState.l2 = 0;
			newInputState.r2 = 0;
			break;
		default:
			break;
		}
	}
	if (event_.type == SDL_JOYDEVICEADDED)    // Conectar mando
	{
		if (!gameController)
			gameController = SDL_GameControllerOpen(0);
	}
	if (event_.type == SDL_JOYDEVICEREMOVED)  // Desconectar mando
	{
		SDL_GameControllerClose(gameController);
		gameController = nullptr;
	}
}

// Getters------------------------------------------------------------------------------------------------------
InputState InputPC::getInputState() { return currentInputState; }