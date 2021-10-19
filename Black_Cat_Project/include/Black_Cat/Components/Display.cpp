#include "Display.h"
#include <GL/glew.h>
#include <iostream>

Display* Display::_instance = nullptr;

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

	_keyStates = SDL_GetKeyboardState(&_keyLenght);
	_prevKeyStates = new Uint8[_keyLenght];
	_scrollY = 0;
	memcpy(_prevKeyStates, _keyStates, _keyLenght);

	this->_isClosed = false;
	Tick();
}

Display::~Display()
{
	SDL_GL_DeleteContext(_glContext);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Display::SwapBuffers()
{
	SDL_GL_SwapWindow(_window);
}

bool Display::IsClosed()
{
	return _isClosed;
}

void Display::Clear(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Display::Listener()
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT: _isClosed = true; break;
		case SDL_MOUSEWHEEL: 
			std::cout << e.wheel.y << std::endl;
			_scrollY = e.wheel.y;
			break;
		}
	}
}

void Display::UpdateNow()
{
	_mouseState = SDL_GetMouseState(&_mouseX, &_mouseY);
	SDL_GetRelativeMouseState(&_dx, &_dy);
	_keyStates = SDL_GetKeyboardState(&_keyLenght);
}

void Display::UpdatePrev()
{
	_prevMouseState = _mouseState;
	memcpy(_prevKeyStates, _keyStates, _keyLenght);
	_scrollY = 0;
}

bool Display::GetKeyDown(SDL_Scancode key)
{
	return _keyStates[key];
}

bool Display::KeyPressed(SDL_Scancode key)
{
	return !_prevKeyStates[key] && _keyStates[key];
}

bool Display::KeyRelesaed(SDL_Scancode key)
{
	return !_prevKeyStates[key] && _keyStates[key];
}


bool Display::GetMouseDown(MOUSE_BUTTON button)
{
	Uint32 mask = 0;
	switch (button)
	{
	case left:
		mask = SDL_BUTTON_LMASK;
		break;
	case right:
		mask = SDL_BUTTON_RMASK;
		break;
	case  middle:
		mask = SDL_BUTTON_MMASK;
		break;
	case back :
		mask = SDL_BUTTON_X1MASK;
		break;
	case forward:
		mask = SDL_BUTTON_X2MASK;
	}

	return (_mouseState & mask);
}

bool Display::MousePressed(MOUSE_BUTTON button)
{
	Uint32 mask = 0;
	switch (button)
	{
	case left:
		mask = SDL_BUTTON_LMASK;
		break;
	case right:
		mask = SDL_BUTTON_RMASK;
		break;
	case  middle:
		mask = SDL_BUTTON_MMASK;
		break;
	case back:
		mask = SDL_BUTTON_X1MASK;
		break;
	case forward:
		mask = SDL_BUTTON_X2MASK;
	}

	return !(_prevMouseState & mask) && (_mouseState & mask);
}

bool Display::MouseReleased(MOUSE_BUTTON button)
{
	Uint32 mask = 0;
	switch (button)
	{
	case left:
		mask = SDL_BUTTON_LMASK;
		break;
	case right:
		mask = SDL_BUTTON_RMASK;
		break;
	case  middle:
		mask = SDL_BUTTON_MMASK;
		break;
	case back:
		mask = SDL_BUTTON_X1MASK;
		break;
	case forward:
		mask = SDL_BUTTON_X2MASK;
	}

	return (_prevMouseState & mask) && !(_mouseState & mask);
}

void Display::Close()
{
	_isClosed = true;
}

void Display::Tick()
{
	_deltaTime = ((float)SDL_GetTicks() - _lastTime) * (TARGET_FPS / 1000.0f);
	if (_deltaTime > TARGET_DELTA_TIME)
		_deltaTime = 1.6f;

	_lastTime = (float)SDL_GetTicks();

	//std::cout << _deltaTime  << std::endl;
}
