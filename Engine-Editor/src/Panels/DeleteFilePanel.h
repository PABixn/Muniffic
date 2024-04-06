#pragma once

#include "Engine/Core/Application.h"
#include "Imgui/imgui.h"
#include "Engine/Core/UUID.h"
#include "../Commands/Commands.h"

namespace eg
{
	class DeleteFilePanel
	{
	public:
		DeleteFilePanel();
		void OnImGuiRender();
		void ShowWindow(UUID uuid, ResourceType type);
		bool IsShown() { return m_Show; }
		
	protected:
		bool m_Show;
		UUID m_UUID;
		ResourceType m_Type;
	};
}
