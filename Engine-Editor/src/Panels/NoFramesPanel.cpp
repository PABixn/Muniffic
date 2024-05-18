#include "NoFramesPanel.h"
#include "Imgui/imgui.h"

namespace eg {
	NoFramesPanel::NoFramesPanel() {}

	bool NoFramesPanel::OpenNoFramesPanel() {
		ShowNoFramesPanel(true);
		return true;
	}

	void NoFramesPanel::OnImGuiRender() {
		if (!m_ShowNoFramesPanel)
			return;
		ImGui::Begin("No Selected Frames", nullptr, ImGuiWindowFlags_NoDecoration);
		ImGui::SetWindowSize(ImVec2(300, 150));
		TextCenteredOnLine("At least one frame must be selected");
		if (ButtonCenteredOnLine("Close",100,50,true))
			ShowNoFramesPanel(false);
		ImGui::End();
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

	void NoFramesPanel::TextCenteredOnLine(const char* label, float alignment) {
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 40));

		float windowWidth = ImGui::GetWindowSize().x;
		float textWidth = ImGui::CalcTextSize(label).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(label);
		ImGui::PopStyleVar();

	}

}