#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Black_Cat/Base/Component.h"

namespace BLK_Cat {
	struct Transform : public Component
	{
		Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(0.2, 0.2, 1.0))
			: _pos(pos), _rot(rot), _scale(scale), _model(glm::mat4(1.0f)), count(0.0f) {}

		glm::vec3 _pos;
		glm::vec3 _rot;
		glm::vec3 _scale;

		glm::mat4 _model;

		float count;
	};

	struct Camera : public Component
	{
		Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar) 
		{
			_perspective = glm::perspective(fov, aspect, zNear, zFar);
			_pos = pos;

			_forward = glm::vec3(0, 0, 1); //z axis
			_up = glm::vec3(0, 1, 0); // y axis
		}

		glm::vec3 _pos;
		glm::mat4 _perspective;
		glm::mat4 _viewProjection;
		//rotation
		glm::vec3 _forward;
		glm::vec3 _up;
	};
}
