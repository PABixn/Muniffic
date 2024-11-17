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
		std::vector<UUID> m_Children;

		std::vector<UUID>& GetChildren() { return m_Children; }
	};
}
