#include "egpch.h"
#include "AssistantPanel.h"
#include <Imgui/imgui.h>
#include "Engine/Project/Project.h"
#include "Imgui/imgui_markdown.h"

namespace eg
{
	char AssistantPanel::buffer[1024];

	AssistantPanel::AssistantPanel()
	{
		initiateTime = time.now();

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

	void AssistantPanel::OnImGuiRender()
	{
		if(pendingMessage && std::chrono::duration_cast<std::chrono::milliseconds>(time.now() - initiateTime).count() % 100 == 0)
			if(assistantManager->TryGetMessage(threadID))
				pendingMessage = false;

		ImGui::Begin("Assistant Panel");

		static ImGui::MarkdownConfig mdConfig;

		ImGui::Text("This is your conversation with Bob.");
		ImGui::Text("Thread ID : % s", threadID.c_str());

		ImGui::Separator();

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
		}

		ImGui::Separator();

		ImGui::Text("Message:");
		ImGui::SameLine();
		ImGui::InputTextMultiline("##", buffer, 1024);

		if (ImGui::Button("Send"))
		{
			RunMessage();
		}

		ImGui::End();
	}

	void AssistantPanel::RunMessage()
	{
		pendingMessage = true;
		assistantManager->AddMessage(threadID, buffer);
		assistantManager->InitiateRun(threadID);
		memset(buffer, 0, 1024);
		if(assistantManager->TryGetMessage(threadID))
			pendingMessage = false;
	}
}