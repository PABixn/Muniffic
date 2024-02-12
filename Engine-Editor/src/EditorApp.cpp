#include "EditorApp.h"
#include <Imgui/imgui.h>
#include "EditorLayer.h"
namespace eg {

	bool Editor::OnWindowClose(WindowCloseEvent& e)
	{
		if (GetIsSaved())
		{
			SetRunning(false);
			return true;
		}
		(*(dynamic_cast<EditorLayer*>(this->GetFirstLayer()))).GetUnsavedChangesPanel()->SetUnsavedChangesPanelRender(true);
		return false;
	}

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		ApplicationSpecification spec;
		spec.Name = "Muniffic editor";
		spec.CommandLineArgs = args;

		return new Editor(spec);
	};


}