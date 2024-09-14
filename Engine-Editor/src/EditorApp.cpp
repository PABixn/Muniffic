#include "EditorApp.h"
#include <Imgui/imgui.h>
#include "EditorLayer.h"
#include "Engine/Resources/ResourceSerializer.h"

namespace eg {
	bool Editor::OnWindowClose(WindowCloseEvent& e)
	{
		if (GetIsSaved())
		{
			SetRunning(false);
			return true;
		}
		ResourceSerializer::SerializeResourceCache();
		(*(dynamic_cast<EditorLayer*>(this->GetFirstLayer()))).GetUnsavedChangesPanel()->SetUnsavedChangesPanelRender(true);
		return false;
	}

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		//m_WelcomingPanel = CreateScope<WelcomingPanel>();
		ApplicationSpecification spec;
		spec.Name = "Editor";
		spec.CommandLineArgs = args;
		//Welcome welcome = new Welcome(spec);

		//if(Welcome)
		//return new Welcome(spec);

		return new Editor(spec);
	};


}