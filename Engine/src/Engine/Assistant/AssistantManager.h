#pragma once
#include <string>
#include <vector>
#include "python/include/Python.h"

namespace eg
{
	class AssistantManager
	{
	public:
		struct Message
		{
			std::string role;
			std::string content;
			int id;
		};

		struct Thread
		{
			std::vector<Message*> messages;
			std::vector<std::string> m_RunIDs;
		};

		AssistantManager();
		~AssistantManager();

		bool LoadAssistant();
		void SaveAssistant();

		void Init();
		void StartListening();
		void StopListening();
		bool CheckMicrophoneAvailable();
		void CreateAssistant(std::string assistantName, std::string assistantInstructions);
		std::string CreateThread();
		void InitiateRun(std::string threadID);
		void AddMessage(std::string threadID, std::string message);
		bool WaitForCompletion(std::string threadID);
		bool CheckIfAssistantExists(std::string assistantID);
		bool TakeAction(std::string threadID, std::string runID, PyGILState_STATE gstate);

		void SpeakText(std::string text);
		void SetVolume(float volume);

		std::vector<Message*> SetAssistantID(std::string assistantID) { m_AssistantID = assistantID; }
		std::string GetAssistantID() { return m_AssistantID; }
		std::vector<Message*> GetMessages(std::string threadID) { return m_Threads.at(threadID)->messages; }

		void SetVoiceAssistantListening(bool listening) { IsVoiceAssistantListening = listening; }
		bool GetVoiceAssistantListening() { return IsVoiceAssistantListening; }

		bool IsNewVoiceMessageAvailable() { return newVoiceMessageAvailable; }
		std::string GetLastVoiceMessage() { newVoiceMessageAvailable = false; return lastVoiceMessage; }

		bool GetIsMessageInProgress() { return IsMessageInProgress; }
		void SetMessageInProgress(bool available) { IsMessageInProgress = available; }

		void SetShouldReadAloud(bool readAloud) { ShouldReadAloud = readAloud; }
		bool GetShouldReadAloud() { return ShouldReadAloud; }

		const std::string& GetLastMessageRole(std::string threadID);

		bool ThreadAvailable(std::string threadID);

	private:
		std::string m_AssistantName;
		std::string m_AssistantInstructions;
		std::string m_AssistantID;
		std::unordered_map<std::string, Thread*> m_Threads;
		bool newVoiceMessageAvailable;
		std::string lastVoiceMessage;
		bool IsVoiceAssistantListening;
		bool IsVoiceAsssistantInitialized;
		bool IsMessageInProgress;
		bool ShouldReadAloud;

	private:
		PyObject* m_voiceAssistantModule;
		PyObject* m_startVoiceAssistant;
		PyObject* m_stopVoiceAssistant;
		PyObject* m_SpeakText;
		PyObject* m_pModule;
		PyObject* m_CreateAssistant;
		PyObject* m_CreateThread;
		PyObject* m_AddMessage;
		PyObject* m_InitiateRun;
		PyObject* m_WaitForCompletion;
		PyObject* m_GetLastMessage;
		PyObject* m_CheckIfAssistantExists;
		PyObject* m_GetToolCall;
		PyObject* m_SubmitToolOutputs;
		PyObject* m_InitVoiceAssistant;
		PyObject* m_CheckMicrophoneAvailable;
		PyObject* m_SetVolume;
	};
}

