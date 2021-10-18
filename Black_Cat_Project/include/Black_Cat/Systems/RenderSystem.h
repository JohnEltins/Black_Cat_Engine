#pragma once

#include "Black_Cat/Base/entt.hpp"
#include "Black_Cat/Components/RenderSystemComponents.h"
#include "stb_image.h"

#include <tuple>
#include <iostream>
#include <fstream>
//#include <memory>

namespace BLK_Cat
{
	void RendererInit(entt::registry& registry);
	void RendererDraw(entt::registry& registry);

	/////////////////////////////////////////////////////////////////////
	//////////////////////		Draw Primitives		/////////////////////
	/////////////////////////////////////////////////////////////////////

	void CreateTriangle(Triangle& triangle);
	void CreateQuad(Quad& quad);
	void CreateMesh(Mesh& mesh);

	void DrawTriangle(Triangle& triangle, Shader& shader, Texture& texture, Transform& transform, Camera& camera);
	void DrawQuad(Quad& quad, Shader& shader, Texture& texture, Transform& transform, Camera& camera);
	void DrawMesh(Mesh& triangle, Shader& shader, Texture& texture, Transform& transform, Camera& camera);

	/////////////////////////////////////////////////////////////////////
	//////////////////////		Handlers	 ////////////////////////////
	/////////////////////////////////////////////////////////////////////

	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string errorMessage);
	std::string LoadShader(const std::string& fileName);
	GLuint CreateShader(const std::string txt, GLenum shaderType);


	void InitShaderHandler(Shader& shader);
	void InitTextureHandler(Texture& texture);
}