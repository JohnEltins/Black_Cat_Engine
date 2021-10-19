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

void BLK_Cat::TransformUpdade(entt::registry& registry, float dt, Display& display)
{
	auto viewTriangle = registry.view<Transform, Triangle>();
	auto viewQuad = registry.view<Transform, Quad, Camera>();
	auto viewQuadOrtho = registry.view<Transform, Quad, CameraOrtho>();
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

	for (auto entity : viewQuadOrtho)
	{
		Transform& tranform = viewQuadOrtho.get<Transform>(entity);

		tranform._pos.x += cosf(tranform.count * 0.55f) * 2.0f;
		tranform._pos.y += sinf(tranform.count * 0.55f) * 2.0f;

		//tranform._pos.x = tranform._pos.x * 2.0f;


		std::cout << tranform._pos.x << std::endl;

		//tranform._rot.x = -1 * tranform.count;
		//tranform._rot.y = -1 * tranform.count;
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
	auto viewProj = registry.view<Camera>();
	auto viewOrtho = registry.view<CameraOrtho>();


	for (auto entity : viewProj)
	{
		Camera& camera = viewProj.get<Camera>(entity);
		camera._viewProjection = GetViewProjection(camera);
	}

	for (auto entity : viewOrtho)
	{
		CameraOrtho& camera = viewOrtho.get<CameraOrtho>(entity);
		camera._viewOrtho = GetViewOrtho(camera);
	}

}

void BLK_Cat::CameraUpdate(entt::registry& registry, Display& display ,float dt)
{
	auto view = registry.view<Camera>();
	auto viewOrtho = registry.view<CameraOrtho>();

	for (auto entity : view)
	{
		Camera& camera = view.get<Camera>(entity);

		
		if (display.GetMouseDown(Display::right)) {

			glm::vec2 move = glm::vec2((float)display.GetMousePosD().x, (float)display.GetMousePosD().y);
			camera._pos.x += (dt * 0.005f) * move.x;
			camera._pos.y += (dt * 0.005f) * move.y;
		}
		
		camera._pos.z += (dt * 0.5f) * (float)display.MouseScroll();
		camera._viewProjection = GetViewProjection(camera);
	}

	for (auto entity : viewOrtho)
	{
		CameraOrtho& camera = viewOrtho.get<CameraOrtho>(entity);
		Transform& tranform = registry.get<Transform>(entity);

		if (display.GetMouseDown(Display::right)) {

			glm::vec2 move = glm::vec2((float)display.GetMousePosD().x, (float)display.GetMousePosD().y);
			camera._increment.x -= (dt * 0.5f) * move.x;
			camera._increment.y += (dt * 0.5f) * move.y;
		}

		tranform._scale.x += (dt * 0.5f) * (float)display.MouseScroll() * 15.0f;
		tranform._scale.y += (dt * 0.5f) * (float)display.MouseScroll() * 15.0f;

		tranform._model = GetModel(tranform);
		camera._viewOrtho = GetViewOrtho(camera);
	}
}

glm::mat4 BLK_Cat::GetViewProjection(Camera& camera)
{
	return camera._perspective * glm::lookAt(camera._pos, (camera._pos + camera._forward), camera._up);
}

glm::mat4 BLK_Cat::GetViewOrtho(CameraOrtho& camera)
{
	return camera._viewOrtho = glm::ortho(	camera._cameraSet[0][0] + camera._increment.x, camera._cameraSet[1][0] + camera._increment.x,
											camera._cameraSet[0][1] + camera._increment.y, camera._cameraSet[1][1] + camera._increment.y,
											camera._cameraSet[0][2] + camera._increment.z, camera._cameraSet[1][2] + camera._increment.z);
}
