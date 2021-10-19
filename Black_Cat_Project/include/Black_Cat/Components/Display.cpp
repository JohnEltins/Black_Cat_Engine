#include "Display.h"
#include <GL/glew.h>
#include <iostream>


const char* glsl_version = "#version 330";


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

	_width = width;
	_height = height;

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
	initImgui();
}

Display::~Display()
{
	SDL_GL_DeleteContext(_glContext);
	SDL_DestroyWindow(_window);
	SDL_Quit();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
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
		case SDL_QUIT: Close(); break;
		case SDL_MOUSEWHEEL: _scrollY = e.wheel.y; break;
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
}

void Display::initImgui()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;

	}

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(_window, _glContext);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void Display::renderImgui()
{
	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void Display::beginImgui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();;
	ImGui::NewFrame();
}

void Display::endImgui()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(float(GetScreenWidth()), float(GetScreenHeight()));

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags | ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup = SDL_GL_GetCurrentWindow();
		
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup, _glContext);
	}
}
