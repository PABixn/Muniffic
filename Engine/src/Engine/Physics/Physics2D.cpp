#include "egpch.h"
#include "Physics2D.h"

namespace eg
{
	namespace Utils
	{
		void RecreateFixture(const RigidBody2DComponent& rb, BoxCollider2DComponent& bc, const TransformComponent& transform)
		{
		    EG_PROFILE_FUNCTION();
			b2Body* body = (b2Body*)rb.RuntimeBody;

			if (!body)
				return;

			b2Fixture* fixture = body->GetFixtureList();

			while (fixture)
			{
				if (fixture->GetShape()->GetType() == b2Shape::Type::e_polygon)
				{
					body->DestroyFixture(fixture);
					break;
				}

				fixture = fixture->GetNext();
			}

			b2PolygonShape shape;
			shape.SetAsBox(bc.Size.x * transform.Scale.x, bc.Size.y * transform.Scale.y, { bc.Offset.x, bc.Offset.y }, 0.0f);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = bc.Density;
			fixtureDef.friction = bc.Friction;
			fixtureDef.restitution = bc.Restitution;
			fixtureDef.restitutionThreshold = bc.RestitutionThreshold;
			fixtureDef.isSensor = bc.IsSensor;

			b2Fixture* newFixture = body->CreateFixture(&fixtureDef);
			bc.RuntimeFixture = newFixture;
		}

		void RecreateFixture(const RigidBody2DComponent& rb, CircleCollider2DComponent& cc, const TransformComponent& transform)
		{
		    EG_PROFILE_FUNCTION();
			b2Body* body = (b2Body*)rb.RuntimeBody;

			if (!body)
				return;

			b2Fixture* fixture = body->GetFixtureList();

			while (fixture)
			{
				if (fixture->GetShape()->GetType() == b2Shape::Type::e_circle)
				{
					body->DestroyFixture(fixture);
					break;
				}

				fixture = fixture->GetNext();
			}

			b2CircleShape shape;
			shape.m_radius = cc.Radius * transform.Scale.x;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &shape;
			fixtureDef.density = cc.Density;
			fixtureDef.friction = cc.Friction;
			fixtureDef.restitution = cc.Restitution;
			fixtureDef.restitutionThreshold = cc.RestitutionThreshold;
			fixtureDef.isSensor = cc.IsSensor;

			b2Fixture* newFixture = body->CreateFixture(&fixtureDef);
			cc.RuntimeFixture = newFixture;
		}

		void RecreateFixture(const RigidBody2DComponent& rb, PolyCollider2DComponent& pc, const TransformComponent& transform)
		{
			b2Body* body = (b2Body*)rb.RuntimeBody;

			if (!body)
				return;

			b2Fixture* fixture = body->GetFixtureList();

			while (fixture)
			{
				if (fixture->GetShape()->GetType() == b2Shape::Type::e_polygon)
				{
					body->DestroyFixture(fixture);
					break;
				}

				fixture = fixture->GetNext();
			}

			b2PolygonShape shape;
			//b2Vec2 vertex = b2Vec2(pc.Vertices.at(0).x, pc.Vertices.at(0).y);
			if (pc.VertexCount != 0) {
				b2Vec2* vertices = new b2Vec2[pc.VertexCount];
				for (int i = 0; i < pc.VertexCount; i++) {
					vertices[i] = b2Vec2(pc.Vertices.at(i).x * pc.Size.x * transform.Scale.x, pc.Vertices.at(i).y * pc.Size.y * transform.Scale.y);
				}
				//b2Vec2* VerticesPtr = &pc.Vertices;
				shape.Set(vertices, pc.VertexCount);
				

				delete[] vertices;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &shape;
				fixtureDef.density = pc.Density;
				fixtureDef.friction = pc.Friction;
				fixtureDef.restitution = pc.Restitution;
				fixtureDef.restitutionThreshold = pc.RestitutionThreshold;
				fixtureDef.isSensor = pc.IsSensor;

				b2Fixture* newFixture = body->CreateFixture(&fixtureDef);
				pc.RuntimeFixture = newFixture;
			}
		}


	}
}
