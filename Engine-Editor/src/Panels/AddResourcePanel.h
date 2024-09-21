#pragma once
#include "Engine.h"
#include <filesystem>
#include "Engine/Resources/Systems/ResourceSystem.h"
#include "ResourcesPanels/AnimationPanel.h"
#include "ResourcesPanels/ImagePanel.h"
#include <imgui/imgui.h>


namespace eg {
	class EditorLayer;
	class AddResourcePanel {
	public:
		AddResourcePanel();

		void OnImGuiRender();
		void Update(float ts);

		bool ChooseNewResource(const std::string filter);
		void showResourcePanel(bool show) { m_showResourcePanel = show; }

		bool ButtonCenteredOnLine(const char* label, int width = 0, int height = 0, bool IsDown = false, float alignment = 0.5f);
		void TextCenteredOnLine(const char* label, float alignment = 0.5f);
		bool PositionButtonWithTheSameWidth(const char* label,int numberOfButtonsInLine,int index, int width = 0, int height = 0);
		void DrawCenteredText(const std::string& label, const float& cellSize, const float& cursorX, const int& btnIndex);
		bool RenderFile(const std::string& label, const float& thumbnailSize);
		void LoadIcons();
		bool IsResourcePanelOpen() { return m_showResourcePanel; }
	private:
		Ref<ImagePanel> m_ImagePanel;
		Ref<AnimationPanel> m_AnimationPanel;
		Ref<Texture2D> m_AnimationIcon;
		Ref<Texture2D> m_ShaderIcon;
		Ref<Texture2D> m_FontIcon;
		Ref<Texture2D> m_TextIcon;
		Ref<Texture2D> m_ImageIcon;
		Ref<Texture2D> m_ScriptIcon;
		Ref<Texture2D> m_NativeScriptIcon;
		Ref<Texture2D> m_CustomIcon;

		bool m_showResourcePanel = false;
		std::filesystem::path m_ResourcePath;
		bool m_ImagePanelinitialized = false;
		bool m_AnimationPanelinitialized = false;
	};
}