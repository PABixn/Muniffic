#include "UnsavedChangesPanel.h"
#include "ConsolePanel.h"
namespace eg {
	bool UnsavedChangesPanelRender = false;
	void UnsavedChangesPanel::SetUnsavedChangesPanelRender(bool val) {
		UnsavedChangesPanelRender = val;
	}
	bool UnsavedChangesPanel::GetUnsavedChangesPanelRender() {
		return UnsavedChangesPanelRender;
	}
	UnsavedChangesPanel::UnsavedChangesPanel() {};
	void UnsavedChangesPanel::OnImGuiRender() {
		ImGui::SetNextWindowPos(ImVec2(Application::Get().GetWindow().GetWidth()/2, Application::Get().GetWindow().GetHeight() / 2));
		/*
		ImGui::SetNextWindowSize(ImVec2(350, 350));*/
		ImGui::Begin("Unsaved Changes", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		ImGui::Text("Warning: Unsaved changes");
		ConsolePanel::Log("You have unsaved changes", ConsolePanel::LogType::Warning);
		bool SaveBttn = ImGui::Button("Save");
		ImGui::SameLine();
		bool NotSaveBttn = ImGui::Button("Don't save");
		ImGui::SameLine();
		bool CancelBttn = ImGui::Button("Cancel");
		if (SaveBttn) {
			(*(dynamic_cast<EditorLayer*>(Application::Get().GetFirstLayer()))).Save();
			Application::Get().Close();
		}else if (NotSaveBttn) {
			Application::Get().Close();
		}
		else if (CancelBttn) {
			SetUnsavedChangesPanelRender(false);
		}
		ImGui::End();
	};
}
