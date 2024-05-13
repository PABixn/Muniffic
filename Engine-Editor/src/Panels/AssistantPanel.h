#pragma once
#include <string>
#include <Engine/Assistant/AssistantManager.h>

namespace eg
{
	class AssistantPanel
	{
	public:
		AssistantPanel();
		void OnImGuiRender();

	private:
		std::string threadID;
		AssistantManager* assistantManager;
	};
}
