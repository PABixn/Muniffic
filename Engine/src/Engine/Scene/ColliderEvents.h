#pragma once

#include "Engine.h"

namespace eg
{
	class ColliderEvents
	{
	public:
		struct Collision2D
		{
			UUID m_Entity;
			glm::vec2 m_ContactPoints;
		};
	};
}