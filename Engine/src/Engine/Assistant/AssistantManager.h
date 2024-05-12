#pragma once
#include <string>
#include <vector>
#include "python/include/Python.h"

namespace eg
{
	struct Thread
	{
		std::string threadID;
		std::vector<std::string> messages;
		std::vector<std::string> m_RunIDs;
	};

	class AssistantManager
	{
	public:
		AssistantManager();
		~AssistantManager();

		void Init();
		std::string CreateAssistant(std::string assistantName, std::string assistantInstructions);
		std::string CreateThread();
		std::string InitiateRun(std::string assistantID, std::string threadID);

	private:
		std::string m_AssistantName;
		std::string m_AssistantInstructions;
		std::string m_AssistantID;
		std::vector<Thread*> m_Threads;
	};
}

