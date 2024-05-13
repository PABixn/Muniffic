#pragma once
#include <string>
#include <vector>
#include "python/include/Python.h"

namespace eg
{
	struct Message
	{
		std::string role;
		std::string content;
	};

	struct Thread
	{
		std::vector<Message*> messages;
		std::vector<std::string> m_RunIDs;
	};

	class AssistantManager
	{
	public:
		AssistantManager();
		~AssistantManager();

		void Init();
		void CreateAssistant(std::string assistantName, std::string assistantInstructions);
		std::string CreateThread();
		void InitiateRun(std::string threadID);
		void AddMessage(std::string threadID, std::string message);
		std::string WaitForCompletion(std::string threadID);

		std::vector<Message*> GetMessages(std::string threadID) { return m_Threads.at(threadID)->messages; }

	private:
		std::string m_AssistantName;
		std::string m_AssistantInstructions;
		std::string m_AssistantID;
		std::unordered_map<std::string, Thread*> m_Threads;

	private:
		PyObject* m_pModule;
		PyObject* m_CreateAssistant;
		PyObject* m_CreateThread;
		PyObject* m_AddMessage;
		PyObject* m_InitiateRun;
		PyObject* m_WaitForCompletion;
		PyObject* m_GetLastMessage;
	};
}

