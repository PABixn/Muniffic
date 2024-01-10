#pragma once
#include "Engine.h"
#include "Engine/Core/EntryPoint.h"
#include "Imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"
#include "Commands/Commands.h"
#include "GLFW/include/GLFW/glfw3.h"
#include "Platform/Windows/WindowsWindow.h"

namespace eg {
	class Editor : public Application
	{
	public:
		Editor(ApplicationSpecification spec)
			: Application(spec) {
			PushLayer(new EditorLayer());
			ChangeName("veri Silli editor");
		};

		~Editor() {}

		void OnUpdate() {}
		bool OnWindowClose(WindowCloseEvent& e);
		void ChangeName(const char* text) ;
	};
}