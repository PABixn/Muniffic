#include "egpch.h"
#include "AnimationPanel.h"

namespace eg {
	AnimationPanel::AnimationPanel(const std::filesystem::path& path)
	{
		InitAnimationPanel(path);
	}

	bool AnimationPanel::InitAnimationPanel(const std::filesystem::path& path)
	{
		std::filesystem::path textureBasePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / "Textures";
		bool resourceLoad = false;
		m_LoadedResource = new Resource();
		resourceLoad = resourceSystemLoad(path.string(), ResourceType::Image, m_LoadedResource);

		if (!resourceLoad) {
			return false;
		}
		m_OriginalResourcePath = path;
		m_ResourceData = new TextureResourceData();
		((TextureResourceData*)m_ResourceData)->ResourcePath = "Textures";
		((TextureResourceData*)m_ResourceData)->ImageName = m_TextureData.ResourcePath.stem().string();
		((TextureResourceData*)m_ResourceData)->Extension = m_TextureData.ResourcePath.extension().string();
		((TextureResourceData*)m_ResourceData)->Height = ((ImageResourceData*)m_LoadedResource->Data)->height;
		((TextureResourceData*)m_ResourceData)->Width = ((ImageResourceData*)m_LoadedResource->Data)->width;
		((TextureResourceData*)m_ResourceData)->Channels = ((ImageResourceData*)m_LoadedResource->Data)->channelCount;
		m_PreviewData = Texture2D::Create(path.string());

	}

	void AnimationPanel::OnImGuiRender()
	{
	}

	void AnimationPanel::ResetData()
	{
	}
}