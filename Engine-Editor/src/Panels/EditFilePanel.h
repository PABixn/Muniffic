#pragma once

#include "Engine/Core/Application.h"
#include "Engine/Renderer/Texture.h"
#include "Engine/Renderer/SubTexture2D.h"
#include "InvalidDataPanel.h"
#include "Imgui/imgui.h"

namespace eg{
	class EditFilePanel
	{
	public:
		EditFilePanel();
		void OnImGuiRender();
		void ShowWindow(std::filesystem::path keyPath, ResourceType type);
		std::filesystem::path GetKeyPath() { return m_Path; }
		void SetPath(std::filesystem::path keyPath) { m_Path = keyPath; }
		ResourceType GetType() { return m_Type; }
		void SetType(ResourceType type) { m_Type = type; }
		bool IsShown() { return m_Show; }
		void SetInvalidDataPanel(InvalidDataPanel* invalidDataPanel) { m_InvalidDataPanel = invalidDataPanel; }

	protected:
		bool m_Show;
		std::filesystem::path m_Path;
		ResourceType m_Type;
		glm::vec2 m_SpriteSize;
		Ref<Texture2D> m_SpriteSheet;
		InvalidDataPanel* m_InvalidDataPanel;
		std::vector<std::shared_ptr<SubTexture2D>> m_Sprites;
	};
}
