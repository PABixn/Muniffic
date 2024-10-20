#include "EditorApp.h"
#include <Imgui/imgui.h>
#include "EditorLayer.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "./IconLoader.h"

namespace eg {
Editor::Editor(ApplicationSpecification spec)
    : Application(spec)
    {
        PushLayer(new EditorLayer());
        GLFWimage images[1];
        images[0].pixels = stbi_load(LOGO_DIRECTORY, &images[0].width, &images[0].height, 0, 4);
        const char* failReason = stbi_failure_reason();

		SetWindowIcon((GLFWwindow*)this->GetWindow().GetNativeWindow());
        //glfwSetWindowIcon((GLFWwindow*)this->GetWindow().GetNativeWindow(), 1, images);
        stbi_image_free(images[0].pixels);
    };


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

	void Editor::SetWindowIcon(GLFWwindow* window) {
		glBindTexture(GL_TEXTURE_2D, IconLoader::GetIcon(Icons::Editor_Logo)->GetRendererID());

		int width = IconLoader::GetIcon(Icons::Editor_Logo)->GetWidth();
		int height = IconLoader::GetIcon(Icons::Editor_Logo)->GetHeight();

		std::vector<unsigned char> pixels(width * height * 4);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

		GLFWimage image;
		image.width = width;
		image.height = height;
		image.pixels = pixels.data();
		glfwSetWindowIcon(window, 1, &image);

		glBindTexture(GL_TEXTURE_2D, 0);

	}

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		EG_PROFILE_FUNCTION();

		ApplicationSpecification spec;
		spec.Name = "Editor";
		spec.CommandLineArgs = args;

		return new Editor(spec);
	};
}