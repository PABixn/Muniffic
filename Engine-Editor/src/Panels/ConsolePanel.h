#pragma once

#include "Engine/Core/Application.h"
#include "../EditorLayer.h"
#include "Imgui/imgui.h"
#include <vector>
#include "Engine/Project/Project.h"
#include "egpch.h"
#include <string>
#include <xstring>
#include <unordered_map>
#include <chrono>
#include <ctime>

namespace eg {
	


	class ConsolePanel
	{
	public:
		enum class LogType {
			Info, Warning, Error
		};
		class LogMessage {
		public:
			LogMessage(const std::string& timestamp, const std::string& message, ConsolePanel::LogType logType)
				:timestamp(timestamp), message(message), logType(logType) {}
			std::string timestamp;
			std::string message;
			ConsolePanel::LogType logType;
		};
		ConsolePanel();
		~ConsolePanel() {};

		void OnImGuiRender();
		
		static void Log(const std::string& message, LogType type);
		void Draw();
		static std::vector<LogMessage*> Logs;
		static void ClearLogs();
		static std::string getCurrentTime();

	};
}