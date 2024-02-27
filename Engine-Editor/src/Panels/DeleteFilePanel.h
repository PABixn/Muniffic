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

		FileDeleteMethod m_Result = FileDeleteMethod::Cancel;
		bool m_Show;
	};
}
