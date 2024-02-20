#include "ConsolePanel.h"

namespace eg {
	ConsolePanel::ConsolePanel() {

    };
    std::vector<ConsolePanel::LogMessage*> ConsolePanel::Logs;
    enum class LogType {
        Info,
        Warning,
        Error
    };
    void ConsolePanel::Log(const std::string& message, LogType type) {
        Logs.push_back(new ConsolePanel::LogMessage(message, type));
    }
    void ConsolePanel::Draw() {
        std::string message;
        for (auto& log : Logs) {
            switch (log->logType) {
			case LogType::Info:
                message = "INFO  |  " + log->message;
				ImGui::Text(message.c_str());
				break;
			case LogType::Warning:
                message = "WARNING  |  " + log->message;
                ImGui::Text(message.c_str());
				break;
			case LogType::Error:
                message = "ERROR  |  " + log->message;
                ImGui::Text(message.c_str());
				break;
			}
		}
		Logs.clear();
	}
    void ConsolePanel::OnImGuiRender() {
        ImGui::Begin("Console");
        this->Draw();
        ImGui::End();
    };
	
}