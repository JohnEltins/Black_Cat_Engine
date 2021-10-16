#pragma once

#include "Base/EntityManager.h"
#include "Base/Entity.h"
#include "Base/Scene.h"

#include "Components/Canvas.h"
#include "Components/2D_primitives.h"

#include "Systems/Renderer.h"
#include "Black_Cat/Systems/Renderer2D.h"
#include "Systems/TransformHandler.h"

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