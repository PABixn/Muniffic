#define PY_SSIZE_T_CLEAN

#include "egpch.h"
#include "AssistantManager.h"
#include <Engine/Project/Project.h>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include "Engine/Utils/YAMLConversion.h"

namespace eg
{
	AssistantManager::AssistantManager()
	{
		Init();
	}

	void AssistantManager::Init()
	{
		Py_Initialize();
		PyEval_InitThreads();

		PyObject* sysPath = PySys_GetObject("path");

		if (sysPath == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to get sysPath object");
		}

		std::filesystem::path modulePath = Project::GetResourcesPath() / "resources/assistant";

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
		m_CheckIfAssistantExists = PyObject_GetAttrString(m_pModule, "check_if_assistant_exists");

		if (m_CreateAssistant == nullptr || m_CreateThread == nullptr || m_InitiateRun == nullptr || m_WaitForCompletion == nullptr || m_GetLastMessage == nullptr || m_CheckIfAssistantExists == nullptr)
		{
			EG_CORE_ERROR("Failed to load Python functions");
		}

		PyEval_SaveThread();
	}

	void AssistantManager::CreateAssistant(std::string assistantName, std::string assistantInstructions = "")
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

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

		PyGILState_Release(gstate);

		SaveAssistant();
	}

	std::string AssistantManager::CreateThread()
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* result = PyObject_CallObject(m_CreateThread, nullptr);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call create_thread function");
		}

		std::string threadID = PyUnicode_AsUTF8(result);
		Thread* thread = new Thread();
		m_Threads[threadID] = thread;

		PyGILState_Release(gstate);

		return threadID;
	}

	void AssistantManager::InitiateRun(std::string threadID)
	{
		if (m_Threads.find(threadID) == m_Threads.end())
		{
			EG_CORE_ERROR("Thread not found");
			return;
		}

		// Ensure GIL state is acquired
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* args = PyTuple_Pack(2, PyUnicode_FromString(m_AssistantID.c_str()), PyUnicode_FromString(threadID.c_str()));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
			PyGILState_Release(gstate);
			return;
		}

		PyObject* result = PyObject_CallObject(m_InitiateRun, args);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call initiate_run function");
			PyGILState_Release(gstate);
			return;
		}

		m_Threads.at(threadID)->m_RunIDs.push_back(PyUnicode_AsUTF8(result));

		// Release GIL state
		PyGILState_Release(gstate);
	}


	void AssistantManager::AddMessage(std::string threadID, std::string message)
	{
		if (m_Threads.find(threadID) == m_Threads.end())
		{
			EG_CORE_ERROR("Thread not found");
			return;
		}

		Message* messageObj = new Message();
		messageObj->role = "user";
		messageObj->content = message;

		m_Threads.at(threadID)->messages.push_back(messageObj);

		// Ensure GIL state is acquired
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* args = PyTuple_Pack(2, PyUnicode_FromString(threadID.c_str()), PyUnicode_FromString(message.c_str()));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
			PyGILState_Release(gstate);

			delete messageObj;
			m_Threads.at(threadID)->messages.pop_back();

			return;
		}

		PyObject_CallObject(m_AddMessage, args);

		// Release GIL state
		PyGILState_Release(gstate);
	}


	bool AssistantManager::WaitForCompletion(std::string threadID)
	{
		if (m_Threads.find(threadID) == m_Threads.end())
		{
			EG_CORE_ERROR("Thread not found");
			return false;
		}

		if (m_Threads.at(threadID)->m_RunIDs.size() == 0)
		{
			EG_CORE_ERROR("No run IDs found for thread");
			return false;
		}

		std::string runID = m_Threads.at(threadID)->m_RunIDs.at(m_Threads.at(threadID)->m_RunIDs.size() - 1);

		// Ensure GIL state is acquired
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* args = PyTuple_Pack(2, PyUnicode_FromString(threadID.c_str()), PyUnicode_FromString(runID.c_str()));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
			PyGILState_Release(gstate);
			return false;
		}

		PyObject* result = PyObject_CallObject(m_WaitForCompletion, args);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call wait_for_completion function");
			PyGILState_Release(gstate);
			return false;
		}

		std::string status = PyUnicode_AsUTF8(result);

		if (status == "completed")
		{
			args = PyTuple_Pack(1, PyUnicode_FromString(threadID.c_str()));

			if (args == nullptr)
			{
				PyErr_Print();
				EG_CORE_ERROR("Failed to create args object");
				PyGILState_Release(gstate);
				return false;
			}

			result = PyObject_CallObject(m_GetLastMessage, args);

			if (result == nullptr)
			{
				PyErr_Print();
				EG_CORE_ERROR("Failed to call get_last_message function");
				PyGILState_Release(gstate);
				return false;
			}

			std::string message = PyUnicode_AsUTF8(result);

			PyGILState_Release(gstate);

			Message* messageObj = new Message();
			messageObj->role = "assistant";
			messageObj->content = message;

			m_Threads.at(threadID)->messages.push_back(messageObj);

			return true;
		}
		else if (status == "failed" || status == "cancelled" || status == "expired")
		{
			EG_CORE_ERROR("Run " + status);
			PyGILState_Release(gstate);
			return false;
		}

		PyGILState_Release(gstate);
		return false; // Added to handle cases where no condition is met
	}


	void AssistantManager::SaveAssistant()
	{
		std::filesystem::path path = Project::GetResourcesPath() / "resources/assistant/assistant.mndata";
		YAML::Emitter out;

		if(!std::filesystem::exists(path.parent_path()))
			std::filesystem::create_directories(path.parent_path());

		out << YAML::BeginMap;
		out << YAML::Key << "assistantID" << YAML::Value << m_AssistantID;
		out << YAML::EndMap;

		std::ofstream file(path.string(), std::ios::trunc);
		file << out.c_str();
		file.close();
	}

	bool AssistantManager::LoadAssistant()
	{
		std::filesystem::path path = Project::GetResourcesPath() / "resources/assistant/assistant.mndata";

		if (!std::filesystem::exists(path))
		{
			std::filesystem::create_directories(path.parent_path());
			std::ofstream file(path.string(), std::ios::trunc);
			file.close();
			return false;
		}

		YAML::Node data;

		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			EG_CORE_ERROR("Failed to load assistant data");
			return false;
		}

		if(data["assistantID"])
			m_AssistantID = data["assistantID"].as<std::string>();
		else
			return false;

		if (!CheckIfAssistantExists(m_AssistantID))
		{
			EG_CORE_ERROR("Assistant with ID {0} does not exist", m_AssistantID);
			return false;
		}

		EG_CORE_INFO("Assistant loaded with ID: {0}", m_AssistantID);

		return true;
	}

	bool AssistantManager::CheckIfAssistantExists(std::string assistantID)
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* args = PyTuple_Pack(1, PyUnicode_FromString(assistantID.c_str()));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
			return false;
		}

		PyObject* result = PyObject_CallObject(m_CheckIfAssistantExists, args);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call check_if_assistant_exists function");
			return false;
		}	

		PyGILState_Release(gstate);

		return PyObject_IsTrue(result);
	}

	AssistantManager::~AssistantManager()
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		Py_Finalize();
	}
}