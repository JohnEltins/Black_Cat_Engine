#pragma once
#include "Black_Cat/Base/Component.h"
#include "Black_Cat/Base/Entity.h"

namespace BLK_Cat
{
	class Scene : public Entity
	{
	public:
		Scene(const EntityID id, EntityManager* manager)
			: Entity(id, manager)
		{}

		~Scene() = default;
	};
}