#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Black_Cat/Base/Component.h"

namespace BLK_Cat {
	struct Transform : public Component
	{
		Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(0.2, 0.2, 1.0))
			: _pos(pos), _rot(rot), _scale(scale) {}


		glm::vec3 _pos;
		glm::vec3 _rot;
		glm::vec3 _scale;

		glm::mat4 _model;
	};
}
