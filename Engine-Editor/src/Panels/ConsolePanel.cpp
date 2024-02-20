#include "ConsolePanel.h"

namespace eg {
	ConsolePanel::ConsolePanel() {
    };
    enum class LogType {
        Info,
        Warning,
        Error
    };
    void ConsolePanel::Log(const std::string& message, LogType type) {
        Logs.push_back({ message, type });
    }
    void ConsolePanel::Draw() {
        std::string message;
        for (auto& log : Logs) {
            switch (log.second) {
			case LogType::Info:
                message = "INFO  |  " + log.first;
				ImGui::Text(message.c_str());
				break;
			case LogType::Warning:
                message = "WARNING  |  " + log.first;
                ImGui::Text(message.c_str());
				break;
			case LogType::Error:
                message = "ERROR  |  " + log.first;
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