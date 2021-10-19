#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Display.h"

namespace BLK_Cat {
	/////////////////////////////////////////////////////////////////////
	//////////////////////		Render Systems Components	 ////////////
	/////////////////////////////////////////////////////////////////////

	struct Vertex
	{
		Vertex(const glm::vec3& pos, const glm::vec2& textCoord)
		{
			_pos = pos;
			_textCoord = textCoord;
		}

		glm::vec3 _pos;
		glm::vec2 _textCoord;
	};

	struct Shader
	{
		Shader(const std::string& filename)
		{
			_filename = filename;
		}

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

	struct Texture
	{
		Texture(const std::string& filename)
		{
			_filename = filename;
		}


		GLuint _texture;
		std::string _filename;
	};

	struct Transform
	{
		Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(0.2, 0.2, 1.0))
			: _pos(pos), _rot(rot), _scale(scale), _model(glm::mat4(1.0f)), count(0.0f), _spawn(pos) {}

		glm::vec3 _pos;
		glm::vec3 _rot;
		glm::vec3 _scale;
		glm::vec3 _spawn;
		glm::mat4 _model;

		float_t count;
	};

	struct Camera
	{
		Camera(const glm::vec3& pos, float_t fov, float_t aspect, float_t zNear, float_t zFar) //perspective
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

	struct CameraOrtho
	{
		CameraOrtho(float_t left, float_t right, float_t bottom, float_t top, float_t zNear, float_t zFar, glm::vec3 increment) //ortho
		{
			_increment = increment;

			_cameraSet = glm::mat2x3();

			_cameraSet[0][0] = left;
			_cameraSet[0][1] = bottom;
			_cameraSet[0][2] = zNear;

			_cameraSet[1][0] = right;
			_cameraSet[1][1] = top;
			_cameraSet[1][2] = zFar;

			_viewOrtho = glm::ortho(	left + _increment.x, right + _increment.x, 
										bottom + _increment.y, top + _increment.y,
										zNear + _increment.z, zFar + _increment.z);
		}

		glm::vec3 _increment;
		glm::mat4 _viewOrtho;
		glm::mat2x3 _cameraSet;
	};

	/////////////////////////////////////////////////////////////////////
	//////////////////////		Render Primitives	 ////////////////////
	/////////////////////////////////////////////////////////////////////

	

	struct Triangle
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

	struct Quad
	{
		Quad()
		{
			//std::cout << "Quad" << std::endl;
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

	struct Duplicate
	{
		Duplicate(){}
	};

	struct Mesh
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

	
}
