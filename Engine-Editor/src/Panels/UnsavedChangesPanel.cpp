#include "UnsavedChangesPanel.h"
namespace eg {
	bool UnsavedChangesPanelRender = false;

	void UnsavedChangesPanel::SetUnsavedChangesPanelRender(bool val) {
		UnsavedChangesPanelRender = val;
	}
	bool UnsavedChangesPanel::GetUnsavedChangesPanelRender() {
		return UnsavedChangesPanelRender;
	}
	UnsavedChangesPanel::UnsavedChangesPanel() {};
	void UnsavedChangesPanel::OnImGuiRender() {/*
		ImGui::SetNextWindowPos(ImVec2(1050, 1050));
		ImGui::SetNextWindowSize(ImVec2(350, 350));*/
		ImGui::Begin("Unsaved Changes", nullptr, ImGuiWindowFlags_NoTitleBar);
		ImGui::Text("Warning: Unsaved changes");
		if (ImGui::Button("Save")) {
			(*(dynamic_cast<EditorLayer*>(Application::Get().GetFirstLayer()))).Save();
			Application::Get().Close();
		}else if (ImGui::Button("Don't save")) {
			Application::Get().Close();
		}
		else if (ImGui::Button("Cancel")) {
			SetUnsavedChangesPanelRender(false);
		}
		ImGui::End();
	};
}
