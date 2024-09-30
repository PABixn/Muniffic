#define PY_SSIZE_T_CLEAN

#include "egpch.h"
#include "AssistantManager.h"
#include <Engine/Project/Project.h>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include "Engine/Utils/YAMLConversion.h"
#include "../../../../Engine-Editor/src/EditorActions.h"

namespace eg
{
	AssistantManager::AssistantManager()
	{
		IsVoiceAssistantListening = false;
		newVoiceMessageAvailable = false;
		IsVoiceAsssistantInitialized = false;
		IsMessageInProgress = false;
		ShouldReadAloud = true;
		m_Threads = std::unordered_map<std::string, Thread*>();

		Init();
	}

	bool AssistantManager::ThreadAvailable(std::string threadID)
	{
		if (m_Threads.find(threadID) == m_Threads.end())
		{
			EG_CORE_ERROR("Thread not found");
			return false;
		}

		if (m_Threads.at(threadID)->messages.size() < 1)
		{
			//EG_CORE_ERROR("No messages found for thread");
			return false;
		}

		return true;
	}

	const std::string& AssistantManager::GetLastMessageRole(std::string threadID)
	{
		if (m_Threads.find(threadID) == m_Threads.end())
		{
			EG_CORE_ERROR("Thread not found");
			return "";
		}

		if (m_Threads.at(threadID)->messages.size() == 0)
		{
			EG_CORE_ERROR("No messages found for thread");
			return "";
		}

		return m_Threads.at(threadID)->messages.at(m_Threads.at(threadID)->messages.size() - 1)->role;
	}

	void AssistantManager::SpeakText(std::string text)
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		if (!IsVoiceAsssistantInitialized)
		{
			PyObject_CallObject(m_InitVoiceAssistant, nullptr);
			IsVoiceAsssistantInitialized = true;
		}

