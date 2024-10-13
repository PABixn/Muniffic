#include "egpch.h"
#include "AssistantPanel.h"
#include <Imgui/imgui.h>
#include "Engine/Project/Project.h"
#include "Imgui/imgui_markdown.h"
#include <thread>
#include <sstream>
#include <unordered_map>
#include <cctype>
#include <algorithm>
#include "../MarkdownRenderer/Markdown.h"
#include "../IconLoader.h"

namespace eg
{
	AssistantPanel::AssistantPanel()
		: assistantManager(CreateScope<AssistantManager>()),
		assistantRespondingAnimation("."),
		m_isListening(false),
		m_isMicrophoneAvailable(false),
		m_isLastMessageFromUser(false),
		m_readAloud(false),
		m_showMessageTooltip(false),
		m_messageCount(0),
		m_isAssistantMessageInProgress(false),
		m_shouldListen(false),
		m_assistantInitialized(false)
	{
        EG_PROFILE_FUNCTION();
		memset(buffer, 0, sizeof(buffer));

		mdConfig = ImGui::MarkdownConfig();

		m_assistantInitialized = assistantManager->CheckAPI();

		if(!m_assistantInitialized)
			return;

		m_isMicrophoneAvailable = assistantManager->CheckMicrophoneAvailable();

		if (!assistantManager->LoadAssistant())
		{
			std::string instructions = LoadInstructions();

			if (instructions == "")
				return;

			if(assistantManager->CreateAssistant("Bob", instructions))
				m_assistantInitialized = true;
		}
		else
			m_assistantInitialized = true;

		if(m_assistantInitialized)
			threadID = assistantManager->CreateThread();
	}

