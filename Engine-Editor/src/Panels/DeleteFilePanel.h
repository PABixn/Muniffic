#pragma once

#include "Engine/Core/Application.h"
#include "Imgui/imgui.h"
#include "Engine/Core/UUID.h"

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
		void ShowWindow(UUID uuid, ResourceType type);
		FileDeleteMethod GetResult() { return m_Result; }
		void SetResult(FileDeleteMethod result) { m_Result = result; }
		UUID GetUUID() { return m_UUID; }
		void SetUUID(UUID uuid) { m_UUID = uuid; }
		ResourceType GetType() { return m_Type; }
		void SetType(ResourceType type) { m_Type = type; }
		bool IsShown() { return m_Show; }
		
	protected:
		bool m_Show;
		UUID m_UUID;
		ResourceType m_Type;
		FileDeleteMethod m_Result = FileDeleteMethod::Cancel;
	};
}
