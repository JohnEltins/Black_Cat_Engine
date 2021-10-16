#pragma once

#include "EntityManager.h"

namespace BLK_Cat {

	class Entity
	{
	public:
		Entity(const EntityID id, EntityManager* manager) : _ID(id), _MGR(manager) {}
		~Entity() = default;

		const EntityID getID() const
		{
			return _ID;
		}

		template<typename T, typename... Args>
		void addComponent(Args&&... args)
		{
			_MGR->addComponent<T>(_ID, std::forward<Args>(args)...);
			std::cout << "Componente adicionado" << std::endl;
		}

		template<typename T>
		inline T& getComponent()
		{
			return _MGR->getComponent<T>(_ID);
		}

		template<typename T>
		inline void removeComponent()
		{
			_MGR->removeComponent<T>(_ID);
			std::cout << "Componente removido" << std::endl;
		}

		template<typename T>
		inline bool hasComponent()
		{
			return _MGR->hasComponent<T>(_ID);
		}

		void destroy()
		{
			_MGR->destroyEntity(_ID);
		}

	protected:
		EntityID _ID;
		EntityManager* _MGR;
	};
}