#include "BLK_Cat.h"
#include "Systems/RenderSystem.h"
#include "Systems/TransformSystem.h"
//#include "Components/RenderSystemComponents.h"

Engine* Engine::_instance = nullptr;

Engine::Engine(int width, int height, std::string& tittle)
{
	_width = width;
	_height = height;
	_tittle = tittle;
	_display = Display::GetInstace(width, height, tittle);
}


Engine::~Engine()
{

}

static void strs(int a)
{
	std::cout << a << std::endl;
}

void Engine::init()
{
	entt::entity tri = _registry.create();
	_registry.emplace<BLK_Cat::Triangle>(tri);
	_registry.emplace<BLK_Cat::Shader>(tri, "./res/solidColorShader");
	_registry.emplace<BLK_Cat::Transform>(tri, glm::vec3(), glm::vec3(), glm::vec3(0.5, 0.5, 1.0));
	_registry.emplace<BLK_Cat::Camera>(tri, glm::vec3(0, 0, -3), 70.0f, (float)_width / (float)_height, 0.01f, 1000.0f);
	_registry.emplace<BLK_Cat::Texture>(tri, "./res/bricks.jpg");

	
	entt::entity quad = _registry.create();
	_registry.emplace<BLK_Cat::Quad>(quad);
	_registry.emplace<BLK_Cat::Shader>(quad, "./res/textureShader");
	_registry.emplace<BLK_Cat::Transform>(quad, glm::vec3(), glm::vec3(), glm::vec3(0.5, 0.5, 1.0));
	_registry.emplace<BLK_Cat::Camera>(quad, glm::vec3(0, 0, -3), 70.0f, (float)_width / (float)_height, 0.01f, 1000.0f);
	_registry.emplace<BLK_Cat::Texture>(quad, "./res/bricks.jpg");

	BLK_Cat::Vertex vertices[] = {
				BLK_Cat::Vertex(glm::vec3(-0.5f, -0.5f, 0.0), glm::vec2(0.0, 0.0)),
				BLK_Cat::Vertex(glm::vec3(0.0,  0.5f, 0.0),  glm::vec2(0.5, 1.0)),
				BLK_Cat::Vertex(glm::vec3(0.5f, -0.5f, 0.0),  glm::vec2(1.0, 0.0))
	};

	uint32_t indices[] = {
			0, 1, 2
	};

	entt::entity mesh = _registry.create();
	_registry.emplace<BLK_Cat::Mesh>(mesh, vertices, 
		sizeof(vertices) / sizeof(vertices[0]), 
		indices, sizeof(indices) / sizeof(indices[0]));

	_registry.emplace<BLK_Cat::Shader>(mesh, "./res/textureShader");
	_registry.emplace<BLK_Cat::Transform>(mesh, glm::vec3(), glm::vec3(), glm::vec3(0.5, 0.5, 1.0));
	_registry.emplace<BLK_Cat::Camera>(mesh, glm::vec3(0, 0, -3), 70.0f, (float)_width / (float)_height, 0.01f, 1000.0f);
	_registry.emplace<BLK_Cat::Texture>(mesh, "./res/bricks.jpg");
	
	BLK_Cat::RendererInit(_registry);
	BLK_Cat::InitCameraHandler(_registry);
	BLK_Cat::InitTransformHandler(_registry);

	//game loop
	while (!_display->IsClosed())
	{
		_display->Listener();
		_display->UpdateNow();


		if (_display->GetKeyDown(SDL_SCANCODE_ESCAPE))
			_display->Close();

		update(_display->GetDeltaTime());

		_display->Clear(0.0f, 0.15f, 0.3f, 1.0f);

		BLK_Cat::RendererDraw(_registry);

		_display->SwapBuffers();
		_display->Tick();
		_display->UpdatePrev();

	}
}

/*
void Engine::activateScene(BLK_Cat::Scene* scene)
{
	scene->addComponent<ActivateScene>();
}
*/


void Engine::changeScene()
{

}

void Engine::closeScene()
{

}


void Engine::update(float dt)
{

	//std::cout << dt << std::endl;
	BLK_Cat::TransformUpdade(_registry, dt);
	BLK_Cat::CameraUpdate(_registry, *_display ,dt);

	if(_display->GetMouseDown(Display::middle)) std::cout << "middle" << std::endl;
	if (_display->MousePressed(Display::left)) std::cout << "left" << std::endl;
	if (_display->MouseReleased(Display::right)) std::cout << "right" << std::endl;


}
