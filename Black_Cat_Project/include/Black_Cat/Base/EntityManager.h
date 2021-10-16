#pragma once

#include "Types.h"
#include "CompList.h"
#include "Component.h"
#include "System.h"
#include "Black_Cat/Components/Mesh.h"
#include "Black_Cat/Components/Transform.h"
#include "Black_Cat/Components/2D_primitives.h"

#include <queue>
#include <map>
#include <memory>

namespace BLK_Cat
{
	class EntityManager
	{
	public:
		EntityManager() : _entityCount(0)
		{
			//reservar espaço na memoria para entidades
			for (EntityID entity = 0u; entity < MAX_ENTITIES; entity++)
			{
				_availableEntities.push(entity);
			}
		}

		virtual ~EntityManager() {}

		void update()
		{
			for (auto& system : _regiteredSystems)
			{
				for (auto& entity : _entitiesSignature)
				{
					//std::cout << "update" << std::endl;
					if (belongToSystem(entity.first, system.second->getSignature()))
						system.second->update(entity.first);
				}
			}
		}

		void init()
		{
			for (auto& system : _regiteredSystems)
			{
				for (auto& entity : _entitiesSignature)
				{
					if(belongToSystem(entity.first, system.second->getSignature()))
						system.second->init(entity.first);
				}
			}
		}

		void render(EntityID entity)
		{
			for (auto& system : _regiteredSystems)
			{
				system.second->render(entity);
			}
		}

		void draw()
		{
			for (auto& system : _regiteredSystems)
			{
				for (auto& entity : _entitiesSignature)
				{
					if (belongToSystem(entity.first, system.second->getSignature()))
					{
						system.second->draw(entity.first);
					}
				}
			}
		}

		const EntityID addNewEntity()
		{
			const EntityID entityID = _availableEntities.front();
			addEntitySignature(entityID);
			_availableEntities.pop();
			_entityCount++;
			std::cout << "entidade adicionada" << std::endl;
			return entityID;
		}

		void destroyEntity(const EntityID entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range!");
			_entitiesSignature.erase(entity);

			//limpar componentes associados
			for (auto& array : _componentsArrays)
			{
				array.second->erase(entity);
			}

			//limpar sistemas associados
			for (auto& system : _regiteredSystems)
			{
				system.second->removeEntity(entity);
			}

			_entityCount--;
			_availableEntities.push(entity); //entidade vazia, apenas o id
			std::cout << "entidade destruida" << std::endl;
		}

		template <typename T, typename... Args>
		void addComponent(const EntityID entity, Args&&... args)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range");
			assert(_entitiesSignature[entity]->size() < MAX_COMPONENTS && "Component count limit reached"); 

			T component(std::forward<Args>(args)...); //manda argumentos pro contrutor do componente
			component._entityID = entity;

			const ComponentTypeID compType = CompType<T>();

			getEntitySignature(entity)->insert(compType);
			getCompList<T>()->insert(component);
			updateEntityTargetSystems(entity);
		}

		template <typename T>
		void removeComponent(const EntityID entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range");
			const ComponentTypeID compType = CompType<T>();

			getEntitySignature(entity)->erase(compType);
			getCompList<T>()->erase(entity);
			updateEntityTargetSystems(entity);
		}

		template <typename T>
		T& getComponent(const EntityID entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range");
			const ComponentTypeID compType = CompType<T>();
			return getCompList<T>()->get(entity);
		}

		template <typename T>
		EntityID getFirstReference()
		{
			for (auto& entity : _entitiesSignature)
			{
				if (hasComponent<T>(entity.first))
				{
					return entity.first;
				}
			}


			return -1;
		}

		template <typename T>
		const int hasComponent(const EntityID entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range");
			//return (getCompList<T>()->get(entity));
			const ComponentTypeID compType = CompType<T>();
			return (getEntitySignature(entity)->count(compType));
		}

		template <typename T>
		void registerSystem(EntityManager* manager)
		{
			const SystemTypeID systemType = SystemType<T>();
			assert(_regiteredSystems.count(systemType) == 0 && "System already registered");
			auto system = std::make_shared<T>(manager);

			for (EntityID entity = 0; entity < _entityCount; entity++)
			{
				addEntityToSystem(entity, system.get());
			}

			std::cout << "sistema registrado" << std::endl;
			_regiteredSystems[systemType] = std::move(system);
		}

		template <typename T>
		void unregisterSystem()
		{
			const SystemTypeID systemType = SystemType<T>();
			assert(_regiteredSystems.count(systemType) != 0 && "System not found");
			_regiteredSystems.erase(systemType);
		}

	private:
		template <typename T>
		void addCompList()
		{
			const ComponentTypeID compType = CompType<T>();
			assert(_componentsArrays.find(compType) == _componentsArrays.end() && "Component List already registered");
			_componentsArrays[compType] = std::move(std::make_shared<CompList<T>>());
		}

		template <typename T>
		std::shared_ptr<CompList<T>> getCompList() 
		{
			const ComponentTypeID compType = CompType<T>();
			if (_componentsArrays.count(compType) == 0)
				addCompList<T>();


			return std::static_pointer_cast<CompList<T>>(_componentsArrays.at(compType));
		}

		void addEntitySignature(const EntityID entity)
		{
			assert(_entitiesSignature.find(entity) == _entitiesSignature.end() && "Signature already exist");
			_entitiesSignature[entity] = std::move(std::make_shared<EntitySignature>());
		}

		std::shared_ptr<EntitySignature> getEntitySignature(const EntityID entity)
		{
			assert(_entitiesSignature.find(entity) != _entitiesSignature.end() && "Signature not found");
			return _entitiesSignature.at(entity);
		}

		void updateEntityTargetSystems(const EntityID entity) 
		{
			for (auto& system : _regiteredSystems)
			{
				addEntityToSystem(entity, system.second.get());
			}
		}

		void addEntityToSystem(const EntityID entity, System* system) 
		{
			if (belongToSystem(entity, system->_signature))
			{
				system->_entities.insert(entity);
			}
			else
			{
				system->_entities.erase(entity);
			}
		}

		bool belongToSystem(const EntityID entity, EntitySignature systemSignature)
		{
			for (const auto compType : systemSignature)
			{
				if (getEntitySignature(entity)->count(compType) == 0)
				{
					return false;
				}
			}

			return true;
		}

		EntityID _entityCount;
		std::queue<EntityID> _availableEntities; //entidades criadas
		std::map<EntityID, std::shared_ptr<EntitySignature>> _entitiesSignature; //associação entidade a um componente
		std::map<SystemTypeID, std::shared_ptr<System>> _regiteredSystems; //associa os ids dos sistemas a classe de um systema
		std::map<ComponentTypeID, std::shared_ptr<ICompList>> _componentsArrays; //associa um tipo de component id a seu vetor de ids de um mesmo tipo
	};
}

