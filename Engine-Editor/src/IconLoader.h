#pragma once
#include <string>
#include <unordered_map>
#include "Engine.h"

namespace eg
{
	enum class Icons
	{
		Editor_PlayButton = 0,
		Editor_PauseButton,
		Editor_StopButton,
		Editor_StepButton,
		Editor_SimulateButton,
		ResourceNotFound,
		WelcomePanel_NewProject,
		WelcomePanel_DeleteProject,
		ContentBrowser_Directory,
		ContentBrowser_File,
		ContentBrowser_FileAnimation,
		ContentBrowser_FileAudio,
		ContentBrowser_FileScript,
		ContentBrowser_Plus,
		ContentBrowser_Arrow,
		Component_Transform,
		Component_Camera,
		Component_Script,
		Component_SpriteRenderer,
		Component_SubTextureRenderer,
		Component_CircleRenderer,
		Component_RigidBody2D,
		Component_BoxCollider2D,
		Component_CircleCollider2D,
		Component_TextRenderer,
		Component_Animator,
		Component_Puzzle,
		Component_Audio,
		AddResource_Animation,
		AddResource_Shader,
		AddResource_Font,
		AddResource_Text,
		AddResource_Image,
		AddResource_Script,
		AddResource_NativeScript,
		AddResource_Custom,
		AnimationEditor_LengthChange,
		AnimationEditor_LengthChangeSelected,
		AnimationEditor_MoveIcon,
		AnimationEditor_MoveIconSelected,
		Assistant_Settings,
		Assistant_CopyCode,
		Assistant_Microphone,
		Assistant_MicrophoneOff,
		Assistant_MicrophoneUnavailable,
		Assistant_Read,
		Assistant_Send,
		MAX
	};

	class IconLoader
	{
	public:
		static void LoadIcons();
		static Ref<Texture2D> GetIcon(Icons icon);

	private:
		static std::filesystem::path GetIconPath(Icons icon);

	private:
		static std::unordered_map<Icons, Ref<Texture2D>> m_Icons;
		static Ref<Texture2D> m_DefaultIcon;
		static std::filesystem::path m_DefaultIconPath;
	};
}
