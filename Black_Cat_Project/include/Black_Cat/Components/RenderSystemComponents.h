#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>


#include "Black_Cat/Base/ECS.h"
#include "Display.h"

namespace BLK_Cat {
	struct Canvas : public Component
	{

		Canvas(uint32_t width, uint32_t height, std::string& tittle)
		{
			_width = width;
			_height = height;
			_tittle = tittle;
			_display = nullptr;
		}

		virtual ~Canvas() {}

		uint32_t _width;
		uint32_t _height;
		std::string _tittle;
		Display* _display;
	};

	/////////////////////////////////////////////////////////////////////
	//////////////////////		Render Primitives	 ////////////////////
	/////////////////////////////////////////////////////////////////////

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
		uint32_t _numVertices;
		Vertex* _vertices;
	};

	struct Quad : public Component
	{
		Quad()
		{
			std::cout << "Quad" << std::endl;
		}

		enum {
			POSITION_VB,
			TEXTCOORD_VB,

			NUM_BUFFERS
		};

		GLuint _VAO;
		uint32_t _numVertices;
		Vertex* _vertices;
	};

	struct Mesh : public Component
	{
		Mesh(const Vertex* vertices, const uint32_t numVertices, const uint32_t* indices, const uint32_t numIndices)
		{
			_vertexArrayObject = 0;
			for (uint32_t i = 0; i < NUM_BUFFERS; i++)
				_vertexArrayBuffers[i] = GLuint(0);


			//_vertices = new Vertex[numIndices];
			_numVertices = numVertices;

			//_indices = indices;
			_vertices = (Vertex*)malloc(numVertices * sizeof(vertices[0]));
			_indices = (uint32_t*)malloc(numIndices * sizeof(indices[0]));

			for (uint32_t i = 0; i < numVertices; i++)
			{
				_vertices[i] = vertices[i];
			}

			for (uint32_t i = 0; i < numIndices; i++)
			{
				_indices[i] = indices[i];
			}

			_numIndices = numIndices;
		}

		virtual ~Mesh() {}

		enum {
			POSITION_VB,
			TEXTCOORD_VB,
			INDEX_VB,

			NUM_BUFFERS
		};

		GLuint _vertexArrayObject;
		GLuint _vertexArrayBuffers[NUM_BUFFERS];
		uint32_t _numVertices;
		uint32_t _numIndices; //draw count
		uint32_t* _indices;
		Vertex* _vertices;

	};

	/////////////////////////////////////////////////////////////////////
	//////////////////////		Render Systems Components	 ////////////
	/////////////////////////////////////////////////////////////////////

	struct Shader : public Component
	{
		Shader(const std::string& filename)
		{
			_filename = filename;
		}
		virtual ~Shader() {}

		enum SHADERS
		{
			VertexShader, FragmentShader, NUM_SHADERS
		};

		enum UNIFORMS
		{
			TRANSFORM_U,

			NUM_UNIFORMS
		};


		std::string _filename;
		GLuint _program;
		GLuint _shaders[SHADERS::NUM_SHADERS];
		GLuint _uniforms[UNIFORMS::NUM_UNIFORMS];
	};

	struct Texture : public Component
	{
		Texture(const std::string& filename)
		{
			_filename = filename;
		}

		virtual ~Texture() {}


		GLuint _texture;
		std::string _filename;
	};

	struct Transform : public Component
	{
		Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(0.2, 0.2, 1.0))
			: _pos(pos), _rot(rot), _scale(scale), _model(glm::mat4(1.0f)), count(0.0f) {}

		glm::vec3 _pos;
		glm::vec3 _rot;
		glm::vec3 _scale;

		glm::mat4 _model;

		float_t count;
	};

	struct Camera : public Component
	{
		Camera(const glm::vec3& pos, float_t fov, float_t aspect, float_t zNear, float_t zFar)
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
