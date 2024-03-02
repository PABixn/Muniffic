#pragma once

#include "Engine/Core/UUID.h"

#include "../../../../Engine/vendor/entt/include/entt.hpp"

namespace eg
{
	class EntityInfo
	{
	public:
		EntityInfo() = default;
		EntityInfo(UUID parent)
			: m_Parent(parent) { }

		UUID m_Parent;
		int m_Layer = 0;
		std::vector<UUID> m_Children;

	};
}
