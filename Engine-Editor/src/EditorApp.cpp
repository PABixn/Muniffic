#include "EditorApp.h"
#include <Imgui/imgui.h>
#include "EditorLayer.h"
#include "Engine/Resources/ResourceSerializer.h"

namespace eg {

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