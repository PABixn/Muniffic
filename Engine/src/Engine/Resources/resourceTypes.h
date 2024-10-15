#pragma once
#include <cstdint>
#include <string>
#include <filesystem>
#include "Engine/Project/Project.h"
#include "Engine/Core/UUID.h"
#include "Engine/Scripting/ScriptingTypes.h"

namespace eg
{
	enum class ResourceType
	{
		Scene,
		Animation,
		Frame,
		Audio,
		SpriteAtlas,
		SubTexture,
		Shader,
		Font,
		Text,
		Image,
		Script,
		NativeScript,
		None,
		Custom
	};

	struct Resource
	{
		uint32_t LoaderId;
		ResourceType Type;
		std::string Name;
		std::filesystem::path Path;
		uint64_t DataSize;
		void *Data;
	};

	struct ImageResourceData
	{
		uint8_t channelCount;
		uint32_t width;
		uint32_t height;
		unsigned char *pixels;
	};

	struct ResourceData
	{
		std::string ResourceName = "";
		std::string Extension = "";
		UUID ParentDirectory = 0;
		ResourceType Type;
	};

	struct AudioResourceData : public ResourceData
	{
	};

	struct SpriteAtlasResourceData : public ResourceData
	{
		int Width = 0, Height = 0;
		int Channels = 0;
		std::vector<UUID> Sprites;
	};

	struct AnimationResourceData : public ResourceData
	{
		float FrameRate;
		int FrameCount;
		bool Loop;
		std::vector<UUID> Frames;
	};

	struct FrameResourceData : public ResourceData
	{
		UUID SubTexture;
		int Duration;
		std::string ClassName;
		std::string FunctionCallName;
	};

	struct SubTextureResourceData : public ResourceData
	{
		glm::vec2 TexCoords[4];
		UUID Texture;
	};

	struct MethodResourceData
	{
		std::string ClassName;
		std::string MethodName;
		std::vector<std::pair<std::string, ScriptFieldType>> Parameters;
	};

	struct ScriptResourceData : public ResourceData
	{
		bool IsEnabled = true;
		std::unordered_map<std::string, MethodResourceData> Methods;
	};

	struct FontResourceData : public ResourceData
	{
	};

	struct SceneResourceData : public ResourceData
	{
	};

	struct TextureResourceData : public ResourceData
	{
		int Width = 0, Height = 0;
		int Channels = 0;
	};

}