		PyObject* args = PyTuple_Pack(1, PyUnicode_FromString(text.c_str()));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
		}

		PyObject_CallObject(m_SpeakText, args);

		PyGILState_Release(gstate);

		IsMessageInProgress = false;
	}

	bool AssistantManager::CheckMicrophoneAvailable()
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* result = PyObject_CallObject(m_CheckMicrophoneAvailable, nullptr);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call checkMicrophoneAvailable function");
		}

		PyGILState_Release(gstate);

		return PyObject_IsTrue(result);
	}

	void AssistantManager::SetVolume(float volume)
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* args = PyTuple_Pack(1, PyFloat_FromDouble(volume));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
		}

		PyObject_CallObject(m_SetVolume, args);

		PyGILState_Release(gstate);
	}

	void AssistantManager::StopListening()
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject_CallObject(m_stopVoiceAssistant, nullptr);

		PyGILState_Release(gstate);
	}

	void AssistantManager::StartListening()
	{
		IsVoiceAssistantListening = true;

		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* result = PyObject_CallObject(m_startVoiceAssistant, nullptr);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call startVoiceAssistant function");
		}

		if (!PyUnicode_Check(result))
		{
			PyGILState_Release(gstate);
			IsVoiceAssistantListening = false;
			EG_CORE_ERROR("Failed to get voice message");
			
			return;
		}

		std::string text = PyUnicode_AsUTF8(result);

		PyGILState_Release(gstate);

		lastVoiceMessage = text;
		newVoiceMessageAvailable = true;
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

		std::filesystem::path modulePath = "resources/assistant";

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

		m_pModule = PyImport_ImportModule("assistant");

		if (m_pModule == nullptr)
		{
			EG_CORE_ERROR("Failed to load Python module");
		}

		m_voiceAssistantModule = PyImport_ImportModule("voiceAssistant");

		if (m_voiceAssistantModule == nullptr)
		{
			EG_CORE_ERROR("Failed to load Python module");
		}

		m_startVoiceAssistant = PyObject_GetAttrString(m_voiceAssistantModule, "StartVoiceAssistant");
		m_stopVoiceAssistant = PyObject_GetAttrString(m_voiceAssistantModule, "StopVoiceAssistant");
		m_SpeakText = PyObject_GetAttrString(m_voiceAssistantModule, "SpeakText");
		m_InitVoiceAssistant = PyObject_GetAttrString(m_voiceAssistantModule, "InitVoiceAssistant");
		m_CheckMicrophoneAvailable = PyObject_GetAttrString(m_voiceAssistantModule, "CheckMicrophoneAvailable");
		m_SetVolume = PyObject_GetAttrString(m_voiceAssistantModule, "SetVolume");

		if (m_startVoiceAssistant == nullptr && m_SpeakText == nullptr && m_stopVoiceAssistant == nullptr && m_InitVoiceAssistant == nullptr && m_CheckMicrophoneAvailable == nullptr && m_SetVolume == nullptr)
		{
			EG_CORE_ERROR("Failed to load Python functions");
		}

		m_CreateAssistant = PyObject_GetAttrString(m_pModule, "create_assistant");
		m_CreateThread = PyObject_GetAttrString(m_pModule, "create_thread");
		m_AddMessage = PyObject_GetAttrString(m_pModule, "add_message");
		m_InitiateRun = PyObject_GetAttrString(m_pModule, "initiate_run");
		m_WaitForCompletion = PyObject_GetAttrString(m_pModule, "wait_for_completion");
		m_GetLastMessage = PyObject_GetAttrString(m_pModule, "get_last_message");
		m_CheckIfAssistantExists = PyObject_GetAttrString(m_pModule, "check_if_assistant_exists");
		m_GetToolCall = PyObject_GetAttrString(m_pModule, "get_tool_call");
		m_SubmitToolOutputs = PyObject_GetAttrString(m_pModule, "submit_tool_outputs");
		m_CheckAPI = PyObject_GetAttrString(m_pModule, "check_api_key");

		if (m_CreateAssistant == nullptr || m_CreateThread == nullptr || m_InitiateRun == nullptr || m_WaitForCompletion == nullptr || m_GetLastMessage == nullptr || m_CheckIfAssistantExists == nullptr || m_GetLastMessage == nullptr || m_SubmitToolOutputs == nullptr || m_CheckAPI == nullptr)
		{
			EG_CORE_ERROR("Failed to load Python functions");
		}

		PyEval_SaveThread();
	}

	bool AssistantManager::CheckAPI()
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* result = PyObject_CallObject(m_CheckAPI, nullptr);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call check_api_key function");
			return false;
		}

		PyGILState_Release(gstate);

		return PyObject_IsTrue(result);
	}

	bool AssistantManager::CreateAssistant(std::string assistantName, std::string assistantInstructions = "")
	{
		PyGILState_STATE gstate = PyGILState_Ensure();

		PyObject* args = PyTuple_Pack(3, PyUnicode_FromString(assistantName.c_str()), PyUnicode_FromString("gpt-4o-mini"), PyUnicode_FromString(assistantInstructions.c_str()));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
			return false;
		}

		PyObject* result = PyObject_CallObject(m_CreateAssistant, args);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call create_assistant function");
			return false;
		}
			
		EG_CORE_INFO("Assistant created with ID: {0}", PyUnicode_AsUTF8(result));

		m_AssistantID = PyUnicode_AsUTF8(result);

		PyGILState_Release(gstate);

		SaveAssistant();

		return true;
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
			messageObj->id = m_Threads.at(threadID)->messages.size();

			if (ShouldReadAloud)
			{
				std::thread t([this, messageObj] { SpeakText(messageObj->content); });
				t.detach();
			}
			else
				IsMessageInProgress = false;
			
			m_Threads.at(threadID)->messages.push_back(messageObj);

			return true;
		}
		else if (status == "requires_action")
		{
			TakeAction(threadID, runID, gstate);
		}
		else if (status == "failed" || status == "cancelled" || status == "expired")
		{
			EG_CORE_ERROR("Run " + status);
			PyGILState_Release(gstate);
			
			return false;
		}

		PyGILState_Release(gstate);
		
		return false;
	}

	bool AssistantManager::TakeAction(std::string threadID, std::string runID, PyGILState_STATE gstate)
	{
		PyObject* args = PyTuple_Pack(2, PyUnicode_FromString(threadID.c_str()), PyUnicode_FromString(runID.c_str()));

		if (args == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to create args object");
			PyGILState_Release(gstate);
			
			return false;
		}

		PyObject* result = PyObject_CallObject(m_GetToolCall, args);

		if (result == nullptr)
		{
			PyErr_Print();
			EG_CORE_ERROR("Failed to call get_tool_call function");
			PyGILState_Release(gstate);
			
			return false;
		}

		if (PyList_Check(result))
		{
			std::vector<std::unordered_map<std::string, std::string>> toolCalls;

			std::vector<PyObject*> list_result;

			Py_ssize_t size = PyList_Size(result);
			for (Py_ssize_t i = 0; i < size; ++i)
			{
				PyObject* item = PyList_GetItem(result, i);
				list_result.push_back(item);
			}

			for (PyObject* item : list_result)
			{
				if (item && PyDict_Check(item))
				{
					std::unordered_map<std::string, std::string> dict_result;
					PyObject* key;
					PyObject* value;
					Py_ssize_t pos = 0;

					while (PyDict_Next(item, &pos, &key, &value))
					{
						if (PyUnicode_Check(key))
						{
							std::string cpp_key = PyUnicode_AsUTF8(key);
							if (PyUnicode_Check(value))
							{
								dict_result[cpp_key] = PyUnicode_AsUTF8(value);
							}
							else if (PyLong_Check(value))
							{
								dict_result[cpp_key] = std::to_string(PyLong_AsLong(value));
							}
							else if (PyFloat_Check(value))
							{
								dict_result[cpp_key] = std::to_string(PyFloat_AsDouble(value));
							}
						}
					}

					toolCalls.push_back(dict_result);
				}
			}

			std::string output = "{";
			int i = 0;

			for (std::unordered_map<std::string, std::string> toolCall : toolCalls)
			{
				std::string toolCallID = toolCall["tool_call_id"];
				std::string functionName = toolCall["function_name"];
				std::vector<std::string> params;

				std::vector<std::string> arg_keys;
				for (const auto& [key, value] : toolCall)
				{
					if (key.substr(0, 3) == "arg")
					{
						arg_keys.push_back(key);
					}
				}

				std::sort(arg_keys.begin(), arg_keys.end());

				for (const auto& key : arg_keys)
				{
					params.push_back(toolCall[key]);
				}

				std::string callToolOutput = EditorActions::ExecuteEntityAction(functionName, params);

				output += "\"" + toolCallID + "\": \"" + callToolOutput + "\"";

				if (i != toolCalls.size() - 1)
					output += ",";

				i++;
			}

			output += "}";

			EG_CORE_INFO(output);

			if (toolCalls.size() > 0)
			{
				args = PyTuple_Pack(3, PyUnicode_FromString(threadID.c_str()), PyUnicode_FromString(runID.c_str()), PyUnicode_FromString(output.c_str()));

				if (args == nullptr)
				{
					PyErr_Print();
					EG_CORE_ERROR("Failed to create args object");
					PyGILState_Release(gstate);
					
					return false;
				}

				result = PyObject_CallObject(m_SubmitToolOutputs, args);

				if (result == nullptr)
				{
					PyErr_Print();
					EG_CORE_ERROR("Failed to call submit_tool_outputs function");
					PyGILState_Release(gstate);
					
					return false;
				}
			}

			WaitForCompletion(threadID);
		}
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
		std::filesystem::path path = "resources/assistant/assistant.mndata";

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
		{
			
			return false;
		}

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