	std::string AssistantPanel::LoadInstructions()
	{
        EG_PROFILE_FUNCTION();
		std::string filename = "resources/assistant/instructions.txt";

		std::ifstream file(filename);
		if (!file.is_open()) {
			EG_CORE_ERROR("Failed to load assistant's instructions: {0}", filename);
			return "";
		}

		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	void AssistantPanel::DoMessage(const std::string& message)
	{
        EG_PROFILE_FUNCTION();
		if (assistantManager->GetVoiceAssistantListening())
		{
			assistantManager->SetVoiceAssistantListening(false);
			assistantManager->StopListening();
		}

		assistantManager->AddMessage(threadID, message);
		assistantManager->InitiateRun(threadID);
		assistantManager->WaitForCompletion(threadID);
	}

	void AssistantPanel::RenderUserMessage(const std::string& message)
	{
        EG_PROFILE_FUNCTION();
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
        EG_PROFILE_FUNCTION();
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

	std::string AssistantPanel::ExtractLanguageName(std::string line) {
        EG_PROFILE_FUNCTION();
		if(line.find("```") == std::string::npos)
			return "";

		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
		return line.substr(3);
	}

	bool AssistantPanel::StartsOrEndsCodeBlock(std::string line) {
        EG_PROFILE_FUNCTION();
		if (line.find("```") != std::string::npos)
		{
			line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
			return line.starts_with("```");
		}

		return false;
	}

	void AssistantPanel::RenderAssistantMessage(const std::string& message, int id)
	{
        EG_PROFILE_FUNCTION();
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->ChannelsSplit(3);
		drawList->ChannelsSetCurrent(2);

		float padding = 10.0f;
		float maxBubbleWidth = ImGui::GetWindowSize().x * 0.6f;
		float codeToolbarHeight = 30.0f;
		int buttonIndex = 1;

		std::stringstream stream(message);
		std::string line = "", language = "", msg = "";
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

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);

		while (std::getline(stream, line))
		{
			if (StartsOrEndsCodeBlock(line))
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

					if (ImGui::ImageButton((std::to_string(id) + std::to_string(buttonIndex)).c_str(), (ImTextureID)IconLoader::GetIcon(Icons::Assistant_CopyCode)->GetRendererID(), ImVec2(iconSize, iconSize), ImVec2(0, 0), ImVec2(1, 1)))
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
						ImVec2(ImGui::GetCursorScreenPos().x + bubbleWidth - padding, ImGui::GetCursorScreenPos().y + codeToolbarHeight),
						IM_COL32(32, 26, 48, 255), 10.0f, ImDrawFlags_RoundCornersTop
					);

					drawList->ChannelsSetCurrent(2);

					cursorPos = ImGui::GetCursorScreenPos();

					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + bubbleWidth - padding * 2);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
					//ImGui::Markdown(msg.c_str(), msg.size(), mdConfig);
					Markdown::text(msg, ImGui::GetCursorPos().x + bubbleWidth - padding * 2, padding * 2);
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
					//ImGui::Markdown(msg.c_str(), msg.size(), mdConfig, padding);
					Markdown::text(msg, ImGui::GetCursorPos().x + bubbleWidth - padding, padding);
					ImGui::PopTextWrapPos();
					msg = "";

					insideCode = true;
					language = ExtractLanguageName(line);
				}
			}
			else
			{
				if (!msg.empty())
					msg += "\n";
				msg += line;
			}
		}

		ImGui::PopTextWrapPos();

		if (!msg.empty())
		{
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + bubbleWidth - padding);
			//ImGui::Markdown(msg.c_str(), msg.size(), mdConfig, padding);
			Markdown::text(msg, ImGui::GetCursorPos().x + bubbleWidth - padding, padding);
			ImGui::PopTextWrapPos();
		}

		drawList->ChannelsSetCurrent(0);

		ImGui::GetWindowDrawList()->AddRectFilled(
			initialCursorPos,
			ImVec2(ImGui::GetCursorScreenPos().x + bubbleWidth, ImGui::GetCursorScreenPos().y + padding),
			IM_COL32(64, 54, 89, 255), 10.0f
		);

		if (ImGui::IsMouseHoveringRect(initialCursorPos,
			ImVec2(ImGui::GetCursorScreenPos().x + bubbleWidth, ImGui::GetCursorScreenPos().y + padding)))
			m_showMessageTooltip = true;
		else
			m_showMessageTooltip = false;

		drawList->ChannelsMerge();

		ImGui::EndGroup();

		ImGui::NewLine();
	}

	void AssistantPanel::OnImGuiRender()
	{
        EG_PROFILE_FUNCTION();
		ImGui::Begin("Assistant Panel");

		if (!m_assistantInitialized)
		{
			ImGui::Text("Assistant initialization failed");
			ImGui::End();
			return;
		}

		UpdateAssistantStatus();

		float buttonSize = 25.0f;
		float availableHeight = ImGui::GetWindowSize().y - buttonSize * 4 - ImGui::GetCursorPosY();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		ImGui::SetCursorPosX(ImGui::GetWindowSize().x - buttonSize * 1.5);

		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 center = ImVec2(pos.x + buttonSize * 0.5f, pos.y + buttonSize * 0.5f);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

		if (ImGui::ImageButton((ImTextureID)IconLoader::GetIcon(Icons::Assistant_Settings)->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
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

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + buttonSize * 0.5);

		RenderAssistantSettings();

		ImGui::BeginChild("Messages", ImVec2(ImGui::GetWindowSize().x, availableHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

		if (assistantManager->ThreadAvailable(threadID))
		{
			m_isLastMessageFromUser = assistantManager->GetLastMessageRole(threadID) == "user";
			for (AssistantManager::Message* msg : assistantManager->GetMessages(threadID))
			{
				if (msg->role == "assistant")
					RenderAssistantMessage(msg->content, msg->id);
				else
					RenderUserMessage(msg->content);

				ImGui::NewLine();
			}

			if (m_isLastMessageFromUser)
			{
				static float timer = 0.0f;
				timer += ImGui::GetIO().DeltaTime;

				if (m_isAssistantMessageInProgress == false)
					m_isAssistantMessageInProgress = true;

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
			else
				m_isAssistantMessageInProgress = false;

			if (m_messageCount != assistantManager->GetMessages(threadID).size() || m_isAssistantMessageInProgress)
			{
				m_messageCount = assistantManager->GetMessages(threadID).size();
				ImGui::SetScrollHereY(1.0f);
			}
		}

		ImGui::EndChild();

		if (m_showMessageTooltip)
		{
			ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.25f, 0.21f, 0.35f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);

			if(ImGui::BeginPopupContextItem("MessageTooltip"))
			{
				/*if (ImGui::ImageButton(m_IconReadMessageAloud, ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
				{
					m_IconReadMessageAloud = (ImTextureID)m_IconReadAloudActive->GetRendererID();
				}

				if (ImGui::IsItemHovered())
					m_IconReadMessageAloud = (ImTextureID)m_IconReadAloudHover->GetRendererID();
				else
					m_IconReadMessageAloud = (ImTextureID)m_IconReadAloud->GetRendererID();*/


				ImGui::EndPopup();
			}
			ImGui::PopStyleColor(4);
			ImGui::PopStyleVar(1);
		}

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

		if (ImGui::ImageButton((ImTextureID)IconLoader::GetIcon(Icons::Assistant_Send)->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
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

		ImTextureID icon = m_isMicrophoneAvailable ? m_isListening ? (ImTextureID)IconLoader::GetIcon(Icons::Assistant_Microphone)->GetRendererID() : (ImTextureID)IconLoader::GetIcon(Icons::Assistant_MicrophoneOff)->GetRendererID() : (ImTextureID)IconLoader::GetIcon(Icons::Assistant_MicrophoneUnavailable)->GetRendererID();

		if (ImGui::ImageButton(icon, { buttonSize, buttonSize }, ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
		{
			if (m_isMicrophoneAvailable)
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

	void AssistantPanel::UpdateAssistantStatus()
	{
        EG_PROFILE_FUNCTION();
		if (assistantManager->IsNewVoiceMessageAvailable())
		{
			std::string message = assistantManager->GetLastVoiceMessage();
			if (message.size() < 1024)
			{
				memcpy(buffer, message.data(), message.size());
				RunMessage(message);
			}
		}

		if (assistantManager->GetIsMessageInProgress())
			m_isListening = false;
		else if (m_shouldListen)
			m_isListening = true;

		if (m_isListening && !assistantManager->GetVoiceAssistantListening())
		{
			assistantManager->SetVoiceAssistantListening(true);
			std::thread(&AssistantManager::StartListening, assistantManager.get()).detach();
		}
		else if (!m_isListening && assistantManager->GetVoiceAssistantListening())
		{
			assistantManager->SetVoiceAssistantListening(false);
			std::thread(&AssistantManager::StopListening, assistantManager.get()).detach();
		}
	}

	void AssistantPanel::RenderAssistantSettings()
	{
        EG_PROFILE_FUNCTION();
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

			ImGui::Text("Read aloud");
			ImGui::Checkbox("##readAloud", &m_readAloud);

			ImGui::Text("Should voice assistant listen");
			if (ImGui::Checkbox("##listen", &m_shouldListen))
			{
				if (m_shouldListen)
					m_isListening = true;
				else
					m_isListening = false;
			}

			const char* languages[] = { "C++", "Python", "JavaScript", "Java", "C#" };
			static int current_language = 0;
			ImGui::Text("Preferred language");
			ImGui::Combo("##language", &current_language, languages, IM_ARRAYSIZE(languages));

			if (ImGui::Button("Close", ImVec2(120, 0)))
			{
				std::thread([this] { assistantManager->SetVolume(volume); }).detach();
				ImGui::CloseCurrentPopup();
			}

			ImGui::PopStyleColor(13);
			ImGui::PopStyleVar(6);

			ImGui::EndPopup();
		}
	}

	void AssistantPanel::RunMessage(const std::string& message)
	{
        EG_PROFILE_FUNCTION();
		if (message.empty() || m_isLastMessageFromUser)
			return;

		assistantManager->SetMessageInProgress(true);
		assistantManager->SetShouldReadAloud(m_readAloud);

		memset(buffer, 0, sizeof(buffer));
		std::thread(&AssistantPanel::DoMessage, this, message).detach();
	}
}
