#include "egpch.h"
#include "Components.h"
#include "box2d.h"

namespace eg {
    bool BoxCollider2DComponent::CollidesWith(const BoxCollider2DComponent& other)
    {
        b2PolygonShape* poly = (b2PolygonShape*);

    }
	bool CollidesWith(const CircleCollider2DComponent& other);
	bool CollidesWith(glm::vec2* point);
	bool CollidesWith(glm::vec2* position, glm::vec2* size);
	bool CollidesWith(glm::vec2* position, float radius);
	bool CollidesWith(const std::vector<glm::vec2>& vertices);
	bool CollidesWithEdge(glm::vec2* v0, glm::vec2* v1);
	bool CollidesWithTopEdge(const BoxCollider2DComponent& other);
	bool CollidesWithBottomEdge(const BoxCollider2DComponent& other);
	bool CollidesWithLeftEdge(const BoxCollider2DComponent& other);
	bool CollidesWithRightEdge(const BoxCollider2DComponent& other);
}