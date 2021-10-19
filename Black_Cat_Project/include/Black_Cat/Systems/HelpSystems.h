#pragma once

#include "Black_Cat/Base/entt.hpp"
#include "Black_Cat/Components/RenderSystemComponents.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace BLK_Cat {
	void HelpSystemsUpdate(entt::registry& registry, float dt, Display& display);
	void DuplicateEntity(entt::registry& registry, entt::entity entity, Display& display);
}
