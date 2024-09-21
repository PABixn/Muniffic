#pragma once
#include <string>
#include <memory>
#include <Engine/Assistant/AssistantManager.h>
#include <Imgui/imgui.h>
#include <Engine/Renderer/Texture.h>
#include "Imgui/imgui_markdown.h"

namespace eg
{
    class AssistantPanel
    {
    public:
        AssistantPanel();
        void OnImGuiRender();

    private:
        void DoMessage(const std::string& message);
        void RenderUserMessage(const std::string& message);
        void RenderAssistantMessage(const std::string& message, int id = 0);
        void RunMessage(const std::string& message);
        std::string GetLanguageSymbol(const std::string& language);
        bool StartsOrEndsCodeBlock(std::string line);
        std::string ExtractLanguageName(std::string line);


        char buffer[1024];
        std::string threadID;
        std::unique_ptr<AssistantManager> assistantManager;
        std::string assistantRespondingAnimation;
        Ref<Texture2D> m_IconCopy;
        Ref<Texture2D> m_IconSend;
        Ref<Texture2D> m_IconMicrophone;
        Ref<Texture2D> m_IconMicrophoneOff;
        Ref<Texture2D> m_IconMicrophoneUnavailable;
        Ref<Texture2D> m_IconSettings;
        Ref<Texture2D> m_IconReadAloud;
        Ref<Texture2D> m_IconReadAloudHover;
        Ref<Texture2D> m_IconReadAloudActive;
        ImTextureID m_IconReadMessageAloud;
        bool m_isListening;
        bool m_shouldListen;
        bool m_isMicrophoneAvailable;
        bool m_isLastMessageFromUser;
        bool m_readAloud;
        bool m_showMessageTooltip;
        bool m_isAssistantMessageInProgress;
        ImGui::MarkdownConfig mdConfig;
        int m_messageCount;
    };
}
