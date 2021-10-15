#pragma once
#include "Black_Cat/Base/System.h"
#include "Black_Cat/Components/Canvas.h"
#include "Black_Cat/Components/Mesh.h"
#include "Black_Cat/Base/EntityManager.h"
#include "Black_Cat/Components/Shader.h"
#include "Black_Cat/Components/Texture.h"

#include <fstream>
#include <iostream>
#include "stb_image.h"

namespace BLK_Cat{

	struct Renderer : public System
	{
		Renderer(EntityManager* manager)
			: System(manager)
		{
			addComponentSignature<Canvas>();
		}

		void init(const EntityID entity) override
		{
			_manager->getComponent<Canvas>(entity)._display = new Display(_manager->getComponent<Canvas>(entity)._width,
				_manager->getComponent<Canvas>(entity)._height,
				_manager->getComponent<Canvas>(entity)._tittle);


		}

		void render(const EntityID entity) override
		{
			while (!_manager->getComponent<Canvas>(entity)._display->isClosed())
			{
				_manager->getComponent<Canvas>(entity)._display->clear(0.0f, 0.15f, 0.3f, 1.0f);
				_manager->update();
				_manager->draw();
				_manager->getComponent<Canvas>(entity)._display->swapBuffers();

			}
		}
	};

	struct Draw : public System
	{
		Draw(EntityManager* manager)
			: System(manager)
		{
			addComponentSignature<Mesh>();
		}

