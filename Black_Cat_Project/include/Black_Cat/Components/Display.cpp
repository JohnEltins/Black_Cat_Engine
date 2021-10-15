#include "Display.h"
#include <GL/glew.h>
#include <iostream>

Display::Display(int width, int height, std::string& tittle)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	//setar tons de cores, 32 bits, 256 tons de cada cor
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32); //32 bit pallete, buffer de 32 bits
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	this->_window = SDL_CreateWindow(tittle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	this->_glContext = SDL_GL_CreateContext(this->_window);


	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize" << std::endl;
	}

	this->_isClosed = false;
}

Display::~Display()
{
	SDL_GL_DeleteContext(_glContext);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Display::swapBuffers()
{
	SDL_GL_SwapWindow(_window);

	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			_isClosed = true;
		}
	}
}

bool Display::isClosed()
{
	return _isClosed;
}

void Display::clear(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);
	glClear(GL_COLOR_BUFFER_BIT);
}
