#include "EditorApp.h"
#include <Imgui/imgui.h>
#include "EditorLayer.h"
#include "Engine/Resources/ResourceSerializer.h"

namespace eg {
Editor::Editor(ApplicationSpecification spec)
    : Application(spec)
    {
        PushLayer(new EditorLayer());
        GLFWimage images[1];
        images[0].pixels = stbi_load(LOGO_DIRECTORY, &images[0].width, &images[0].height, 0, 4);
        const char* failReason = stbi_failure_reason();

        glfwSetWindowIcon((GLFWwindow*)this->GetWindow().GetNativeWindow(), 1, images);
        stbi_image_free(images[0].pixels);
    };

bool Editor::OnWindowClose(WindowCloseEvent& e)

	Editor::~Editor()
	{
	}
	bool Editor::OnWindowClose(WindowCloseEvent& e)
	{
		EG_PROFILE_FUNCTION();

		if (IsProjectSaved())
		{
			SetRunning(false);
			return true;
		}
		ResourceSerializer::SerializeResourceCache();
		(*(dynamic_cast<EditorLayer*>(this->GetFirstLayer()))).SetIsWindowTryingToClose(true);
		return false;
	}

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		EG_PROFILE_FUNCTION();

		ApplicationSpecification spec;
		spec.Name = "Editor";
		spec.CommandLineArgs = args;

		return new Editor(spec);
	};
}