#pragma once

#include "imgui.h"
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
	private:
		bool m_BlockEvents = true;
		float m_Time;
		ImVec4 m_LightShade = ImVec4(0.251f, 0.212f, 0.349f, 1.0f);
		ImVec4 m_NormalShade = ImVec4(0.192f, 0.157f, 0.267f, 1.0f);
		ImVec4 m_DarkShade = ImVec4(0.125f, 0.102f, 0.188f, 1.0f);
	};
}