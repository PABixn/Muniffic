#include "NoFramesPanel.h"
#include "Imgui/imgui.h"

namespace eg {
	NoFramesPanel::NoFramesPanel() {}

	void NoFramesPanel::OpenNoFramesPanel() {
		m_ShowNoFramesPanel = true;
	}

	void NoFramesPanel::OnImGuiRender() {
		if (!m_ShowNoFramesPanel)
			return;
		ImGui::Begin("No Selected Frames", nullptr, ImGuiWindowFlags_NoDecoration);
		ImGui::SetWindowSize(ImVec2(300, 150));
		ImGui::Text("At least one frame must be selected");
		if (ButtonCenteredOnLine("Close",100,50,true));
			m_ShowNoFramesPanel = false;
	}

	bool NoFramesPanel::ButtonCenteredOnLine(const char* label, int width, int height, bool isDown, float alignment)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);

		float windowWidth = ImGui::GetWindowSize().x;
		float textWidth = ImGui::CalcTextSize(label).x;
		textWidth += style.WindowPadding.x * 2.0f;

		if (width && height)
		{
			if (isDown)
				ImGui::SetCursorPosY(ImGui::GetWindowSize().y - style.WindowPadding.y - windowWidth * 0.025 - height);
			textWidth = width;
			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			bool OpenPopup = ImGui::Button(label, ImVec2(width, height));
			ImGui::PopStyleVar();
			return OpenPopup;
		}

		if (isDown)
			ImGui::SetCursorPosY(ImGui::GetWindowSize().y - style.WindowPadding.y - windowWidth * 0.025 - ImGui::CalcTextSize(label).y);
		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		bool OpenPopup = ImGui::Button(label);
		ImGui::PopStyleVar();
		return OpenPopup;
	}

}