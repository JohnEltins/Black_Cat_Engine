#pragma once

#include "Types.h"
#include "glm/glm.hpp"

namespace BLK_Cat {
	struct System
	{
		System(EntityManager* manager) { _manager = manager; }
		virtual ~System() = default;

		void removeEntity(const EntityID entity)
		{
			_entities.erase(entity);
		}

		void addEntity(const EntityID entity)
		{
			_entities.insert(entity);
		}

		const EntitySignature getSignature() const
		{
			return _signature;
		}

		template<typename T>
		void addComponentSignature()
		{
			_signature.insert(CompType<T>());
		}

		virtual void init(const EntityID entity) {}
		virtual void update(const ComponentTypeID entity) {}

		virtual void render(const EntityID entity) {}
		virtual void draw(const EntityID entity) {}
		virtual void destroy(){}

	protected:
		friend class EntityManager;
		EntityManager* _manager;
		EntitySignature _signature;
		std::set<EntityID> _entities;
	};
}

