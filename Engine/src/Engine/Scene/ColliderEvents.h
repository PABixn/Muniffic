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
	};

	struct Collision2D
	{
	public:
		Collision2D(UUID otherEntity, glm::vec2 contactPoints, float friction, float restitution, float tangentSpeed)
			: otherEntity(otherEntity), contactPoints(contactPoints), friction(friction), restitution(restitution), tangentSpeed(tangentSpeed) {}


		UUID otherEntity;
		glm::vec2 contactPoints;
		float friction;
		float restitution;
		float tangentSpeed;
	};

	struct InternalCollision2DEvent
	{
		UUID entityA;
		UUID entityB;
		glm::vec2 contactPoints;
		float friction;
		float restitution;
		float tangentSpeed;
	};
}