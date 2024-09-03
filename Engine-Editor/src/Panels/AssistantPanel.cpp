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
			std::string instructions = "You are an assistant for Muniffic Game Engine. You can answer questions about the engine, explain mechanics (how everything works) and also write scripts using utilities provided by Muniffic Script Engine. You can also call functions when it is needed to satisfy the user. When user asks to do something, firstly check if you can do it yourself using provieded function, if not, continue with other tools (remember that TransformComponent is default, it can't be added or removed). Before answering any question about the engine (specifically scripts) check the documentation. \n You have needed documentation in following files (don't write about these files to the user): \n - Muniffic.docx - general documentation of the engine, explaining how the whole engine works. \n - Scripting-Engine.cs - scripting engine code containing all functions that can be used for scripting. \n - Scripting-Engine-Examples.cs - general rules for writing scripts and examples of how to perform specific actions(how to use scripting system functions). \n You answer questions only about Muniffic, don't write anything about other engines. \n If you don't know something or are unsure, make an elegant excuse. \n If user asks something about scripts, write an example full script file performing requested action.";

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

	void RenderYourMessage(const std::string& message) {
		float padding = 10.0f; // Padding around text inside the bubble
		float rightMargin = 15.0f; // Margin from the right side of the window
		float maxBubbleWidth = ImGui::GetWindowSize().x * 0.6f; // 60% of the window width

		// Set wrap position for text
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + maxBubbleWidth - padding * 2);

		// Calculate text size with wrapping enabled
		ImVec2 textSize = ImGui::CalcTextSize(message.c_str(), nullptr, false, maxBubbleWidth - padding * 2);
		ImGui::PopTextWrapPos(); // Reset wrap position

		float bubbleWidth = textSize.x + padding * 2; // Adjust bubble width if text is smaller than max width
		float bubbleHeight = textSize.y + padding * 2; // Adjust bubble height for wrapped text

		// Get the current window width
		float windowWidth = ImGui::GetWindowSize().x;

		// Calculate the start position for right-aligned text with a margin from the right
		ImGui::SetCursorPosX(windowWidth - bubbleWidth - rightMargin);
		ImGui::BeginGroup(); // Start a group to align text and rectangle together

		// Draw the bubble background
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetCursorScreenPos(),
			ImVec2(ImGui::GetCursorScreenPos().x + bubbleWidth, ImGui::GetCursorScreenPos().y + bubbleHeight),
			IM_COL32(64, 54, 89, 255), 5.0f // Background color and corner rounding
		);

		// Add some padding before the text
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);

		// Render the text with wrapping
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + bubbleWidth - padding);
		ImGui::TextUnformatted(message.c_str());
		ImGui::PopTextWrapPos();

		ImGui::EndGroup(); // End the group
	}

	// Function to render other's message (left-aligned)
	void RenderOtherMessage(const std::string& message) {
		float padding = 10.0f; // Padding around text inside the bubble
		float maxBubbleWidth = ImGui::GetWindowSize().x * 0.6f; // 60% of the window width

		// Set wrap position for text
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + maxBubbleWidth - padding * 2);

		// Calculate text size with wrapping enabled
		ImVec2 textSize = ImGui::CalcTextSize(message.c_str(), nullptr, false, maxBubbleWidth - padding * 2);
		ImGui::PopTextWrapPos(); // Reset wrap position

		float bubbleWidth = textSize.x + padding * 2; // Adjust bubble width if text is smaller than max width
		float bubbleHeight = textSize.y + padding * 2; // Adjust bubble height for wrapped text

		ImGui::BeginGroup(); // Start a group to align text and rectangle together

		// Draw the bubble background
		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetCursorScreenPos(),
			ImVec2(ImGui::GetCursorScreenPos().x + bubbleWidth, ImGui::GetCursorScreenPos().y + bubbleHeight),
			IM_COL32(64, 54, 89, 255), 5.0f // Background color and corner rounding
		);

		// Add some padding before the text
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);

		// Render the text with wrapping
		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + bubbleWidth - padding);
		ImGui::TextUnformatted(message.c_str());
		ImGui::PopTextWrapPos();

		ImGui::EndGroup(); // End the group
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
			if(msg->role == "assistant")
				RenderOtherMessage(msg->content);
			else
				RenderYourMessage(msg->content);
			lastMessageRole = msg->role;
			ImGui::NewLine();
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