#pragma once

#include <imgui.h>
#include "Engine/Core/Layer.h"
#include "Engine/Events/KeyEvent.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Core/Timestep.h"

namespace eg {
	class ENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		void Begin();
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }
		void SetDarkThemeColors();

		uint32_t GetActiveWidgetID() const;
	public:
		static ImVec4 m_LightShade;
		static ImVec4 m_NormalShade;
		static ImVec4 m_DarkShade;
		static ImVec4 m_LightTextShade;
	private:
		bool m_BlockEvents = true;
		float m_Time;
	};
}