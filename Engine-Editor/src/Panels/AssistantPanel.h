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
		void DrawTextWithBackground(Message* msg, const ImVec4& bg_color);

	private:
		static char buffer[1024];
		std::string threadID;
		AssistantManager* assistantManager;
	};
}
