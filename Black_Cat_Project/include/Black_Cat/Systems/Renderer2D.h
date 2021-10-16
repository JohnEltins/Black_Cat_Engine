#pragma once

#include "Black_Cat/Base/System.h"
#include "Black_Cat/Base/EntityManager.h"

#include "Black_Cat/Components/Canvas.h"
#include "Black_Cat/Components/2D_primitives.h"
#include "Black_Cat/Components/Shader.h"
#include "Black_Cat/Components/Texture.h"

namespace BLK_Cat
{
	struct Renderer2D : System
	{
		Renderer2D(EntityManager* manager)
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
			//std::cout << "Render2D" << std::endl;
			_manager->draw();
			_manager->update();
		}
	
		void destroy() {}
	};

	struct DrawTriangle : public System
	{
		DrawTriangle(EntityManager* manager)
			: System(manager)
		{
			addComponentSignature<Triangle>();
		}

		void init(const EntityID entity) override
		{

			Vertex vertices[] = {
				Vertex(glm::vec3(-0.5f, -0.5f, 0.0), glm::vec2(0.0, 0.0)),
				Vertex(glm::vec3(0.0,  0.5f, 0.0),  glm::vec2(0.5, 1.0)),
				Vertex(glm::vec3(0.5f, -0.5f, 0.0),  glm::vec2(1.0, 0.0))
			};

			const unsigned int drawCount = sizeof(vertices) / sizeof(vertices[0]);
			GLuint VBO[drawCount];

			_manager->getComponent<Triangle>(entity)._drawCount = drawCount;

			_manager->getComponent<Triangle>(entity)._vertices = vertices;

			glGenVertexArrays(1, &_manager->getComponent<Triangle>(entity)._VAO);
			glBindVertexArray(_manager->getComponent<Triangle>(entity)._VAO);

			std::vector<glm::vec3> positions;
			std::vector<glm::vec2> texture;

			positions.reserve(drawCount);
			texture.reserve(drawCount);

			for (unsigned int i = 0; i < drawCount; i++)
			{
				positions.push_back(*_manager->getComponent<Triangle>(entity)._vertices[i].getPos());
				texture.push_back(*_manager->getComponent<Triangle>(entity)._vertices[i].getTextCoord());
			}

			glGenBuffers(_manager->getComponent<Triangle>(entity).NUM_BUFFERS, VBO);

			//buffer de vertices
			glBindBuffer(GL_ARRAY_BUFFER, VBO[_manager->getComponent<Triangle>(entity).POSITION_VB]);
			glBufferData(GL_ARRAY_BUFFER, drawCount * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			//buffer de textCoord
			glBindBuffer(GL_ARRAY_BUFFER, VBO[_manager->getComponent<Triangle>(entity).TEXTCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, drawCount * sizeof(texture[0]), &texture[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			glBindVertexArray(0);
		}


		void draw(const EntityID entity) override 
		{
			if (_manager->hasComponent<Drawable>(entity))
			{
				glUseProgram(0);
				glUseProgram(_manager->getComponent<Shader>(entity)._program);

				glm::mat4 model = _manager->getComponent<Camera>(_manager->getFirstReference<Camera>())._viewProjection
					* _manager->getComponent<Transform>(entity)._model;

				glUniformMatrix4fv(_manager->getComponent<Shader>(entity)._uniforms[_manager->getComponent<Shader>(entity).UNIFORMS::TRANSFORM_U], 1, GL_FALSE, &model[0][0]);

				glActiveTexture(GL_TEXTURE0); //setar textura de uma das 32 unidades
				glBindTexture(GL_TEXTURE_2D, 0);
				glBindTexture(GL_TEXTURE_2D, _manager->getComponent<Texture>(entity)._texture);

				glBindVertexArray(_manager->getComponent<Triangle>(entity)._VAO);
				glDrawArrays(GL_TRIANGLES, 0, _manager->getComponent<Triangle>(entity)._drawCount);
				glBindVertexArray(0);
			}
		}

		void destroy() override  {}
	};

	struct DrawQuad : System
	{
		DrawQuad(EntityManager* manager)
			: System(manager)
		{
			addComponentSignature<Quad>();
			std::cout << "quad signature" << std::endl;
		}

		void init(const EntityID entity) override
		{
			std::cout << "quad init" << std::endl;
			Vertex vertices[] = {
				Vertex(glm::vec3(0.5f,  0.5f, 0.0), glm::vec2(-1.0, -1.0)),
				Vertex(glm::vec3(0.5f, -0.5f, 0.0), glm::vec2(1.0, -1.0)),
				Vertex(glm::vec3(-0.5f,-0.5f, 0.0), glm::vec2(1.0, 1.0)),
				Vertex(glm::vec3(-0.5f, 0.5f, 0.0), glm::vec2(-1.0, 1.0))
			};

			unsigned int indices[] = {
				0, 1, 2,
				2, 3, 0
			};

			const unsigned int drawCount = sizeof(vertices) / sizeof(vertices[0]);
			GLuint VBO[drawCount];

			_manager->getComponent<Quad>(entity)._drawCount = drawCount;

			_manager->getComponent<Quad>(entity)._vertices = vertices;

			glGenVertexArrays(1, &_manager->getComponent<Quad>(entity)._VAO);
			glBindVertexArray(_manager->getComponent<Quad>(entity)._VAO);

			std::vector<glm::vec3> positions;
			std::vector<glm::vec2> texture;

			positions.reserve(drawCount);
			texture.reserve(drawCount);

			for (unsigned int i = 0; i < drawCount; i++)
			{
				positions.push_back(*_manager->getComponent<Quad>(entity)._vertices[i].getPos());
				texture.push_back(*_manager->getComponent<Quad>(entity)._vertices[i].getTextCoord());
			}

			glGenBuffers(_manager->getComponent<Quad>(entity).NUM_BUFFERS, VBO);

			//buffer de vertices
			glBindBuffer(GL_ARRAY_BUFFER, VBO[_manager->getComponent<Quad>(entity).POSITION_VB]);
			glBufferData(GL_ARRAY_BUFFER, drawCount * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			//buffer de textCoord
			glBindBuffer(GL_ARRAY_BUFFER, VBO[_manager->getComponent<Quad>(entity).TEXTCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, drawCount * sizeof(texture[0]), &texture[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

			unsigned int EBO;
			glGenBuffers(1, &EBO);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glBindVertexArray(0);
		}


		void draw(const EntityID entity) override 
		{
			if (_manager->hasComponent<Drawable>(entity))
			{
				glUseProgram(0);
				glUseProgram(_manager->getComponent<Shader>(entity)._program);

				glm::mat4 model = _manager->getComponent<Camera>(_manager->getFirstReference<Camera>())._viewProjection
					* _manager->getComponent<Transform>(entity)._model;

				glUniformMatrix4fv(_manager->getComponent<Shader>(entity)._uniforms[_manager->getComponent<Shader>(entity).UNIFORMS::TRANSFORM_U], 1, GL_FALSE, &model[0][0]);

				glActiveTexture(GL_TEXTURE0); //setar textura de uma das 32 unidades
				glBindTexture(GL_TEXTURE_2D, 0);
				glBindTexture(GL_TEXTURE_2D, _manager->getComponent<Texture>(entity)._texture);

				glBindVertexArray(_manager->getComponent<Quad>(entity)._VAO);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glBindVertexArray(0);
			}
		}

		void destroy() override {}
	};
}
