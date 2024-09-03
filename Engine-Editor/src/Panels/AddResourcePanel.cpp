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
#include <Imgui/imgui_internal.h>

namespace eg
{
	static float spaceY = 0;
	static float textY = 0;
	AddResourcePanel::AddResourcePanel()
	{
		m_ImagePanel = CreateRef<ImagePanel>();
		m_AnimationPanel = CreateRef<AnimationPanel>();
		m_ImagePanelinitialized = m_ImagePanel->InitImagePanel();
		m_AnimationPanelinitialized = m_AnimationPanel->InitAnimationPanel();
		m_AnimationIcon = Texture2D::Create("resources/icons/addResourcePanel/animation_icon_white.png");
		m_ShaderIcon = Texture2D::Create("resources/icons/addResourcePanel/shader_icon_white.png");
		m_FontIcon = Texture2D::Create("resources/icons/addResourcePanel/font_icon_white.png");
		m_TextIcon = Texture2D::Create("resources/icons/addResourcePanel/text_icon_white.png");
		m_ImageIcon = Texture2D::Create("resources/icons/addResourcePanel/image_icon_white.png");
		m_ScriptIcon = Texture2D::Create("resources/icons/addResourcePanel/script_icon_white.png");
		m_NativeScriptIcon = Texture2D::Create("resources/icons/addResourcePanel/nativescript_icon_white.png");
		m_CustomIcon = Texture2D::Create("resources/icons/addResourcePanel/custom_icon_white.png");

		auto& io = ImGui::GetIO();
		m_PoppinsLightFont = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Light.ttf", 25.0f, NULL, io.Fonts->GetGlyphRangesDefault());
		IM_ASSERT(m_PoppinsLightFont != NULL);
		m_PoppinsMediumFont = io.Fonts->AddFontFromFileTTF("assets/fonts/poppins/Poppins-Medium.ttf", 50.0f, NULL, io.Fonts->GetGlyphRangesDefault());
		IM_ASSERT(m_PoppinsMediumFont != NULL);
	}
	void AddResourcePanel::OnImGuiRender()
	{
		if (!m_showResourcePanel)
			return;
				int size = 800;
				ImGuiStyle& style = ImGui::GetStyle();
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2((size * 0.05), 20));
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.125f, 0.102f, 0.188f, 1.0f));
				style.WindowRounding = 12.0f;

				ImGui::Begin("Resource Loader",nullptr,ImGuiWindowFlags_NoDecoration);

				ImGui::SetWindowSize(ImVec2(size, 550));
				ImGui::PushFont(m_PoppinsMediumFont);
				TextCenteredOnLine("Resource Loader");
				ImGui::PopFont();
				ImGui::PushFont(m_PoppinsLightFont);
				
				style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0, 0, 0);
				style.Colors[ImGuiCol_ButtonActive] = ImVec4(0, 0, 0, 0);

				if (PositionButtonWithTheSameWidth("Animation",4,1, 150,50))
				{
					bool resourceChosen = ChooseNewResource("Image (*.png)\0*.png\0");
					if (resourceChosen)
					{
						if(m_AnimationPanel)
							m_AnimationPanel->OpenAnimationPanel(m_ResourcePath);
					}
				}
				ImGui::SameLine();
				if (PositionButtonWithTheSameWidth("Shader", 4, 2, 150,50))
				{
					bool resourceChosen = ChooseNewResource("Shader (*.shader)\0*.shader\0");
				}
				ImGui::SameLine();
				if (PositionButtonWithTheSameWidth("Font", 4, 3, 150,50))
				{
					bool resourceChosen = ChooseNewResource("Font (*.ttf)\0*.ttf\0");
				}
				ImGui::SameLine();
				if (PositionButtonWithTheSameWidth("Text", 4, 4, 150,50)) {
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

				style.Colors[ImGuiCol_Button] = ImVec4(0.204f, 0.145f, 0.278f, 1.0f);
				style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);
				style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);

				if(ButtonCenteredOnLine("Cancel", 150, 50, true))
				{
					m_showResourcePanel = false;
				}
				ImGui::PopFont();
				ImGui::End();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();

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

	bool AddResourcePanel::ButtonCenteredOnLine(const char* label, int width, int height,bool isDown, float alignment)
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
			bool OpenPopup = ImGui::Button(label,ImVec2(width,height));
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

	void AddResourcePanel::TextCenteredOnLine(const char* label, float alignment) {
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 40));
		float oldSize = ImGui::GetFont()->Scale;
		//ImGui::GetFont()->Scale *= 2;
		ImGui::PushFont(ImGui::GetFont());

		float windowWidth = ImGui::GetWindowSize().x;
		float textWidth = ImGui::CalcTextSize(label).x;

		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		ImGui::Text(label);
		//ImGui::GetFont()->Scale = oldSize;
		ImGui::PopFont();;
		ImGui::PopStyleVar();
		
	}

	bool AddResourcePanel::PositionButtonWithTheSameWidth(const char* label, int numberOfButtonsInLine, int index, int width, int height) {
		if (index <= numberOfButtonsInLine) {
			ImGuiStyle& style = ImGui::GetStyle();
			const float thumbnailSize = 128.0f;

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
			//ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 80));

			float windowWidth = ImGui::GetWindowSize().x - style.WindowPadding.x * 2;
			float spaceForOneBtn = width + ((windowWidth - width * 4) / (numberOfButtonsInLine - 1));

			float spaceX = spaceForOneBtn * (index - 1) + style.WindowPadding.x;
			if (index == 1)
				spaceY = ImGui::GetCursorPosY();
			ImGui::SetCursorPos(ImVec2(spaceX,spaceY));

			/*if (width && height) {
				bool OpenPopup = ImGui::Button(label, ImVec2(width, height));
				ImGui::PopStyleVar(2);
				return OpenPopup;
			}*/

			bool OpenPopup = RenderFile(label,thumbnailSize);
			ImGui::PopStyleVar();
			DrawCenteredText(label, thumbnailSize + 16.0f,spaceX,index);
			return OpenPopup;

		}
		return NULL;
	}

	void AddResourcePanel::DrawCenteredText(const std::string& label, const float& cellSize, const float& cursorX, const int& btnIndex) {
		auto textWidth = ImGui::CalcTextSize(label.c_str()).x;
		//auto CursorX = ImGui::GetCursorPosX();
		float offset = (cellSize - textWidth) * 0.48f;
		if (btnIndex == 1)
			textY = ImGui::GetCursorPosY();
		ImGui::SetCursorPosX(cursorX + offset);
		ImGui::SetCursorPosY(textY);
		ImGui::TextWrapped(label.c_str());
	}

	bool AddResourcePanel::RenderFile(const std::string& label, const float& thumbnailSize) {
		//static float thumbnailSize = 128.0f;

		ImGui::PushID(label.c_str());

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		bool openPopup = false;
		if(label._Equal("Animation"))
			openPopup = ImGui::ImageButton((ImTextureID)m_AnimationIcon->GetRendererID(), { thumbnailSize, thumbnailSize });
		else if(label._Equal("Shader"))
			openPopup = ImGui::ImageButton((ImTextureID)m_ShaderIcon->GetRendererID(), { thumbnailSize, thumbnailSize });
		else if (label._Equal("Font"))
			openPopup = ImGui::ImageButton((ImTextureID)m_FontIcon->GetRendererID(), { thumbnailSize, thumbnailSize });
		else if (label._Equal("Text"))
			openPopup = ImGui::ImageButton((ImTextureID)m_TextIcon->GetRendererID(), { thumbnailSize, thumbnailSize });
		else if (label._Equal("Image"))
			openPopup = ImGui::ImageButton((ImTextureID)m_ImageIcon->GetRendererID(), { thumbnailSize, thumbnailSize });
		else if (label._Equal("Script"))
			openPopup = ImGui::ImageButton((ImTextureID)m_ScriptIcon->GetRendererID(), { thumbnailSize, thumbnailSize });
		else if (label._Equal("NativeScript"))
			openPopup = ImGui::ImageButton((ImTextureID)m_NativeScriptIcon->GetRendererID(), { thumbnailSize, thumbnailSize });
		else if (label._Equal("Custom"))
			openPopup = ImGui::ImageButton((ImTextureID)m_CustomIcon->GetRendererID(), { thumbnailSize, thumbnailSize });
		ImGui::PopStyleColor();
		ImGui::PopID(); 

		return openPopup;
	}
	
}