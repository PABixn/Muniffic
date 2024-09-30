#include "IconLoader.h"
#include <filesystem>

namespace eg
{
	void IconLoader::LoadIcons()
	{
		for(int i = 0; i < static_cast<int>(Icons::Component_Puzzle); i++)
		{
			std::filesystem::path iconPath = GetIconPath(static_cast<Icons>(i));
			Ref<Texture2D> icon = Texture2D::Create(iconPath.string());
			if(icon->IsLoaded())
				m_Icons[static_cast<Icons>(i)] = Texture2D::Create(iconPath.string());
			else
				EG_WARN("Could not load texture {0}", iconPath.string());
		}
	}

	std::filesystem::path IconLoader::GetIconPath(Icons icon)
	{
		std::filesystem::path basePath = "resources/icons/";
		std::filesystem::path editorPath = basePath / "editor/";
		std::filesystem::path welcomePanelPath = basePath / "welcome_panel/";
		std::filesystem::path contentBrowserPath = basePath / "content_browser/";
		std::filesystem::path componentPath = basePath / "component/";

		switch (icon)
		{
			case Icons::Editor_PauseButton:
				return editorPath / "pause_button.png";
			case Icons::Editor_PlayButton:
				return editorPath / "play_button.png";
			case Icons::Editor_StopButton:
				return editorPath / "stop_button.png";
			case Icons::Editor_StepButton:
				return editorPath / "step_button.png";
			case Icons::Editor_SimulateButton:
				return editorPath / "simulate_button.png";
			case Icons::Editor_ResourceNotFound:
				return editorPath / "resource_not_found.png";
			case Icons::WelcomePanel_DeleteProject:
				return welcomePanelPath / "delete_project.png";
			case Icons::WelcomePanel_NewProject:
				return welcomePanelPath / "new_project.png";
			case Icons::ContentBrowser_Directory:
				return contentBrowserPath / "directory.png";
			case Icons::ContentBrowser_File:
				return contentBrowserPath / "file.png";
			case Icons::Component_Icon:
				return componentPath / "component_icon.png";
			case Icons::Component_Transform:
				return componentPath / "transform.png";
			case Icons::Component_Camera:
				return componentPath / "camera.png";
			case Icons::Component_Script:
				return componentPath / "script.png";
			case Icons::Component_SpriteRenderer:
				return componentPath / "sprite_renderer.png";
			case Icons::Component_SubTextureRenderer:
				return componentPath / "sub_texture_renderer.png";
			case Icons::Component_CircleRenderer:
				return componentPath / "circle_renderer.png";
			case Icons::Component_RigidBody2D:
				return componentPath / "rigid_body_2d.png";
			case Icons::Component_BoxCollider2D:
				return componentPath / "box_collider_2d.png";
			case Icons::Component_CircleCollider2D:
				return componentPath / "circle_collider_2d.png";
			case Icons::Component_TextRenderer:
				return componentPath / "text_renderer.png";
			case Icons::Component_Animator:
				return componentPath / "animator.png";
			case Icons::Component_Puzzle:
				return componentPath / "puzzle.png";
			default:
				return editorPath / "resource_not_found.png";
		}
	}
}