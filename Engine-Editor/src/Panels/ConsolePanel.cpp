#include "ConsolePanel.h"

namespace eg {
	ConsolePanel::ConsolePanel() {
        EG_PROFILE_FUNCTION();
    };
    std::vector<ConsolePanel::LogMessage*> ConsolePanel::Logs;
    enum class LogType {
        Info,
        Warning,
        Error
    };
    void ConsolePanel::ClearLogs() {
        EG_PROFILE_FUNCTION();
        Logs.clear();
    };
    std::string ConsolePanel::getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::tm* localTime = std::localtime(&currentTime);
        std::string time = std::to_string(localTime->tm_hour) + ":" + std::to_string(localTime->tm_min) + ":" + std::to_string(localTime->tm_sec);
        return time;
    }
    void ConsolePanel::Draw() {
        EG_PROFILE_FUNCTION();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 8.0f));
        for (auto& log : Logs) {
            switch (log->logType) {
            case LogType::Info:
                ImGui::TextColored(ImVec4(0.0f, 0.6f, 1.0f, 1.0f), "[INFO]");
                break;
            case LogType::Warning:
                ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), "[WARNING]");
                break;
            case LogType::Error:
                ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "[ERROR]");
                break;
            }
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), log->timestamp.c_str());
            ImGui::SameLine();
            ImGui::TextWrapped(log->message.c_str());
        }
        ImGui::PopStyleVar();
        //ConsolePanel::ClearLogs();
    };

    void ConsolePanel::Log(const std::string& message, LogType type) {
        EG_PROFILE_FUNCTION();
        Logs.push_back(new ConsolePanel::LogMessage(ConsolePanel::getCurrentTime(), message, type));
    }

    void ConsolePanel::OnImGuiRender() {
        EG_PROFILE_FUNCTION();
        ImGui::Begin("Console");
        this->Draw();
        ImGui::End();
    };




}
