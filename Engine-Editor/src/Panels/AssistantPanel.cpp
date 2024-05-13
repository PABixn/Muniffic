#include "egpch.h"
#include "AssistantPanel.h"
#include <Imgui/imgui.h>

namespace eg
{
	AssistantPanel::AssistantPanel()
	{
		assistantManager = new AssistantManager();
		assistantManager->CreateAssistant("Bob", "You are a helpful assistant.");
		threadID = assistantManager->CreateThread();
	}

	void AssistantPanel::OnImGuiRender()
	{
		ImGui::Begin("Assistant Panel");

		ImGui::Text("This is your conversation with Bob.");
		ImGui::Text("Thread ID : % s", threadID.c_str());

		ImGui::Separator();

		for (Message* msg : assistantManager->GetMessages(threadID))
		{
			std::string content = msg->role + ": " + msg->content;
			ImGui::Text(content.c_str());
		}

		ImGui::Separator();

		static char buffer[1024];

		ImGui::InputText("Message", buffer, 1024);

		if (ImGui::Button("Send"))
		{
			assistantManager->AddMessage(threadID, buffer);
			assistantManager->InitiateRun(threadID);
			memset(buffer, 0, 1024);
			assistantManager->WaitForCompletion(threadID);
		}

		ImGui::End();
	}
}