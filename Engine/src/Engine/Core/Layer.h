#pragma once
#include "../../Engine/vendor/Glad/include/glad/glad.h"
#include "Core.h"
#include "Engine/Events/Event.h"
#include "Engine/Core/Timestep.h"

namespace eg
{
	class Layer
	{
	public:
		Layer( const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach(){}
		virtual void OnDetach(){}
		virtual void OnUpdate(Timestep ts){}
		virtual void OnImGuiRender(){}
		virtual void OnEvent(Event& event){}

		inline const std::string& GetName() const { return m_DebugName; }
		//inline const GLFWwindow& GetGLFWwindow() { return *m_Window; }
	protected:
		std::string m_DebugName;
		//GLFWwindow* m_Window;
	};
}