		void init(const EntityID entity) override
		{
			glGenVertexArrays(1, &_manager->getComponent<Mesh>(entity)._vertexArrayObject);
			glBindVertexArray(_manager->getComponent<Mesh>(entity)._vertexArrayObject);

			std::vector<glm::vec3> positions;
			std::vector<glm::vec2> texture;

			positions.reserve(_manager->getComponent<Mesh>(entity)._drawCount);
			texture.reserve(_manager->getComponent<Mesh>(entity)._drawCount);

			for (unsigned int i = 0; i < _manager->getComponent<Mesh>(entity)._drawCount; i++)
			{
				positions.push_back(*_manager->getComponent<Mesh>(entity)._vertices[i].getPos());
				texture.push_back(*_manager->getComponent<Mesh>(entity)._vertices[i].getTextCoord());
			}

			glGenBuffers(_manager->getComponent<Mesh>(entity).NUM_BUFFERS, _manager->getComponent<Mesh>(entity)._vertexArrayBuffers);

			//buffer de vertices
			glBindBuffer(GL_ARRAY_BUFFER, _manager->getComponent<Mesh>(entity)._vertexArrayBuffers[_manager->getComponent<Mesh>(entity).POSITION_VB]);
			glBufferData(GL_ARRAY_BUFFER, _manager->getComponent<Mesh>(entity)._drawCount * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			//buffer de textCoord
			glBindBuffer(GL_ARRAY_BUFFER, _manager->getComponent<Mesh>(entity)._vertexArrayBuffers[_manager->getComponent<Mesh>(entity).TEXTCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, _manager->getComponent<Mesh>(entity)._drawCount * sizeof(texture[0]), &texture[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glBindVertexArray(0);
		}


		void draw(const EntityID entity, glm::mat4 model) override
		{
			glUseProgram(0);
			glUseProgram(_manager->getComponent<Shader>(entity)._program);

			glUniformMatrix4fv(_manager->getComponent<Shader>(entity)._uniforms[_manager->getComponent<Shader>(entity).UNIFORMS::TRANSFORM_U], 1, GL_FALSE, &model[0][0]);

			glActiveTexture(GL_TEXTURE0); //setar textura de uma das 32 unidades
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindTexture(GL_TEXTURE_2D, _manager->getComponent<Texture>(entity)._texture);

			glBindVertexArray(_manager->getComponent<Mesh>(entity)._vertexArrayObject);
			glDrawArrays(GL_TRIANGLES, 0, _manager->getComponent<Mesh>(entity)._drawCount);
			glBindVertexArray(0);
		}
	};

	struct ShaderHandler : public System
	{
		ShaderHandler(EntityManager* manager)
			:System(manager)
		{
			addComponentSignature<Shader>();
		}

		void init(const ComponentTypeID entity) override
		{
			_manager->getComponent<Shader>(entity)._program = glCreateProgram();
			_manager->getComponent<Shader>(entity)._shaders[_manager->getComponent<Shader>(entity).SHADERS::VertexShader] = CreateShader(LoadShader(_manager->getComponent<Shader>(entity)._filename + ".vs"), GL_VERTEX_SHADER);
			_manager->getComponent<Shader>(entity)._shaders[_manager->getComponent<Shader>(entity).SHADERS::FragmentShader] = CreateShader(LoadShader(_manager->getComponent<Shader>(entity)._filename + ".fs"), GL_FRAGMENT_SHADER);

			for (unsigned int i = 0; i < _manager->getComponent<Shader>(entity).NUM_SHADERS; i++)
				glAttachShader(_manager->getComponent<Shader>(entity)._program, _manager->getComponent<Shader>(entity)._shaders[i]);

			glBindAttribLocation(_manager->getComponent<Shader>(entity)._program, 0, "position");
			glBindAttribLocation(_manager->getComponent<Shader>(entity)._program, 1, "texCoord");

			glLinkProgram(_manager->getComponent<Shader>(entity)._program);
			CheckShaderError(_manager->getComponent<Shader>(entity)._program, GL_LINK_STATUS, true, "Error shader program failed to link: ");

			glValidateProgram(_manager->getComponent<Shader>(entity)._program);
			CheckShaderError(_manager->getComponent<Shader>(entity)._program, GL_VALIDATE_STATUS, true, "Error shader program is invalid: ");

			_manager->getComponent<Shader>(entity)._uniforms[_manager->getComponent<Shader>(entity).TRANSFORM_U] = glGetUniformLocation(_manager->getComponent<Shader>(entity)._program, "transform");
		}

		void update(const ComponentTypeID entity) override
		{
		}

		void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string errorMessage)
		{
			GLint success = 0;
			GLchar error[1024] = { 0 };

			if (isProgram)
				glGetProgramiv(shader, flag, &success);
			else
				glGetShaderiv(shader, flag, &success);

			if (success == GL_FALSE)
			{
				if (isProgram)
					glGetProgramInfoLog(shader, sizeof(error), NULL, error);
				else
					glGetShaderInfoLog(shader, sizeof(error), NULL, error);

				std::cerr << errorMessage << ": '" << error << "'" << std::endl;
			}
		}

		std::string LoadShader(const std::string& fileName)
		{
			std::ifstream file;
			file.open((fileName).c_str());

			std::string output;
			std::string line;

			if (file.is_open())
			{
				while (file.good())
				{
					getline(file, line);
					output.append(line + "\n");
				}
			}
			else
			{
				std::cerr << "Unable to Load shader: " << fileName << std::endl;
			}

			std::cout << output << std::endl;
			return output;
		}

		GLuint CreateShader(const std::string txt, GLenum shaderType)
		{
			GLuint shader = glCreateShader(shaderType);

			if (shader == 0)
				std::cerr << "Error: shader creation failed!" << std::endl;

			const GLchar* shaderSourceStrings[1];
			GLint shaderSouceStringLenghts[1];

			shaderSourceStrings[0] = txt.c_str();
			shaderSouceStringLenghts[0] = txt.length();

			glShaderSource(shader, 1, shaderSourceStrings, shaderSouceStringLenghts);
			glCompileShader(shader);

			CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error shader failed to compile: ");

			return shader;
		}
	};

	struct TextureHandler : public System
	{
		TextureHandler(EntityManager* manager)
			: System(manager)
		{
			addComponentSignature<Texture>();
		}

		void init(const ComponentTypeID entity) override
		{
			int width, height, numComponents;
			unsigned char* imageData = stbi_load(_manager->getComponent<Texture>(entity)._filename.c_str(), &width, &height, &numComponents, 4); //carregar png 

			if (imageData == NULL)
				std::cerr << "Texture loading failed: " << _manager->getComponent<Texture>(entity)._filename << std::endl;

			glGenTextures(1, &_manager->getComponent<Texture>(entity)._texture);
			glBindTexture(GL_TEXTURE_2D, _manager->getComponent<Texture>(entity)._texture);

			//repetir caso a textura chegue aou final
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			//interpolação caso a imagem seja aumentada ou diminuida
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//envia imagem para o buffer de textura na gpu
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

			stbi_image_free(imageData);
		}

		void update(const ComponentTypeID entity) override
		{

		}
	};

}