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
		m_IconCopy = Texture2D::Create("resources/icons/copyCode.png");
		m_IconSend = Texture2D::Create("resources/icons/sendIcon.png");
		m_IconMicrophone = Texture2D::Create("resources/icons/micIcon.png");
		m_IconMicrophoneOff = Texture2D::Create("resources/icons/micOffIcon.png");
		m_isListening = false;
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

	std::string AssistantPanel::GetLanguageSymbol(std::string language)
	{
		if(language == "csharp")
			return "C#";
		else if(language == "cpp")
			return "C++";
		else if (language == "python")
			return "Python";
		else if (language == "javascript")
			return "JavaScript";
		else if (language == "java")
			return "Java";
		else if (language == "ruby")
			return "Ruby";
		else if (language == "php")
			return "PHP";
		else if (language == "swift")
			return "Swift";
		else if (language == "kotlin")
			return "Kotlin";
		else if (language == "typescript")
			return "TypeScript";
		else if (language == "go")
			return "Go";
		else if (language == "rust")
			return "Rust";
		else if (language == "scala")
			return "Scala";
		else if (language == "r")
			return "R";
		else if (language == "perl")
			return "Perl";
		else if (language == "lua")
			return "Lua";
		else if (language == "haskell")
			return "Haskell";
		else if (language == "elixir")
			return "Elixir";
		else if (language == "clojure")
			return "Clojure";
		else if (language == "fsharp")
			return "F#";
		else if (language == "erlang")
			return "Erlang";
		else if (language == "dart")
			return "Dart";
		else if (language == "crystal")
			return "Crystal";
		else if (language == "cobol")
			return "COBOL";
		else if (language == "bash")
			return "Bash";
		else if (language == "assembly")
			return "Assembly";
		else if (language == "powershell")
			return "PowerShell";
		else if (language == "sql")
			return "SQL";
		else if (language == "html")
			return "HTML";
		else if (language == "css")
			return "CSS";
		else if (language == "xml")
			return "XML";
		else if (language == "json")
			return "JSON";
		else if (language == "yaml")
			return "YAML";
		else if (language == "markdown")
			return "Markdown";
	}

	void AssistantPanel::RenderAssistantMessage(const std::string& message, int id) {
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->ChannelsSplit(3);
		drawList->ChannelsSetCurrent(2);

		float padding = 10.0f;
		float maxBubbleWidth = ImGui::GetWindowSize().x * 0.6f;
		float codeToolbarHeight = 30.0f;
		int buttonIndex = 1;

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
					float iconSize = codeToolbarHeight - padding * 1.5;

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding * 2.5);

					ImVec2 cursorPos = ImGui::GetCursorScreenPos();

					float iconCenterY = codeToolbarHeight / 2 - iconSize / 2;
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + iconCenterY / 2);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + bubbleWidth - padding * 2 - iconSize);

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.2));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

					if (ImGui::ImageButton((std::to_string(id) + std::to_string(buttonIndex)).c_str(), (ImTextureID)m_IconCopy->GetRendererID(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1)))
					{
						ImGui::LogToClipboard();
						ImGui::LogText(msg.c_str());
						ImGui::LogFinish();
					}

					ImGui::PopStyleColor(3);

					buttonIndex++;
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() - iconSize - iconCenterY / 2);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding * 2.5);
					ImGui::Text(GetLanguageSymbol(language).c_str());

					drawList->ChannelsSetCurrent(1);

					ImGui::GetWindowDrawList()->AddRectFilled(
						ImVec2(cursorPos.x + padding, cursorPos.y),
						ImVec2(ImGui::GetCursorScreenPos().x + bubbleWidth - padding, ImGui::GetCursorScreenPos().y + padding * 2 + codeToolbarHeight),
						IM_COL32(32, 26, 48, 255), 10.0f, ImDrawFlags_RoundCornersTop
					);

					drawList->ChannelsSetCurrent(2);

					cursorPos = ImGui::GetCursorScreenPos();

					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + bubbleWidth - padding * 2);
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding * 2);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
					ImGui::Text(msg.c_str());
					ImGui::PopTextWrapPos();

					drawList->ChannelsSetCurrent(1);

					ImGui::GetWindowDrawList()->AddRectFilled(
						ImVec2(cursorPos.x + padding, cursorPos.y),
						ImVec2(ImGui::GetCursorScreenPos().x + bubbleWidth - padding, ImGui::GetCursorScreenPos().y + padding),
						IM_COL32(18, 14, 28, 255), 10.0f, ImDrawFlags_RoundCornersBottom
					);

					drawList->ChannelsSetCurrent(2);

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding * 4);
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
		if (m_isListening)
		{
			if (!assistantManager->GetVoiceAssistantListening())
			{
				std::thread t([this] { assistantManager->StartListening(); });
				t.detach();
			}
		}
		else
		{
			if (assistantManager->GetVoiceAssistantListening())
			{
				assistantManager->SetVoiceAssistantListening(false);
				std::thread t([this] { assistantManager->StopListening(); });
				t.detach();
			}
		}

		if (assistantManager->IsNewVoiceMessageAvailable())
		{
			std::string message = assistantManager->GetLastVoiceMessage();
			memcpy(buffer, message.data(), message.size());
			std::thread t(DoMessage, assistantManager, threadID, buffer);
			t.detach();
		}

		ImGui::Begin("Assistant Panel");

		ImGui::Text("This is your conversation with Bob.");
		ImGui::Text("Thread ID : %s", threadID.c_str());

		ImGui::Separator();

		float inputAreaHeight = 100.0f;
		float availableHeight = ImGui::GetWindowSize().y - inputAreaHeight - ImGui::GetCursorPosY();

		ImGui::BeginChild("Messages", ImVec2(ImGui::GetWindowSize().x, availableHeight), true, ImGuiWindowFlags_HorizontalScrollbar);

		std::string lastMessageRole = "";

		for (Message* msg : assistantManager->GetMessages(threadID))
		{
			if (msg->role == "assistant")
				RenderAssistantMessage(msg->content, msg->id);
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

		ImGui::EndChild();

		ImGui::SetCursorPosY(ImGui::GetWindowSize().y - inputAreaHeight + 10);

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.25f, 0.21f, 0.35f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.20f, 0.20f, 0.25f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.25f, 0.25f, 0.30f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));

		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 4.0f)); 

		if (ImGui::InputTextWithHint("##", "Message", buffer, 1024, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::thread t(DoMessage, assistantManager, threadID, buffer);
			t.detach();
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(4);

		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.5));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((ImTextureID)m_IconSend->GetRendererID(), {25, 25}, ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
		{
			std::thread t(DoMessage, assistantManager, threadID, buffer);
			t.detach();
		}

		ImGui::SameLine();

		if (ImGui::ImageButton(m_isListening ? (ImTextureID)m_IconMicrophone->GetRendererID() : (ImTextureID)m_IconMicrophoneOff->GetRendererID(), { 25, 25 }, ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
		{
			m_isListening = !m_isListening;
		}

		ImGui::PopStyleColor(3);

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