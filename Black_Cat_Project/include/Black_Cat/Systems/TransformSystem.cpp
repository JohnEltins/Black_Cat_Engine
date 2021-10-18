#include "TransformSystem.h"
#include "Black_Cat/Components/Display.h"
#include <iostream>

void BLK_Cat::InitTransformHandler(entt::registry& registry)
{
	auto view = registry.view<Transform>();

	for (auto entity : view)
	{
		Transform& tranform = view.get<Transform>(entity);
		tranform._model = GetModel(tranform);
		tranform.count = 0.0f;
	}
}

void BLK_Cat::TransformUpdade(entt::registry& registry, float dt)
{
	auto viewTriangle = registry.view<Transform, Triangle>();
	auto viewQuad = registry.view<Transform, Quad>();
	auto viewMesh = registry.view<Transform, Mesh>();

	for (auto entity : viewTriangle)
	{
		Transform& tranform = viewTriangle.get<Transform>(entity);

		tranform._pos.y = cosf(tranform.count / 2);

		tranform._rot.x = -1 * tranform.count;
		tranform._rot.y = -1 * tranform.count;
		tranform._rot.z = -1 * tranform.count;

		tranform.count += dt * 0.05f;
		tranform._model = GetModel(tranform);
	}

	for (auto entity : viewQuad)
	{
		Transform& tranform = viewQuad.get<Transform>(entity);

		tranform._pos.x = cosf(tranform.count / 2);

		tranform._rot.x = -1 * tranform.count;
		tranform._rot.y = -1 * tranform.count;
		tranform._rot.z = -1 * tranform.count;

		tranform.count += dt * 0.05f;
		tranform._model = GetModel(tranform);
	}

	for (auto entity : viewMesh)
	{
		Transform& tranform = viewMesh.get<Transform>(entity);

		tranform._pos.y = -1 * cosf(tranform.count / 2);

		tranform._rot.x = -1 * tranform.count;
		tranform._rot.y = -1 * tranform.count;
		tranform._rot.z = -1 * tranform.count;

		tranform.count += dt * 0.05f;
		tranform._model = GetModel(tranform);
	}
}

glm::mat4 BLK_Cat::GetModel(Transform& transform)
{
	glm::mat4 posMatrix = glm::translate(transform._pos);

	glm::mat4 rotXMatrix = glm::rotate(transform._rot.x, glm::vec3(1, 0, 0));
	glm::mat4 rotYMatrix = glm::rotate(transform._rot.y, glm::vec3(0, 1, 0));
	glm::mat4 rotZMatrix = glm::rotate(transform._rot.z, glm::vec3(0, 0, 1));

	glm::mat4 scaleMatrix = glm::scale(transform._scale);

	//ordem da multiplicação de matriz importa, X, Y Z, !ordem oposta!

	glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

	//escala, gira e movimenta, nessa ordem
	return posMatrix * rotMatrix * scaleMatrix;
}

void BLK_Cat::InitCameraHandler(entt::registry& registry)
{
	auto view = registry.view<Camera>();

	for (auto entity : view)
	{
		Camera& camera = view.get<Camera>(entity);
		camera._viewProjection = GetViewProjection(camera);
	}

}

void BLK_Cat::CameraUpdate(entt::registry& registry, Display& display ,float dt)
{
	auto view = registry.view<Camera>();
	for (auto entity : view)
	{
		glm::vec2 move = glm::vec2((float)display.GetKeyDown(SDL_SCANCODE_RIGHT) - display.GetKeyDown(SDL_SCANCODE_LEFT),
			(float)display.GetKeyDown(SDL_SCANCODE_DOWN) - display.GetKeyDown(SDL_SCANCODE_UP));

		float zoom = display.GetKeyDown(SDL_SCANCODE_PAGEUP) - display.GetKeyDown(SDL_SCANCODE_PAGEDOWN);

		//move = glm::normalize(move);
		if (move.x && move.y)
		{
			float x = move.x / sqrtf(pow(move.x, 2) + pow(move.y, 2));
			float y = move.y / sqrtf(pow(move.x, 2) + pow(move.y, 2));
			move.x = x;
			move.y = y;
		}
		
		std::cout << move.x << std::endl;
		

		Camera& camera = view.get<Camera>(entity);
		camera._pos.x -= (dt * 0.05f) * move.x;
		camera._pos.y -= (dt * 0.05f) * move.y;
		camera._pos.z += (dt * 0.08f) * zoom;


		camera._viewProjection = GetViewProjection(camera);
	}
}

glm::mat4 BLK_Cat::GetViewProjection(Camera& camera)
{
	return camera._perspective * glm::lookAt(camera._pos, (camera._pos + camera._forward), camera._up);
}
