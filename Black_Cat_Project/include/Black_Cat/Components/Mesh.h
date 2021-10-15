#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Black_Cat/Base/Component.h"
#include "Vertex.h"

namespace BLK_Cat {


	struct Mesh : public Component
	{
		Mesh(Vertex* vertices, unsigned int numVertices)
		{
			_vertexArrayObject = 0;
			for (int i = 0; i < NUM_BUFFERS; i++)
				_vertexArrayBuffers[i] = GLuint(0);

			_vertices = vertices;
			_drawCount = numVertices;
		}

		virtual ~Mesh() {}

		enum {
			POSITION_VB,
			TEXTCOORD_VB,

			NUM_BUFFERS
		};

		GLuint _vertexArrayObject;
		GLuint _vertexArrayBuffers[NUM_BUFFERS];
		unsigned int _drawCount;
		Vertex* _vertices;
	};

}