#pragma once
#include <iostream>
#include <set>
#include <queue>
#include <map>
#include <memory>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <fstream>

namespace BLK_Cat
{
	/////////////////////////////////////////////////////////////////////
	//////////////////////		Types	 ////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	struct System;
	struct Component;
	class EntityManager;

	const size_t MAX_ENTITIES = 5000;
	const size_t MAX_COMPONENTS = 32;

	using EntityID = size_t;
	using SystemTypeID = size_t;
	using ComponentTypeID = size_t;
	using EntitySignature = std::set<ComponentTypeID>; //Ids de todos os componentes associados a uma entidade

	//cria id unico pra um componente
	inline ComponentTypeID GetRuntimeComponentID()
	{
		static ComponentTypeID typeID = 0u; //u -> unsigned int
		return typeID++;
	}

	//cria id unico pra um sistema
	inline SystemTypeID GetRuntimeSystemID()
	{
		static SystemTypeID typeID = 0u; //u -> unsigned int
		return typeID++;
	}

	//retorna o id do componente
	template <typename T>
	inline static const ComponentTypeID CompType() noexcept
	{
		static_assert((std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value), "Invalid template type");
		static const ComponentTypeID typeID = GetRuntimeComponentID();
		return typeID;
	}

	//retorna o id do systema
	template <typename T>
	inline static const SystemTypeID SystemType() noexcept
	{
		static_assert((std::is_base_of<System, T>::value && !std::is_same<System, T>::value), "Invalid template type");
		static const SystemTypeID typeID = GetRuntimeSystemID();
		return typeID;
	}

	/////////////////////////////////////////////////////////////////////
	//////////////////////		System Base		/////////////////////////
	/////////////////////////////////////////////////////////////////////

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
		virtual void destroy() {}

	protected:
		friend class EntityManager;
		EntityManager* _manager;
		EntitySignature _signature;
		std::set<EntityID> _entities;
	};

	/////////////////////////////////////////////////////////////////////
	//////////////////////		Component List		/////////////////////
	/////////////////////////////////////////////////////////////////////

	class ICompList
	{
	public:
		ICompList() = default;
		virtual ~ICompList() = default;
		virtual void erase(const EntityID entity) {}

	};

	//lista de componentes de um tipo
	template<typename T>
	class CompList : public ICompList
	{
	public:
		CompList() = default;
		~CompList() = default;

		void insert(const T& component)
		{
			//auto comp = std::find_if(data.begin(), data.end(), [&](const T& c) { return c.getID() == component.getID(); });
			auto comp = std::find_if(data.begin(), data.end(), [&](const T& c) { return c.getID() == component.getID(); });

			if (comp == data.end())
			{
				data.push_back(component);
			}
		}

		T& get(const EntityID entity)
		{
			auto comp = std::find_if(data.begin(), data.end(), [&](const T& c) { return c.getID() == entity; });
			//assert( comp != data.end() && "Trying to get non exiting data");

			return *comp;
		}

		virtual void erase(const EntityID entity) override
		{
			auto comp = std::find_if(data.begin(), data.end(), [&](const T& c) { return c.getID() == entity; });
			assert(comp != data.end() && "Trying to get non exiting data");
			if (comp != data.end())
			{
				data.erase(comp);
			}
		}

		std::vector<T> data;
	};

	/////////////////////////////////////////////////////////////////////
	//////////////////////		Manager		/////////////////////////////
	/////////////////////////////////////////////////////////////////////

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
					if (belongToSystem(entity.first, system.second->getSignature()))
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

	/////////////////////////////////////////////////////////////////////
	//////////////////////		Entity Base		/////////////////////////
	/////////////////////////////////////////////////////////////////////

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

	/////////////////////////////////////////////////////////////////////
	//////////////////////		Component Base		/////////////////////
	/////////////////////////////////////////////////////////////////////

	struct Component
	{
		Component() : _entityID() {}
		virtual ~Component() {}
		inline const EntityID getID() const { return _entityID; }

	protected:
		friend class EntityManager;
		EntityID _entityID;
	};

	struct Drawable : public Component
	{
		Drawable()
		{}

		virtual ~Drawable() {}
	};


	/////////////////////////////////////////////////////////////////////
	//////////////////////		Base Scene		/////////////////////////
	/////////////////////////////////////////////////////////////////////

	class Scene : public Entity
	{
	public:
		Scene(const EntityID id, EntityManager* manager)
			: Entity(id, manager)
		{}

		~Scene() = default;
	};
}
