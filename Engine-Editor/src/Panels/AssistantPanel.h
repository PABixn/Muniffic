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
        void RenderUserMessage(const std::string& message);
        void RenderAssistantMessage(const std::string& message, int id = 0);
        void RunMessage(const std::string& message);
        std::string GetLanguageSymbol(const std::string& language);

    private:
        void DoMessage(const std::string& message);

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
        bool m_isListening;
        bool m_isMicrophoneAvailable;
        bool m_isLastMessageFromUser;
    };
}
