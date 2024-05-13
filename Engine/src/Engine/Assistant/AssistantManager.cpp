#define PY_SSIZE_T_CLEAN

#include "egpch.h"
#include "AssistantManager.h"
#include <Engine/Project/Project.h>

namespace eg
{
	AssistantManager::AssistantManager()
	{
		Init();
	}

	void AssistantManager::Init()
	{
		Py_Initialize();

		PyObject* sysPath = PySys_GetObject("path");

		if (sysPath == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to get sysPath object");
		}

		std::filesystem::path modulePath = Project::GetResourcesPath() / "resources/python";

		PyObject* path = PyUnicode_FromString(modulePath.string().c_str());

		if (path == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create path object");
		}

		int result = PyList_Append(sysPath, path);

		if (result == -1)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to append path to sysPath");
		}

		m_pModule = PyImport_ImportModule("main");

		if (m_pModule == nullptr)
		{
			EG_CORE_ERROR("Failed to load Python module");
		}

		m_CreateAssistant = PyObject_GetAttrString(m_pModule, "create_assistant");
		m_CreateThread = PyObject_GetAttrString(m_pModule, "create_thread");
		m_AddMessage = PyObject_GetAttrString(m_pModule, "add_message");
		m_InitiateRun = PyObject_GetAttrString(m_pModule, "initiate_run");
		m_WaitForCompletion = PyObject_GetAttrString(m_pModule, "wait_for_completion");
		m_GetLastMessage = PyObject_GetAttrString(m_pModule, "get_last_message");

		if (m_CreateAssistant == nullptr || m_CreateThread == nullptr || m_InitiateRun == nullptr || m_WaitForCompletion == nullptr || m_GetLastMessage == nullptr)
		{
			EG_CORE_ERROR("Failed to load Python functions");
		}
	}

	void AssistantManager::CreateAssistant(std::string assistantName, std::string assistantInstructions = "")
	{
		PyObject* args = PyTuple_Pack(3, PyUnicode_FromString(assistantName.c_str()), PyUnicode_FromString("gpt-3.5-turbo"), PyUnicode_FromString(assistantInstructions.c_str()));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
		}

		PyObject* result = PyObject_CallObject(m_CreateAssistant, args);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call create_assistant function");
		}

		EG_CORE_INFO("Assistant created with ID: {0}", PyUnicode_AsUTF8(result));

		m_AssistantID = PyUnicode_AsUTF8(result);
	}

	std::string AssistantManager::CreateThread()
	{
		PyObject* result = PyObject_CallObject(m_CreateThread, nullptr);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call create_thread function");
		}

		std::string threadID = PyUnicode_AsUTF8(result);
		Thread* thread = new Thread();
		m_Threads[threadID] = thread;

		return threadID;
	}

	void AssistantManager::InitiateRun(std::string threadID)
	{
		if (m_Threads.find(threadID) == m_Threads.end())
		{
			EG_CORE_ERROR("Thread not found");
			return;
		}

		PyObject* args = PyTuple_Pack(2, PyUnicode_FromString(m_AssistantID.c_str()), PyUnicode_FromString(threadID.c_str()));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
		}

		PyObject* result = PyObject_CallObject(m_InitiateRun, args);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call initiate_run function");
		}

		m_Threads.at(threadID)->m_RunIDs.push_back(PyUnicode_AsUTF8(result));
	}

	void AssistantManager::AddMessage(std::string threadID, std::string message)
	{
		if (m_Threads.find(threadID) == m_Threads.end())
		{
			EG_CORE_ERROR("Thread not found");
			return;
		}

		PyObject* args = PyTuple_Pack(2, PyUnicode_FromString(threadID.c_str()), PyUnicode_FromString(message.c_str()));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
		}

		PyObject_CallObject(m_AddMessage, args);

		Message* messageObj = new Message();
		messageObj->role = "user";
		messageObj->content = message;

		m_Threads.at(threadID)->messages.push_back(messageObj);
	}

	std::string AssistantManager::WaitForCompletion(std::string threadID)
	{
		if (m_Threads.find(threadID) == m_Threads.end())
		{
			EG_CORE_ERROR("Thread not found");
			return "";
		}

		if (m_Threads.at(threadID)->m_RunIDs.size() == 0)
		{
			EG_CORE_ERROR("No run IDs found for thread");
			return "";
		}

		PyObject* args = PyTuple_Pack(2, PyUnicode_FromString(threadID.c_str()), PyUnicode_FromString(m_Threads.at(threadID)->m_RunIDs.at(0).c_str()));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
		}

		PyObject* result = PyObject_CallObject(m_WaitForCompletion, args);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call wait_for_completion function");
		}

		std::string status = PyUnicode_AsUTF8(result);

		if (status == "completed")
		{
			args = PyTuple_Pack(1, PyUnicode_FromString(threadID.c_str()));

			if (args == nullptr)
			{
				PyErr_Print();
				EG_CORE_ERROR("Failed to create args object");
			}

			result = PyObject_CallObject(m_GetLastMessage, args);

			if (result == nullptr)
			{
				PyErr_Print();
				EG_CORE_ERROR("Failed to call get_last_message function");
			}

			std::string message = PyUnicode_AsUTF8(result);

			Message* messageObj = new Message();
			messageObj->role = "assistant";
			messageObj->content = message;

			m_Threads.at(threadID)->messages.push_back(messageObj);

			return message;
		}
		else if (status == "failed" || status == "cancelled" || status == "expired")
		{
			EG_CORE_ERROR("Run " + status);
			return "";
		}
	}
}