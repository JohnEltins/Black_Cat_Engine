#pragma once

//#include "Systems/RenderSystem.h"
//#include "Systems/TransformSystem.h"
#include "Black_Cat/Base/entt.hpp"
#include "Components/Display.h"
#include "Systems/HelpSystems.h"

class Engine
{
public:
	inline static Engine* GetInstace(int width, int height, std::string& tittle)
	{
		return _instance = (_instance != nullptr) ? _instance : new Engine(width, height, tittle);
	}

	void init();

	//void activateScene(BLK_Cat::Scene* scene);
	void changeScene();
	void closeScene();
	void update(float dt);

private:
	Engine(int width, int height, std::string& tittle);
	~Engine();

	entt::registry _registry; //manager
	uint32_t _width;
	uint32_t _height;
	std::string _tittle;
	Display* _display;
	static Engine* _instance;
};