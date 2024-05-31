#include "egpch.h"
#include "AssistantPanel.h"
#include <Imgui/imgui.h>
#include "Engine/Project/Project.h"
#include "Imgui/imgui_markdown.h"

namespace eg
{
	char AssistantPanel::buffer[1024];
	static ImGui::MarkdownConfig mdConfig;

	AssistantPanel::AssistantPanel()
	{
		assistantManager = new AssistantManager();

		std::string instructions = "You are an assistant for Muniffic game engine user. You can answer questions about the engine, explain mechanics (how everything works) and also control the engine by calling functions. You have C# scripting engine documentation which you can use for writing and explaining C# scripts to user. There is no more documentation, there are no functions apart from these in the documentation.";

		if (!assistantManager->LoadAssistant())
			assistantManager->CreateAssistant("Bob", instructions);

		threadID = assistantManager->CreateThread();
	}

	AssistantPanel::~AssistantPanel()
	{
		delete assistantManager;
		assistantManager = nullptr;
	}

	void DoMessage(AssistantManager* assistantManager, std::string threadID, char* buffer)
	{
		std::string message = buffer;
		memset(buffer, 0, 1024);
		assistantManager->AddMessage(threadID, message);
		assistantManager->InitiateRun(threadID);
		assistantManager->WaitForCompletion(threadID);
	}

	void AssistantPanel::OnImGuiRender()
	{
		ImGui::Begin("Assistant Panel");

		ImGui::Text("This is your conversation with Bob.");
		ImGui::Text("Thread ID : % s", threadID.c_str());

		ImGui::Separator();

		std::string lastMessageRole = "";

		for (Message* msg : assistantManager->GetMessages(threadID))
		{
			std::string role = msg->role + ": ";

			if (msg->role == "assistant")
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.5f, 1.0f));
			else
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.0f, 1.0f));

			ImGui::Button(role.c_str());
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::Markdown(msg->content.c_str(), msg->content.length(), mdConfig);

			lastMessageRole = msg->role;
		}

		if (lastMessageRole == "user")
		{
			std::string msg = "Responding...";
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.5f, 1.0f));
			ImGui::Button("assistant: ");
			ImGui::PopStyleColor();
			ImGui::SameLine();
			ImGui::Markdown(msg.c_str(), msg.length(), mdConfig);

		}

		ImGui::Separator();

		ImGui::Text("Message:");
		ImGui::SameLine();
		ImGui::InputTextMultiline("##", buffer, 1024);

		if (ImGui::Button("Send"))
		{
			std::thread t(DoMessage, assistantManager, threadID, buffer);
			t.detach();
			//RunMessage();
		}

		ImGui::End();
	}

	void AssistantPanel::RunMessage()
	{
		assistantManager->AddMessage(threadID, buffer);
		assistantManager->InitiateRun(threadID);
		memset(buffer, 0, 1024);
		assistantManager->WaitForCompletion(threadID);
	}
}