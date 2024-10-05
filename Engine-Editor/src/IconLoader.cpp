#include "IconLoader.h"
#include <filesystem>

namespace eg
{
	std::unordered_map<Icons, Ref<Texture2D>> IconLoader::m_Icons = std::unordered_map<Icons, Ref<Texture2D>>();
	Ref<Texture2D> IconLoader::m_DefaultIcon = nullptr;

	Ref<Texture2D> IconLoader::GetIcon(Icons icon)
	{
		if(m_Icons.find(icon) == m_Icons.end())
		{
			EG_WARN("Icon {0} not found", static_cast<int>(icon));
			return m_DefaultIcon;
		}
		return m_Icons[icon];
	}

	void IconLoader::LoadIcons()
	{
		Ref<Texture2D> defaultIcon = Texture2D::Create("resources/graphics/image_not_found.png");

		if (!defaultIcon->IsLoaded())
		{
			EG_WARN("Could not load default texture {0}", "resources/graphics/image_not_found.png");
			return;
		}

		m_DefaultIcon = defaultIcon;

		for(int i = 0; i < static_cast<int>(Icons::MAX); i++)
		{
			std::filesystem::path iconPath = GetIconPath(static_cast<Icons>(i));
			Ref<Texture2D> icon = Texture2D::Create(iconPath.string());

			if (icon == nullptr)
			{
				EG_WARN("Could not load texture {0}", iconPath.string());
				icon = m_DefaultIcon;
				m_Icons[static_cast<Icons>(i)] = icon;
			}
			else
			{
				if (icon->IsLoaded())
					m_Icons[static_cast<Icons>(i)] = icon;
				else
					EG_WARN("Could not load texture {0}", iconPath.string());
			}
		}
	}

	std::filesystem::path IconLoader::GetIconPath(Icons icon)
	{
		std::filesystem::path basePath = "resources/icons/";
		std::filesystem::path editorPath = basePath / "editor/";
		std::filesystem::path welcomePanelPath = basePath / "welcome_panel/";
		std::filesystem::path contentBrowserPath = basePath / "content_browser/";
		std::filesystem::path componentPath = basePath / "component/";
		std::filesystem::path addResourcePath = basePath / "add_resource/";
		std::filesystem::path animationEditorPath = basePath / "animation_editor/";
		std::filesystem::path assistantPath = basePath / "assistant/";

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
			case Icons::Component_Audio:
				return componentPath / "audio.png";
			case Icons::Component_Puzzle:
				return componentPath / "puzzle.png";
			case Icons::AddResource_Animation:
				return addResourcePath / "animation.png";
			case Icons::AddResource_Custom:
				return addResourcePath / "custom.png";
			case Icons::AddResource_Font:
				return addResourcePath / "font.png";
			case Icons::AddResource_Shader:
				return addResourcePath / "shader.png";
			case Icons::AddResource_Text:
				return addResourcePath / "text.png";
			case Icons::AddResource_Image:
				return addResourcePath / "image.png";
			case Icons::AddResource_Script:
				return addResourcePath / "script.png";
			case Icons::AddResource_NativeScript:
				return addResourcePath / "native_script.png";
			case Icons::AnimationEditor_LengthChange:
				return animationEditorPath / "length_change.png";
			case Icons::AnimationEditor_LengthChangeSelected:
				return animationEditorPath / "length_change_selected.png";
			case Icons::AnimationEditor_MoveIcon:
				return animationEditorPath / "move_icon.png";
			case Icons::AnimationEditor_MoveIconSelected:
				return animationEditorPath / "move_icon_selected.png";
			case Icons::Assistant_Settings:
				return assistantPath / "settings.png";
			case Icons::Assistant_CopyCode:
				return assistantPath / "copy_code.png";
			case Icons::Assistant_Microphone:
				return assistantPath / "microphone.png";
			case Icons::Assistant_MicrophoneOff:
				return assistantPath / "microphone_off.png";
			case Icons::Assistant_MicrophoneUnavailable:
				return assistantPath / "microphone_unavailable.png";
			case Icons::Assistant_Read:
				return assistantPath / "read.png";
			case Icons::Assistant_Send:
				return assistantPath / "send.png";

			default:
				return "resources/graphics/resource_not_found.png";
		}
	}
}