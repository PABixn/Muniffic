#pragma once
#include <string>
#include <Engine/Assistant/AssistantManager.h>
#include <Imgui/imgui.h>
#include <Engine/Renderer/Texture.h>

namespace eg
{
	class AssistantPanel
	{
	public:
		AssistantPanel();
		~AssistantPanel();
		void OnImGuiRender();
		void RunMessage();
		void RenderUserMessage(const std::string& message);
		void RenderAssistantMessage(const std::string& message, int id = 0);
		std::string GetLanguageSymbol(std::string language);

	private:
		static char buffer[1024];
		std::string threadID;
		AssistantManager* assistantManager;
		std::string assistantRespondingAnimation;
		Ref<Texture2D> m_IconCopy;
		Ref<Texture2D> m_IconSend;
	};
}
