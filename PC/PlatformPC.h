#pragma once
#include <chrono>
#include <list>

class IObserver;
class PlatformPC {
public:
	PlatformPC();
	static bool Init();
	static void Release();
	static PlatformPC* Instance();

	// Update
	bool tick();

	// Reading methods
	FILE* openBinaryFile(const char* fileName, const char* mode = "rb");
	void closeBinaryFile(FILE* fileName);
	int32_t readInt(FILE* fileName);
	int32_t readByte(FILE* fileName);

	// Random generator method
	float getRandom(float min, float max);
	
	// IObserver methods
	void attach(IObserver* observer);
	void removeObserver(IObserver* observer);

	// Getters
	double getTime();
	double getDeltaTime();

private:
	static PlatformPC* instance_;

	// Tiempos
	double deltaTime_;
	std::chrono::time_point<std::chrono::high_resolution_clock> initTime_;
	std::chrono::time_point<std::chrono::high_resolution_clock> time_;

	std::list<IObserver*> list_observer_;
	
	bool auxInit();
};