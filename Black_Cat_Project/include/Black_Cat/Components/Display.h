#pragma once
#include <string>
#include <SDL2/SDL.h>

class Display
{
public:
	Display(int width, int height, std::string& tittle);
	virtual ~Display();

	void swapBuffers();
	bool isClosed();
	void clear(float r, float g, float b, float a);

private:
	//Display(const Display& other ) {}
	void operator=(const Display& other) {}

	SDL_Window* _window;
	SDL_GLContext _glContext;
	bool _isClosed;
};

