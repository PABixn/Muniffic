#pragma once
#include <string>
#include <Engine/Assistant/AssistantManager.h>

namespace eg
{
	class AssistantPanel
	{
	public:
		AssistantPanel();
		~AssistantPanel();
		void OnImGuiRender();
		void RunMessage();

	private:
		static char buffer[1024];
		std::string threadID;
		AssistantManager* assistantManager;
	};
}
