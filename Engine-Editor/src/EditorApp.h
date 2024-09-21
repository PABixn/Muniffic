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


namespace eg {


	class Editor : public Application
	{
	public:
		Editor(ApplicationSpecification spec)
			: Application(spec) {
			PushLayer(new EditorLayer());
			GLFWimage images[1]; 
			images[0].pixels = stbi_load(LOGO_DIRECTORY, &images[0].width, &images[0].height, 0, 4); 
			const char* failReason = stbi_failure_reason();

			glfwSetWindowIcon((GLFWwindow*)this->GetWindow().GetNativeWindow(), 1, images);
			stbi_image_free(images[0].pixels);
		};

		~Editor() {}

		EditorLayer* GetEditorLayer() { return static_cast<EditorLayer*>(GetFirstLayer()); }

		void OnUpdate() {
		}
		bool OnWindowClose(WindowCloseEvent& e);
	};
}