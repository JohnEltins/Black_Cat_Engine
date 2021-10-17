#pragma once

#include "Black_Cat/Base/ECS.h"
#include "Black_Cat/Components/RenderSystemComponents.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>




namespace BLK_Cat {

	struct TransformHandler : public System
	{
		TransformHandler(EntityManager* manager)
			: System(manager)
		{
			addComponentSignature<Transform>();
		}

		void init(const EntityID entity) override
		{
			//std::cout << "transform init" << std::endl;
			_manager->getComponent<Transform>(entity)._model = getModel(entity);
			_manager->getComponent<Transform>(entity).count = 0.0f;
		}

		void update(const EntityID entity) override
		{
			if (_manager->hasComponent<Quad>(entity))
			{
				setPos(entity, glm::vec3(sinf(_manager->getComponent<Transform>(entity).count) / 2, cosf(_manager->getComponent<Transform>(entity).count) / 2, sinf(_manager->getComponent<Transform>(entity).count)*2));
				getRot(entity).x = -1 * _manager->getComponent<Transform>(entity).count;
				getRot(entity).y = -1 * _manager->getComponent<Transform>(entity).count;
				getRot(entity).z = -1 * _manager->getComponent<Transform>(entity).count;
			}
			else if (_manager->hasComponent<Triangle>(entity))
			{
				setPos(entity, glm::vec3(cosf(_manager->getComponent<Transform>(entity).count) / 2, sinf(_manager->getComponent<Transform>(entity).count) / 2, sinf(_manager->getComponent<Transform>(entity).count) * 2));
				getRot(entity).x = _manager->getComponent<Transform>(entity).count;
				getRot(entity).y = _manager->getComponent<Transform>(entity).count;
				getRot(entity).z = _manager->getComponent<Transform>(entity).count;
			}
			else if (_manager->hasComponent<Mesh>(entity))
			{
				getPos(entity).z = sinf(_manager->getComponent<Transform>(entity).count) * 2;
				getRot(entity).x = _manager->getComponent<Transform>(entity).count;
				getRot(entity).y = _manager->getComponent<Transform>(entity).count;
				getRot(entity).z = _manager->getComponent<Transform>(entity).count;
			}


			_manager->getComponent<Transform>(entity).count += 0.035f;
			_manager->getComponent<Transform>(entity)._model = getModel(entity);
		}

		glm::mat4 getModel(const ComponentTypeID entity)
		{
			glm::mat4 posMatrix = glm::translate(_manager->getComponent<Transform>(entity)._pos);

			glm::mat4 rotXMatrix = glm::rotate(_manager->getComponent<Transform>(entity)._rot.x, glm::vec3(1, 0, 0));
			glm::mat4 rotYMatrix = glm::rotate(_manager->getComponent<Transform>(entity)._rot.y, glm::vec3(0, 1, 0));
			glm::mat4 rotZMatrix = glm::rotate(_manager->getComponent<Transform>(entity)._rot.z, glm::vec3(0, 0, 1));

			glm::mat4 scaleMatrix = glm::scale(_manager->getComponent<Transform>(entity)._scale);

			//ordem da multiplicação de matriz importa, X, Y Z, !ordem oposta!

			glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;


			//escala, gira e movimenta, nessa ordem
			return posMatrix * rotMatrix * scaleMatrix;
		}

		glm::vec3& getPos(const ComponentTypeID entity) { return _manager->getComponent<Transform>(entity)._pos; }
		glm::vec3& getRot(const ComponentTypeID entity) { return _manager->getComponent<Transform>(entity)._rot; }
		glm::vec3& getScale(const ComponentTypeID entity) { return _manager->getComponent<Transform>(entity)._scale; }

		void setPos(const ComponentTypeID entity, const glm::vec3& pos) { _manager->getComponent<Transform>(entity)._pos = pos; }
		void setRot(const ComponentTypeID entity, const glm::vec3& rot) { _manager->getComponent<Transform>(entity)._rot = rot; }
		void setScale(const ComponentTypeID entity, const glm::vec3& scale) { _manager->getComponent<Transform>(entity)._scale = scale; }
	};

	struct CameraHandler : public System
	{
		CameraHandler(EntityManager* manager)
			: System(manager)
		{
			addComponentSignature<Camera>();
		}

		void init(const EntityID entity) override
		{
			_manager->getComponent<Camera>(entity)._viewProjection = getViewProjection(entity);
		}

		glm::mat4 getViewProjection(const ComponentTypeID entity)
		{
			//rotação da camera com perspectiva e movimento da camera
			return _manager->getComponent<Camera>(entity)._perspective * glm::lookAt(_manager->getComponent<Camera>(entity)._pos,
				_manager->getComponent<Camera>(entity)._pos + _manager->getComponent<Camera>(entity)._forward,
				_manager->getComponent<Camera>(entity)._up);
		}


		void update(const EntityID entity) override {}
	};
}