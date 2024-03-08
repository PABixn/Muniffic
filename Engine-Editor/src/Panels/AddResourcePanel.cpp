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
	}
	void AddResourcePanel::OnImGuiRender()
	{
		if (!m_showResourcePanel)
			return;
				ImGui::Begin("Resource Loader");

				if (ImGui::Button("Animation"))
				{
					bool resourceChosen = ChooseNewResource("Image (*.png)\0*.png\0");
					if (resourceChosen)
					{
						bool initialized = m_AnimationPanel->InitAnimationPanel(m_ResourcePath);
						if(initialized)
							m_AnimationPanel->ShowAnimationPanel(true);
					}
				}
				if (ImGui::Button("Shader"))
				{
					bool resourceChosen = ChooseNewResource("Shader (*.shader)\0*.shader\0");
				}
				if (ImGui::Button("Font"))
				{
					bool resourceChosen = ChooseNewResource("Font (*.ttf)\0*.ttf\0");
				}
				if (ImGui::Button("Text")) {
					bool resourceChosen = ChooseNewResource("Text (*.txt)\0*.txt\0");
				}
				if (ImGui::Button("Image"))
				{
					bool resourceChosen = ChooseNewResource("Image (*.png)\0*.png\0");
					if (resourceChosen)
					{
						bool initialized = m_ImagePanel->InitImagePanel(m_ResourcePath);
						if (initialized)
							m_ImagePanel->ShowImagePanel(true);
					}
				}
				if (ImGui::Button("Script"))
				{
					bool resourceChosen = ChooseNewResource("Script (*.cs)\0*.cs\0");
				}
				if (ImGui::Button("NativeScript"))
				{
					bool resourceChosen = ChooseNewResource("NativeScript (*.dll)\0*.dll\0");
				}
				if (ImGui::Button("Custom"))
				{
					bool resourceChosen = ChooseNewResource("Custom (*.custom)\0*.custom\0");
				}

				if(ImGui::Button("Cancel"))
				{
					m_showResourcePanel = false;
				}

				ImGui::End();

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

}