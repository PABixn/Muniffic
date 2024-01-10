#include "EditorApp.h"
namespace eg {

	bool Editor::OnWindowClose(WindowCloseEvent& e)
	{
		if (GetIsSaved())
		{
			SetRunning(false);
			return true;
		}
		return false;
	}
	void Editor::ChangeName(const char* text) {
			if (auto WinWindow = dynamic_cast<WindowsWindow*>(&this->GetWindow()))
				glfwSetWindowTitle((WinWindow)->GetGLFWwindow(), text);
		}

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		ApplicationSpecification spec;
		spec.Name = "Editor";
		spec.CommandLineArgs = args;

		return new Editor(spec);
	};

}