#pragma once

#pragma once

#include "Black_Cat/Base/Component.h"
#include "Black_Cat/Components/Vertex.h"
#include "GL/glew.h"

namespace BLK_Cat
{

	struct Triangle : public Component
	{
		Triangle()
		{
			_vertices = nullptr;
		}

		virtual ~Triangle() {}

		enum {
			POSITION_VB,
			TEXTCOORD_VB,

			NUM_BUFFERS
		};

		GLuint _VAO;
		unsigned int _drawCount;
		Vertex* _vertices;
	};

	struct Quad : public Component
	{
		Quad()
		{

		}

		enum {
			POSITION_VB,
			TEXTCOORD_VB,

			NUM_BUFFERS
		};

		GLuint _VAO;
		unsigned int _drawCount;
		Vertex* _vertices;
	};

}

