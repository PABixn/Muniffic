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
		Editor_ResourceNotFound,
		WelcomePanel_NewProject,
		WelcomePanel_DeleteProject,
		ContentBrowser_Directory,
		ContentBrowser_File,
		Component_Icon,
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
		Component_Puzzle
	};

	class IconLoader
	{
	public:
		static void LoadIcons();

	private:
		static std::filesystem::path GetIconPath(Icons icon);

	private:
		static std::unordered_map<Icons, Ref<Texture2D>> m_Icons;
	};
}
