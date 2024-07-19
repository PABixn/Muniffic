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

		if (!assistantManager->LoadAssistant())
		{
			std::string instructions = "You are an assistant for Muniffic Game Engine. You can answer questions about the engine, explain mechanics (how everything works) and also write scripts using utilities provided by Muniffic Script Engine. \n You have needed documentation in following files (don't write about these files to the user): \n - Muniffic.docx - general documentation of the engine, explaining how the whole engine works. \n - Scripting-Engine.cs - scripting engine code containing all functions that can be used for scripting. \n - Scripting-Engine-Examples.cs - general rules for writing scripts and examples of how to perform specific actions(how to use scripting system functions). \n You answer questions only about Muniffic, don't write anything about other engines. \n If you don't know something or are unsure, make an elegant excuse. \n If user asks something about scripts, write an example full script file performing requested action.";

			assistantManager->CreateAssistant("Bob", instructions);
		}

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