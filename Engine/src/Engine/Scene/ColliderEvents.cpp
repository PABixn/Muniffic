#include "egpch.h"
#include "ColliderEvents.h"
#include <box2d/b2_contact.h>
#include <Engine/Scripting/ScriptEngine.h>

namespace eg
{
	void ContactListener::BeginContact(b2Contact* contact)
	{
		InternalCollision2DEvent collision;
		
		UUID entityA = contact->GetFixtureA()->GetUserData().pointer;
		UUID entityB = contact->GetFixtureB()->GetUserData().pointer;

		collision.entityA = entityA;
		collision.entityB = entityB;

		collision.friction = contact->GetFriction();
		collision.restitution = contact->GetRestitution();
		collision.tangentSpeed = contact->GetTangentSpeed();

		glm::vec2 contactPoints = { contact->GetManifold()->localPoint.x, contact->GetManifold()->localPoint.y };

		collision.contactPoints = contactPoints;

		EG_CORE_TRACE("Collision between {0} and {1} at {2}, {3}", entityA, entityB, contactPoints.x, contactPoints.y);

		for(auto& scriptInstance: ScriptEngine::GetEntityScriptInstances(entityA))
			scriptInstance->InvokeOn2DCollisionEnter(collision);

		for(auto& scriptInstance : ScriptEngine::GetEntityScriptInstances(entityB))
			scriptInstance->InvokeOn2DCollisionEnter(collision);
	}

	void ContactListener::EndContact(b2Contact* contact)
	{
		InternalCollision2DEvent collision;

		UUID entityA = contact->GetFixtureA()->GetUserData().pointer;
		UUID entityB = contact->GetFixtureB()->GetUserData().pointer;

		collision.entityA = entityA;
		collision.entityB = entityB;

		collision.friction = contact->GetFriction();
		collision.restitution = contact->GetRestitution();
		collision.tangentSpeed = contact->GetTangentSpeed();

		glm::vec2 contactPoints = { contact->GetManifold()->localPoint.x, contact->GetManifold()->localPoint.y };

		collision.contactPoints = contactPoints;

		//EG_CORE_TRACE("Collision between {0} and {1} at {2}, {3}", entityA, entityB, contactPoints.x, contactPoints.y);

		for(auto& scriptInstance : ScriptEngine::GetEntityScriptInstances(entityA))
			scriptInstance->InvokeOn2DCollisionExit(collision);

		for(auto& scriptInstance : ScriptEngine::GetEntityScriptInstances(entityB))
			scriptInstance->InvokeOn2DCollisionExit(collision);
	}
}