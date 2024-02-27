#pragma once

#include "Engine/Core/Application.h"
#include "Imgui/imgui.h"

namespace eg
{
	enum class FileDeleteMethod
	{
		DeleteFromProject,
		DeleteFromDisk,
		Cancel
	};

	class DeleteFilePanel
	{
	public:
		DeleteFilePanel();
		void OnImGuiRender();
		void ShowWindow(std::filesystem::path keyPath, ResourceType type);
		FileDeleteMethod GetResult() { return m_Result; }
		void SetResult(FileDeleteMethod result) { m_Result = result; }
		std::filesystem::path GetKeyPath() { return m_KeyPath; }
		void SetKeyPath(std::filesystem::path keyPath) { m_KeyPath = keyPath; }
		ResourceType GetType() { return m_Type; }
		void SetType(ResourceType type) { m_Type = type; }
		bool IsShown() { return m_Show; }
		
	protected:
		bool m_Show;
		std::filesystem::path m_KeyPath;
		ResourceType m_Type;
		FileDeleteMethod m_Result = FileDeleteMethod::Cancel;
	};
}
