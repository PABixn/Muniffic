#pragma once
#include <Imgui/imgui.h>

namespace eg::UI
{
	struct ScopedStyleColor
	{
		ScopedStyleColor() = default;
		ScopedStyleColor(ImGuiCol idx, const ImVec4& col, bool predicate = true)
			: m_Set(predicate)
		{
			if (m_Set)
				ImGui::PushStyleColor(idx, col);
		}
		ScopedStyleColor(ImGuiCol idx, ImU32 color, bool predicate = true)
			: m_Set(predicate)
		{
			if(m_Set)
				ImGui::PushStyleColor(idx, color);
		}

		~ScopedStyleColor()
		{
			if (m_Set)
				ImGui::PopStyleColor();
		}
	private:
		bool m_Set = false;
	};
}