#pragma once

#include "Engine/Core/Application.h"
#include "../EditorLayer.h"
#include "Imgui/imgui.h"
#include <vector>
#include "Engine/Project/Project.h"
#include "egpch.h"
#include <string>
#include <xstring>

namespace eg {

	class ConsolePanel
	{
	public:
		ConsolePanel();
		~ConsolePanel() {};
		void OnImGuiRender();
		enum class LogType {
			Info,
			Warning,
			Error
		};
		void Log(const std::string& message, LogType type);
		void Draw();
		static std::vector<std::pair<std::string, LogType>> Logs;
	private:
		friend class Entity;
	};
}