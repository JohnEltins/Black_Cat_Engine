#pragma once
#include "Black_Cat/Base/Scene.h"
#include "Black_Cat/BLK_Cat.h"

class TestScene : public BLK_Cat::Scene
{
public:
	TestScene(const BLK_Cat::EntityID id, BLK_Cat::EntityManager* manager, int width, int height)
		: BLK_Cat::Scene(id, manager)
	{
		std::cout << "cena carregada" << std::endl;

		_quad = new BLK_Cat::Entity(manager->addNewEntity(), manager);
		_tri = new BLK_Cat::Entity(manager->addNewEntity(), manager);

		_camera = new BLK_Cat::Entity(manager->addNewEntity(), manager);
		_camera->addComponent<BLK_Cat::Camera>(glm::vec3(0, 0, -3), 70.0f, (float)width / (float)height, 0.01f, 1000.0f);


		_quad->addComponent<BLK_Cat::Quad>();
		_quad->addComponent<BLK_Cat::Drawable>();
		_quad->addComponent<BLK_Cat::Shader>("./res/textureShader");
		_quad->addComponent<BLK_Cat::Texture>("./res/bricks.jpg");
		_quad->addComponent< BLK_Cat::Transform>(glm::vec3(), glm::vec3(), glm::vec3(0.5, 0.5, 1.0));


		_tri->addComponent<BLK_Cat::Triangle>();
		_tri->addComponent<BLK_Cat::Drawable>();
		_tri->addComponent<BLK_Cat::Shader>("./res/solidColorShader");
		_tri->addComponent< BLK_Cat::Transform>(glm::vec3(), glm::vec3(), glm::vec3(0.5, 0.5, 1.0));

		manager->registerSystem<BLK_Cat::TransformHandler>(manager);
		manager->registerSystem<BLK_Cat::CameraHandler>(manager);

		manager->registerSystem < BLK_Cat::DrawTriangle>(manager);
		manager->registerSystem<BLK_Cat::DrawQuad>(manager);
		manager->registerSystem<BLK_Cat::ShaderHandler>(manager);
		manager->registerSystem<BLK_Cat::TextureHandler>(manager);
	}

	~TestScene() = default;

	BLK_Cat::Entity* _quad;
	BLK_Cat::Entity* _tri;
	BLK_Cat::Entity* _camera;

};

class TestScene2 : public BLK_Cat::Scene
{
public:
	TestScene2(const BLK_Cat::EntityID id, BLK_Cat::EntityManager* manager)
		: BLK_Cat::Scene(id, manager)
	{
		std::cout << "cena carregada" << std::endl;


		_quad = new BLK_Cat::Entity(manager->addNewEntity(), manager);
		_tri = new BLK_Cat::Entity(manager->addNewEntity(), manager);

		_quad->addComponent<BLK_Cat::Quad>();
		_quad->addComponent<BLK_Cat::Drawable>();
		_quad->addComponent<BLK_Cat::Shader>("./res/solidColorShader");
	
		_quad->addComponent< BLK_Cat::Transform>(glm::vec3(), glm::vec3(), glm::vec3(0.5, 0.5, 1.0));

		_tri->addComponent<BLK_Cat::Triangle>();
		_tri->addComponent<BLK_Cat::Drawable>();
		_tri->addComponent<BLK_Cat::Shader>("./res/textureShader");
		_tri->addComponent<BLK_Cat::Texture>("./res/bricks.jpg");
		_tri->addComponent< BLK_Cat::Transform>(glm::vec3(), glm::vec3(), glm::vec3(0.5, 0.5, 1.0));

		manager->registerSystem < BLK_Cat::DrawTriangle>(manager);
		manager->registerSystem<BLK_Cat::DrawQuad>(manager);
		manager->registerSystem<BLK_Cat::ShaderHandler>(manager);
		manager->registerSystem<BLK_Cat::TextureHandler>(manager);
		manager->registerSystem<BLK_Cat::TransformHandler>(manager);
	}

	~TestScene2() = default;

	BLK_Cat::Entity* _quad;
	BLK_Cat::Entity* _tri;
};