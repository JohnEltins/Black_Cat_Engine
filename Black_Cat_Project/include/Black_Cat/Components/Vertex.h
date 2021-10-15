#pragma once
#include "glm/glm.hpp"

namespace BLK_Cat
{
	class Vertex
	{
	public:
		Vertex(const glm::vec3& pos, const glm::vec2& textCoord)
		{
			this->pos = pos;
			this->textCoord = textCoord;
		}

		inline glm::vec3* getPos() { return &pos; }
		inline glm::vec2* getTextCoord() { return &textCoord; }

	private:
		glm::vec3 pos;
		glm::vec2 textCoord;
	};
}