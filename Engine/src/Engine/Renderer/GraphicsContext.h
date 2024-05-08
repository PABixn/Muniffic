#pragma once
#include "GeometrySystem.h"
namespace eg
{
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void StartFrame() = 0;
		virtual void SwapBuffers() = 0;

		virtual Ref<GeometrySystem> CreateGeometrySystem() = 0;
	};
}