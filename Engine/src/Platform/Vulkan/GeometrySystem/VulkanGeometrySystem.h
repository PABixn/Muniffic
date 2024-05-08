#pragma once
#include "VulkanGeometry.h"
#include "Engine/Renderer/GeometrySystem.h"

namespace eg {
	class VulkanGeometrySystem : public GeometrySystem {
	public:
		VulkanGeometrySystem() = default;
		~VulkanGeometrySystem() = default;

		void AddGeometry(Geometry& geometry) override { m_Geometries.push_back(geometry); }
		void Draw() override;
	private:
		std::vector<VulkanGeometry&> m_Geometries;
	};
}