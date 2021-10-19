#pragma once

#include "Black_Cat/Base/entt.hpp"
#include "Black_Cat/Components/RenderSystemComponents.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace BLK_Cat {
	void InitTransformHandler(entt::registry& registry);
	void TransformUpdade(entt::registry& registry, float dt);
	glm::mat4 GetModel(Transform& transform);

	void InitCameraHandler(entt::registry& registry);
	void CameraUpdate(entt::registry& registry, Display& display ,float dt);
	glm::mat4 GetViewProjection(Camera& camera);
}