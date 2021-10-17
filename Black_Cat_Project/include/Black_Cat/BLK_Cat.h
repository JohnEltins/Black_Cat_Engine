#pragma once

#include "Systems/RenderSystem.h"
#include "Systems/TransformSystem.h"

class Engine
{
public:
	Engine(int width, int height, std::string& tittle);
	~Engine();

	void init();

	void activateScene(BLK_Cat::Scene* scene);
	void changeScene();
	void closeScene();

	inline BLK_Cat::EntityManager* getEngineManager() { return _mgr; }

	void render();
	void update(float dt);

	BLK_Cat::EntityManager* _mgr;
	BLK_Cat::Entity* _canvas;
};

struct ActivateScene : BLK_Cat::Component
{
	ActivateScene() {}

	virtual ~ActivateScene(){}
};