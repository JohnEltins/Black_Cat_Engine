#pragma once

#include "Types.h"
#include <vector>
#include <assert.h>
#include <algorithm>
#include <iostream>

namespace BLK_Cat {
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
}