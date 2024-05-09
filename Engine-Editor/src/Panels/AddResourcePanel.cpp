#include "egpch.h"
#include "Engine.h"
#include "AddResourcePanel.h"
#include "Engine/Utils/PlatformUtils.h"
#include "Engine/Resources/resourceTypes.h"
#include "Engine/Utils/PlatformUtils.h"
#include "Engine/Resources/ResourceSerializer.h"
#include "Engine/Resources/resourceTypes.h"
#include <imgui/imgui.h>
#include <imgui/misc/cpp/imgui_stdlib.h>

#include "../EditorLayer.h"

namespace eg
{
	AddResourcePanel::AddResourcePanel()
	{
		m_ImagePanel = CreateRef<ImagePanel>();
		m_AnimationPanel = CreateRef<AnimationPanel>();
		m_ImagePanelinitialized = m_ImagePanel->InitImagePanel();
		m_AnimationPanelinitialized = m_AnimationPanel->InitAnimationPanel();
	}
	void AddResourcePanel::OnImGuiRender()
	{
		if (!m_showResourcePanel)
			return;
				int size = 800;
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2((size * 0.05), 10));
				ImGui::Begin("Resource Loader",nullptr,ImGuiWindowFlags_NoDecoration);
				ImGui::SetWindowSize(ImVec2(size, 500));
				//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
				TextCenteredOnLine("Resource Loader");
				if (PositionButtonWithTheSameWidth("Animation",4,1,150,50)/*ImGui::Button("Animation", ImVec2(150, 50))*/)
				{
					bool resourceChosen = ChooseNewResource("Image (*.png)\0*.png\0");
					if (resourceChosen)
					{
						if(m_AnimationPanel)
							m_AnimationPanel->OpenAnimationPanel(m_ResourcePath);
					}
				}
				ImGui::SameLine();
				if (PositionButtonWithTheSameWidth("Shader", 4, 2,150,50))
				{
					bool resourceChosen = ChooseNewResource("Shader (*.shader)\0*.shader\0");
				}
				ImGui::SameLine();
				if (PositionButtonWithTheSameWidth("Font", 4, 3,150,50))
				{
					bool resourceChosen = ChooseNewResource("Font (*.ttf)\0*.ttf\0");
				}
				ImGui::SameLine();
				if (PositionButtonWithTheSameWidth("Text", 4, 4,150,50)) {
					bool resourceChosen = ChooseNewResource("Text (*.txt)\0*.txt\0");
				}
				if (PositionButtonWithTheSameWidth("Image", 4, 1, 150, 50))
				{
					bool resourceChosen = ChooseNewResource("Image (*.png)\0*.png\0");
					if (resourceChosen)
					{
						if (m_ImagePanel)
							m_ImagePanel->OpenImagePanel(m_ResourcePath);
					}
				}
				ImGui::SameLine();
				if (PositionButtonWithTheSameWidth("Script", 4, 2, 150, 50))
				{
					bool resourceChosen = ChooseNewResource("Script (*.cs)\0*.cs\0");
				}
				ImGui::SameLine();
				if (PositionButtonWithTheSameWidth("NativeScript", 4, 3, 150, 50))
				{
					bool resourceChosen = ChooseNewResource("NativeScript (*.dll)\0*.dll\0");
				}
				ImGui::SameLine();
				if (PositionButtonWithTheSameWidth("Custom", 4, 4, 150, 50))
				{
					bool resourceChosen = ChooseNewResource("Custom (*.custom)\0*.custom\0");
				}
				if(ButtonCenteredOnLine("Cancel", 150, 50))
				{
					m_showResourcePanel = false;
				}

				ImGui::End();
				ImGui::PopStyleVar();

		m_ImagePanel->OnImGuiRender();
		m_AnimationPanel->OnImGuiRender();
	}

	void AddResourcePanel::Update(float ts)
	{
		if(m_AnimationPanel->IsAnimationPanelOpen())
			m_AnimationPanel->OnUpdate(ts);
	}

	bool AddResourcePanel::ChooseNewResource(const std::string filter)
	{
		std::filesystem::path path = FileDialogs::OpenFile(filter.c_str());
		if (path.empty())
		{
			return false;
		}
		m_ResourcePath = path;
		return true;
	}

	bool AddResourcePanel::ButtonCenteredOnLine(const char* label, int width, int height, float alignment)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		float windowWidth = ImGui::GetWindowSize().x;
		float textWidth = ImGui::CalcTextSize(label).x;
		textWidth += style.WindowPadding.x * 2.0f;

		if (width && height)
		{
			textWidth = width;
			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			return ImGui::Button(label, ImVec2(width, height));
		}

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		return ImGui::Button(label);
	}

	void AddResourcePanel::TextCenteredOnLine(const char* label, float alignment) {
		float windowWidth = ImGui::GetWindowSize().x;
		float textWidth = ImGui::CalcTextSize(label).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(label);
	}

	bool AddResourcePanel::PositionButtonWithTheSameWidth(const char* label,int numberOfButtonsInLine,int index, int width, int height) {
		if (index <= numberOfButtonsInLine) {
			ImGuiStyle& style = ImGui::GetStyle();
			float windowWidth = ImGui::GetWindowSize().x - style.WindowPadding.x * 2; 
			float spaceForOneBtn = width + ((windowWidth - width * 4) / (numberOfButtonsInLine - 1));
			
			float space = spaceForOneBtn * (index - 1) + style.WindowPadding.x;
			ImGui::SetCursorPosX(space);

			if (width && height)
				return ImGui::Button(label, ImVec2(width, height));

			return ImGui::Button(label);
			
		}
		return NULL;
	}

	
}