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
		m_IconMicrophoneUnavailable = Texture2D::Create("resources/icons/micUnavailableIcon.png");
		m_IconSettings = Texture2D::Create("resources/icons/assistantSettingsIcon.png");
		m_isListening = false;
		m_isMicrophoneAvailable = assistantManager->CheckMicrophoneAvailable();
		m_messageInProgress = false;
	}

	AssistantPanel::~AssistantPanel()
	{
		delete assistantManager;
		assistantManager = nullptr;
	}

	void DoMessage(AssistantManager* assistantManager, std::string threadID, char* buffer)
	{
		if (assistantManager->GetVoiceAssistantListening())
		{
			assistantManager->SetVoiceAssistantListening(false);
			assistantManager->StopListening();
		}

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

	std::string AssistantPanel::GetLanguageSymbol(const std::string& language)
	{
		static const std::unordered_map<std::string, std::string> languageMap = {
			{"csharp", "C#"}, {"cpp", "C++"}, {"python", "Python"},
			{"javascript", "JavaScript"}, {"java", "Java"}, {"ruby", "Ruby"},
			{"php", "PHP"}, {"swift", "Swift"}, {"kotlin", "Kotlin"},
			{"typescript", "TypeScript"}, {"go", "Go"}, {"rust", "Rust"},
			{"scala", "Scala"}, {"r", "R"}, {"perl", "Perl"},
			{"lua", "Lua"}, {"haskell", "Haskell"}, {"elixir", "Elixir"},
			{"clojure", "Clojure"}, {"fsharp", "F#"}, {"erlang", "Erlang"},
			{"dart", "Dart"}, {"crystal", "Crystal"}, {"cobol", "COBOL"},
			{"bash", "Bash"}, {"assembly", "Assembly"}, {"powershell", "PowerShell"},
			{"sql", "SQL"}, {"html", "HTML"}, {"css", "CSS"},
			{"xml", "XML"}, {"json", "JSON"}, {"yaml", "YAML"},
			{"markdown", "Markdown"}
		};

		auto it = languageMap.find(language);
		return (it != languageMap.end()) ? it->second : "";
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
		ImGui::Begin("Assistant Panel");

		if (assistantManager->IsNewVoiceMessageAvailable())
		{
			std::string message = assistantManager->GetLastVoiceMessage();
			memcpy(buffer, message.data(), message.size());
			RunMessage(buffer);
		}

		if (assistantManager->GetIsMessageInProgress())
			m_isListening = false;

		if (m_isListening)
		{
			if (!assistantManager->GetVoiceAssistantListening())
			{
				assistantManager->SetVoiceAssistantListening(true);
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

		float buttonSize = 25.0f;
		float availableHeight = ImGui::GetWindowSize().y - buttonSize * 4 - ImGui::GetCursorPosY();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x - buttonSize * 1.5);

		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 center = ImVec2(pos.x + buttonSize * 0.5f, pos.y + buttonSize * 0.5f);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((ImTextureID)m_IconSettings->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
		{
			ImGui::OpenPopup("Assistant Settings");
		}

		if (ImGui::IsItemHovered())
		{
			float glow_radius = 1.0f;
			ImVec4 glow_color = ImVec4(0.8f, 0.8f, 1.0f, 0.3f);

			draw_list->AddCircleFilled(center, buttonSize * 0.5f + glow_radius, ImColor(glow_color), 32);
		}

		ImGui::PopStyleColor(3);

		if (ImGui::BeginPopupModal("Assistant Settings", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.14f, 0.12f, 0.22f, 1.0f));         
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.88f, 0.85f, 0.96f, 1.0f));             
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.25f, 0.21f, 0.35f, 1.00f));
			ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.35f, 0.30f, 0.45f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.21f, 0.35f, 1.0f));           
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.30f, 0.45f, 1.0f));  
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45f, 0.40f, 0.55f, 1.0f));     
			ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.88f, 0.85f, 0.96f, 1.0f));       
			ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.35f, 0.30f, 0.45f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.25f, 0.21f, 0.35f, 1.0f));           
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.35f, 0.30f, 0.45f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.45f, 0.40f, 0.55f, 1.0f));    
			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.25f, 0.21f, 0.35f, 1.0f));

			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 6.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);    
			ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 8.0f);       
			ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 12.0f);      

			ImGui::Text("Assistant's Settings");

			static char instructions[256] = "";
			ImGui::Text("Additional instructions");
			ImGui::InputTextMultiline("##instructions", instructions, IM_ARRAYSIZE(instructions), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 5));

			static float volume = 0.5f;
			ImGui::Text("Assistant's volume");
			ImGui::SliderFloat("##volume", &volume, 0.0f, 1.0f, "%.2f");

			ImGui::SameLine();

			const char* languages[] = { "C++", "Python", "JavaScript", "Java", "C#"};
			static int current_language = 0;
			ImGui::Text("Preferred language");
			ImGui::Combo("##language", &current_language, languages, IM_ARRAYSIZE(languages));

			if (ImGui::Button("Close", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::PopStyleColor(13);
			ImGui::PopStyleVar(6);

			ImGui::EndPopup();
		}

		ImGui::BeginChild("Messages", ImVec2(ImGui::GetWindowSize().x, availableHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

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

		ImGui::SetCursorPosY(ImGui::GetWindowSize().y - buttonSize * 1.5);

		pos = ImGui::GetCursorScreenPos();
		ImVec2 inputSize = ImVec2(300, 30);

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.25f, 0.21f, 0.35f, 1.00f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 1.00f, 1.00f, 1.00f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 6.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);

		if (ImGui::InputTextWithHint("##", "Message", buffer, 1024, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			RunMessage(buffer);
		}

		bool isHovered = ImGui::IsItemHovered();
		bool isActive = ImGui::IsItemActive();

		if (isHovered || isActive) {
			ImVec4 hoverColor = isHovered ? ImVec4(0.45f, 0.42f, 0.55f, 1.00f) : ImVec4(0.55f, 0.50f, 0.70f, 1.00f);
			ImColor borderColor = ImColor(hoverColor);

			float totalWidth = ImGui::CalcItemWidth();

			draw_list->AddRect(pos, ImVec2(pos.x + totalWidth, pos.y + inputSize.y), borderColor, 8.0f, 0, 2.0f);
		}

		ImGui::PopStyleVar(3);
		ImGui::PopStyleColor(2);

		ImGui::SameLine();

		pos = ImGui::GetCursorScreenPos();
		center = ImVec2(pos.x + buttonSize * 0.5f, pos.y + buttonSize * 0.5f);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((ImTextureID)m_IconSend->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
		{
			RunMessage(buffer);
		}

		if (ImGui::IsItemHovered())
		{
			float glow_radius = 1.0f;
			ImVec4 glow_color = ImVec4(0.8f, 0.8f, 1.0f, 0.3f);

			draw_list->AddCircleFilled(center, buttonSize * 0.5f + glow_radius, ImColor(glow_color), 32);
		}

		ImGui::SameLine();

		pos = ImGui::GetCursorScreenPos();
		center = ImVec2(pos.x + buttonSize * 0.5f, pos.y + buttonSize * 0.5f);

		if (ImGui::ImageButton(m_isMicrophoneAvailable ? m_isListening ? (ImTextureID)m_IconMicrophone->GetRendererID() : (ImTextureID)m_IconMicrophoneOff->GetRendererID() : (ImTextureID)m_IconMicrophoneUnavailable->GetRendererID(), { buttonSize, buttonSize }, ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
		{
			if(m_isMicrophoneAvailable)
				m_isListening = !m_isListening;
		}

		if (ImGui::IsItemHovered())
		{
			float glow_radius = 1.0f;
			ImVec4 glow_color = ImVec4(0.8f, 0.8f, 1.0f, 0.5f);

			draw_list->AddCircleFilled(center, buttonSize * 0.5f + glow_radius, ImColor(glow_color), 32);
		}

		if (!m_isMicrophoneAvailable && ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::TextUnformatted("Microphone unavailable");
			ImGui::EndTooltip();
		}

		ImGui::PopStyleColor(3);

		ImGui::End();
	}

	void AssistantPanel::RunMessage(const std::string message)
	{
		if(message.empty())
			return;

		assistantManager->SetMessageInProgress(true);

		std::thread t(DoMessage, assistantManager, threadID, buffer);
		t.detach();
	}
}