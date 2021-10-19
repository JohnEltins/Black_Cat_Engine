#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "glm/glm.hpp"

const float TARGET_FPS = 60.0f;
const float TARGET_DELTA_TIME = 1.5f;

class Display
{
public:
	enum MOUSE_BUTTON { left = 0, right, middle, back, forward };

	inline static Display* GetInstace(int width, int height, std::string& tittle)
	{
		return _instance = (_instance != nullptr) ? _instance : new Display(width, height, tittle);
	}

	void SwapBuffers();
	bool IsClosed();
	void Clear(float r, float g, float b, float a);
	void Listener();

	void UpdateNow();
	void UpdatePrev();

	bool GetKeyDown(SDL_Scancode key);
	bool KeyPressed(SDL_Scancode key);
	bool KeyRelesaed(SDL_Scancode key);

	bool GetMouseDown(MOUSE_BUTTON button);
	bool MousePressed(MOUSE_BUTTON button);
	bool MouseReleased(MOUSE_BUTTON button);


	void Close();
	void Tick();

	inline glm::vec2 GetMousePos() { return glm::vec2(_mouseX, _mouseY); }
	inline glm::vec2 GetMousePosD() { return glm::vec2(_dx, _dy); }
	inline int MouseScroll() { return _scrollY; }


	inline float GetDeltaTime() { return _deltaTime; }
private:
	Display(int width, int height, std::string& tittle);
	~Display();

	SDL_Window* _window;
	SDL_GLContext _glContext;
	bool _isClosed;
	int _keyLenght;
	const uint8_t* _keyStates;
	uint8_t* _prevKeyStates;

	Uint32 _mouseState;
	Uint32 _prevMouseState;


	int _mouseX;
	int _mouseY;
	int _dx;
	int _dy;

	int _scrollY;

	static Display* _instance;

	float _deltaTime;
	float _lastTime;
};

