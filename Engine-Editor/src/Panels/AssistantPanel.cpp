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
		assistantRespondingAnimation = ".";
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

	void AssistantPanel::RenderUserMessage(const std::string& message) {
		float padding = 10.0f; 
		float rightMargin = 15.0f;
		float maxBubbleWidth = ImGui::GetWindowSize().x * 0.6f;

		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + maxBubbleWidth - padding * 2);
		ImVec2 textSize = ImGui::CalcTextSize(message.c_str(), nullptr, false, maxBubbleWidth - padding * 2);
		ImGui::PopTextWrapPos();

		float bubbleWidth = textSize.x + padding * 2;
		float bubbleHeight = textSize.y + padding * 2;

		float windowWidth = ImGui::GetWindowSize().x;

		ImGui::SetCursorPosX(windowWidth - bubbleWidth - rightMargin);
		ImGui::BeginGroup();

		ImGui::GetWindowDrawList()->AddRectFilled(
			ImGui::GetCursorScreenPos(),
			ImVec2(ImGui::GetCursorScreenPos().x + bubbleWidth, ImGui::GetCursorScreenPos().y + bubbleHeight),
			IM_COL32(64, 54, 89, 255), 10.0f
		);

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);

		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + bubbleWidth - padding);
		ImGui::TextUnformatted(message.c_str());
		ImGui::PopTextWrapPos();

		ImGui::EndGroup();
	}

	void AssistantPanel::RenderAssistantMessage(const std::string& message) {
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->ChannelsSplit(2);
		drawList->ChannelsSetCurrent(1);

		float padding = 10.0f;
		float maxBubbleWidth = ImGui::GetWindowSize().x * 0.6f;

		std::stringstream stream(message);
		std::string line, language, msg = "";
		bool insideCode = false;
		float initialCursorPosX = ImGui::GetCursorPosX();

		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + maxBubbleWidth - padding * 2);
		ImVec2 textSize = ImGui::CalcTextSize(message.c_str(), nullptr, false, maxBubbleWidth - padding * 2);
		ImGui::PopTextWrapPos();

		float bubbleWidth = textSize.x + padding * 2;
		float bubbleHeight = textSize.y + padding * 2;

		ImGui::BeginGroup();

		ImVec2 initialCursorPos = ImGui::GetCursorScreenPos();

		ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + bubbleWidth - padding);

		//ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);

		while (std::getline(stream, line))
		{
			if (line.starts_with("```"))
			{
				if (insideCode)
				{
					ImVec2 codeSize = ImGui::CalcTextSize(msg.c_str(), nullptr, false, bubbleWidth - padding * 4);
					float codeHeight = codeSize.y + padding * 2;
					float codeWidth = codeSize.x + padding * 2;

					ImGui::GetWindowDrawList()->AddRectFilled(
						ImVec2(ImGui::GetCursorScreenPos().x + padding, ImGui::GetCursorScreenPos().y + padding * 2),
						ImVec2(ImGui::GetCursorScreenPos().x + codeWidth + padding, ImGui::GetCursorScreenPos().y + codeHeight + padding * 2),
						IM_COL32(18, 14, 28, 255), 10.0f
					);
					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + bubbleWidth - padding * 2);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding * 2);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding * 3);
					ImGui::Text(msg.c_str());
					ImGui::PopTextWrapPos();
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding * 2);
					msg = "";

					insideCode = false;
				}
				else
				{
					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + bubbleWidth - padding);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
					ImGui::Text(msg.c_str());
					ImGui::PopTextWrapPos();
					msg = "";

					insideCode = true;
					language = line.substr(3);
				}
			}
			else
			{
				if(!msg.empty())
					msg += "\n";
				msg += line;
			}
		}

		ImGui::PopTextWrapPos();

		if (!msg.empty())
		{
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + bubbleWidth - padding);
			ImGui::SetCursorPosX(initialCursorPosX + padding);
			ImGui::Text(msg.c_str());
			ImGui::PopTextWrapPos();
		}

		//ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + bubbleWidth - padding);
		//ImGui::Text(message.c_str());
		//ImGui::PopTextWrapPos();

		drawList->ChannelsSetCurrent(0);

		ImGui::GetWindowDrawList()->AddRectFilled(
			initialCursorPos,
			ImVec2(ImGui::GetCursorScreenPos().x + bubbleWidth, ImGui::GetCursorScreenPos().y + padding),
			IM_COL32(64, 54, 89, 255), 10.0f
		);

		drawList->ChannelsMerge();

		ImGui::EndGroup();

		ImGui::NewLine();
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
				RenderAssistantMessage(msg->content);
			else
				RenderUserMessage(msg->content);
			lastMessageRole = msg->role;
			ImGui::NewLine();
		}

		if (lastMessageRole == "user")
		{
			static float timer = 0.0f;
			timer += ImGui::GetIO().DeltaTime;

			if (timer > 0.5f)
			{
				if (assistantRespondingAnimation.size() < 4)
					assistantRespondingAnimation += ".";
				else
					assistantRespondingAnimation = ".";

				timer = 0.0f;
			}

			RenderAssistantMessage(assistantRespondingAnimation);
			ImGui::NewLine();
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