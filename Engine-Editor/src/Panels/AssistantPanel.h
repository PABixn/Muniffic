#pragma once
#include <string>
#include <memory>
#include <Engine/Assistant/AssistantManager.h>
#include <Imgui/imgui.h>
#include <Engine/Renderer/Texture.h>

namespace eg
{
    class AssistantPanel
    {
    public:
        AssistantPanel();
        void OnImGuiRender();

    private:
        std::string LoadInstructions();
        void DoMessage(const std::string& message);
        void RenderUserMessage(const std::string& message);
        void RenderAssistantMessage(const std::string& message, int id = 0);
        void RunMessage(const std::string& message);
        std::string GetLanguageSymbol(const std::string& language);
        bool StartsOrEndsCodeBlock(std::string line);
        std::string ExtractLanguageName(std::string line);
        void RenderAssistantSettings();
        void UpdateAssistantStatus();

    private:
        std::string threadID;
        std::string assistantRespondingAnimation;
        Ref<AssistantManager> assistantManager = nullptr;
        char buffer[1024];
        bool m_isListening;
        bool m_shouldListen;
        bool m_isMicrophoneAvailable;
        bool m_isLastMessageFromUser;
        bool m_readAloud;
        bool m_showMessageTooltip;
        bool m_isAssistantMessageInProgress;
        bool m_assistantInitialized;
        int m_messageCount;
    };
}
