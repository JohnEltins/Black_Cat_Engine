#pragma once
#include <string>
#include <SDL2/SDL.h>

const float TARGET_FPS = 60.0f;
const float TARGET_DELTA_TIME = 1.5f;

class Display
{
public:
	inline static Display* GetInstace(int width, int height, std::string& tittle)
	{
		return _instance = (_instance != nullptr) ? _instance : new Display(width, height, tittle);
	}

	void SwapBuffers();
	bool IsClosed();
	void Clear(float r, float g, float b, float a);
	void Listener();
	bool GetKeyDown(SDL_Scancode key);
	void Close();
	void Tick();

	inline float GetDeltaTime() { return _deltaTime; }

private:
	Display(int width, int height, std::string& tittle);
	~Display();

	SDL_Window* _window;
	SDL_GLContext _glContext;
	bool _isClosed;
	const uint8_t* _keyStates;
	static Display* _instance;

	float _deltaTime;
	float _lastTime;
};

