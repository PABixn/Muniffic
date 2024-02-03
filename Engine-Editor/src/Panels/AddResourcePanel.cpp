#include "egpch.h"
#include "AddResourcePanel.h"
#include "Engine/Utils/PlatformUtils.h"
#include <imgui/imgui.h>
#include "ResourcesPanels/ImagePanel.h"

namespace eg
{
	void AddResourcePanel::OnImGuiRender()
	{
		switch(m_ChosenResourceType)
		{
			case ResourceType::None:
			{
				ImGui::Begin("Resource Loader");

				if (ImGui::Button("Animation"))
				{
					m_ChosenResourceType = ResourceType::Animation;
					bool resourceChosen = ChooseNewResource("Animation (*.anim)\0*.anim\0");
				}
				if (ImGui::Button("Shader"))
				{
					m_ChosenResourceType = ResourceType::Shader;
					bool resourceChosen = ChooseNewResource("Shader (*.shader)\0*.shader\0");
				}
				if (ImGui::Button("Font"))
				{
					m_ChosenResourceType = ResourceType::Font;
					bool resourceChosen = ChooseNewResource("Font (*.ttf)\0*.ttf\0");
				}
				if (ImGui::Button("Text")) {
					m_ChosenResourceType = ResourceType::Text;
					bool resourceChosen = ChooseNewResource("Text (*.txt)\0*.txt\0");
				}
				if (ImGui::Button("Image"))
				{
					m_ChosenResourceType = ResourceType::Image;
					bool resourceChosen = ChooseNewResource("Image (*.png)\0*.png\0");
					if (resourceChosen)
					{
						m_ImagePanel = ImagePanel(m_ResourcePath);
					}
				}
				if (ImGui::Button("Script"))
				{
					m_ChosenResourceType = ResourceType::Script;
					bool resourceChosen = ChooseNewResource("Script (*.cs)\0*.cs\0");
				}
				if (ImGui::Button("NativeScript"))
				{
					m_ChosenResourceType = ResourceType::NativeScript;
					bool resourceChosen = ChooseNewResource("NativeScript (*.dll)\0*.dll\0");
				}
				if (ImGui::Button("Custom"))
				{
					m_ChosenResourceType = ResourceType::Custom;
					bool resourceChosen = ChooseNewResource("Custom (*.custom)\0*.custom\0");
				}

				ImGui::End();
				break;
			}
			case ResourceType::Image:
			{
				m_ImagePanel.OnImGuiRender();
				break;
			}
		}
		
	}
	bool AddResourcePanel::ChooseNewResource(const std::string filter)
	{
		std::filesystem::path path = FileDialogs::OpenFile(filter.c_str());
		if (path.empty())
		{
			m_ChosenResourceType = ResourceType::None;
			return false;
		}
		m_ResourcePath = path;
		return true;
	}
}