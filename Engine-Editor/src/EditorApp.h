#pragma once
#include "Engine.h"
#include "Engine/Core/EntryPoint.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"
#include "Commands/Commands.h"
#include "GLFW/include/GLFW/glfw3.h"
#include "Platform/Windows/WindowsWindow.h"
#include "stb_image.h"

namespace eg
{
	class Editor : public Application
	{
	public:
		Editor(ApplicationSpecification spec);

		~Editor() {}

		EditorLayer* GetEditorLayer() { return static_cast<EditorLayer*>(GetFirstLayer()); }

		void OnUpdate() {}

		bool OnWindowClose(WindowCloseEvent &e);
	};
}