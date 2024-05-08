#pragma once
#include "Geometry.h"

namespace eg {
	class GeometrySystem {
	public:
		GeometrySystem() = default;
		~GeometrySystem() = default;

		static Ref<GeometrySystem> Create();
		void AddGeometry(Ref<Geometry> geometry) { m_Geometries.push_back(geometry); }
		void Draw();

	private:
		std::vector<Ref<Geometry>> m_Geometries;
	};
}