#include "BLK_Cat.h"

Engine::Engine(int width, int height, std::string& tittle)
{
	_mgr = new BLK_Cat::EntityManager();

	_canvas = new BLK_Cat::Entity(_mgr->addNewEntity(), _mgr);
	_canvas->addComponent<BLK_Cat::Canvas>(width, height, tittle);

	_mgr->registerSystem<BLK_Cat::Renderer>(_mgr);
}


Engine::~Engine()
{

}

void Engine::init()
{
	_mgr->init();

	//game loop
	while (!_mgr->getComponent<BLK_Cat::Canvas>(_canvas->getID())._display->isClosed())
	{
		_mgr->getComponent<BLK_Cat::Canvas>(_canvas->getID())._display->clear(0.0f, 0.15f, 0.3f, 1.0f);
		_mgr->render(_canvas->getID());
		_mgr->getComponent<BLK_Cat::Canvas>(_canvas->getID())._display->swapBuffers();
	}
}

void Engine::activateScene(BLK_Cat::Scene* scene)
{
	scene->addComponent<ActivateScene>();
}

void Engine::changeScene()
{

}

void Engine::closeScene()
{

}

void Engine::render()
{
	_mgr->render(_canvas->getID());
}

void Engine::update(float dt)
{

}
