#pragma once

#include "Engine.h"
#include "../../Engine/vendor/Box2D/include/box2d/box2d.h"

namespace eg
{
	class ContactListener : public b2ContactListener
	{
	public:
		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
	};

	struct Collision2D
	{
	public:
		Collision2D(UUID otherEntity, glm::vec2 contactPoints)
			: otherEntity(otherEntity), contactPoints(contactPoints) {}


		UUID otherEntity;
		glm::vec2 contactPoints;
	};

	struct InternalCollision2DEvent
	{
		UUID entityA;
		UUID entityB;
		glm::vec2 contactPoints;
	};
}