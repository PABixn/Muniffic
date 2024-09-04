#pragma once
#include <string>
#include <Engine/Assistant/AssistantManager.h>
#include <Imgui/imgui.h>

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
		void RenderAssistantMessage(const std::string& message);

	private:
		static char buffer[1024];
		std::string threadID;
		AssistantManager* assistantManager;
		std::string assistantRespondingAnimation;
	};
}
