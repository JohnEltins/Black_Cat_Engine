#pragma once
#include <iostream>
#include <set>

namespace BLK_Cat
{
	struct System;
	struct Component;

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
}