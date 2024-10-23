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
        for (auto& log : Logs) {
            switch (log->logType) {
            case LogType::Info:
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.5f, 1.0f));
                ImGui::Button("INFO##xx");
                ImGui::PopStyleColor();
                ImGui::SameLine();
                ImGui::Text(log->message.c_str());
                break;
            case LogType::Warning:
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.0f, 1.0f));
                ImGui::Button("WARNING##xx");
                ImGui::PopStyleColor();
                ImGui::SameLine();
                ImGui::Text(log->message.c_str());
                break;
            case LogType::Error:
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.0f, 1.0f));
                ImGui::Button("ERROR##xx");
                ImGui::PopStyleColor();
                ImGui::SameLine();
                ImGui::Text(log->message.c_str());
                break;
            }
        }
        //ConsolePanel::ClearLogs();
    };

    void ConsolePanel::Log(const std::string& message, LogType type) {
        EG_PROFILE_FUNCTION();
        std::string mess = ConsolePanel::getCurrentTime() + " | " + message;
        Logs.push_back(new ConsolePanel::LogMessage(mess, type));
    }

    void ConsolePanel::OnImGuiRender() {
        EG_PROFILE_FUNCTION();
        ImGui::Begin("Console");
        this->Draw();
        ImGui::End();
    };




}
