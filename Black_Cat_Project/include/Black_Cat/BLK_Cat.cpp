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

	float width = 50.0f;
	float height = 50.0f;

	entt::entity quad = _registry.create();
	_registry.emplace<BLK_Cat::Quad>(quad);
	_registry.emplace<BLK_Cat::Shader>(quad, "./res/textureShader");
	_registry.emplace<BLK_Cat::Transform>(quad, glm::vec3(_width/2, _height/2, 0), glm::vec3(), glm::vec3(width, height, 1.0));
	_registry.emplace<BLK_Cat::CameraOrtho>(quad, (float_t)0, (float_t)_width, (float_t)0, (float_t)_height, -1.0f, 1.0f, glm::vec3());
	_registry.emplace<BLK_Cat::Duplicate>(quad);
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


		if (_display->KeyPressed(SDL_SCANCODE_ESCAPE))
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
	BLK_Cat::TransformUpdade(_registry, dt, *_display);
	BLK_Cat::CameraUpdate(_registry, *_display ,dt);
	BLK_Cat::HelpSystemsUpdate(_registry, dt, *_display);
}
