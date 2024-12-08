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
		static void SetDarkThemeColors();
		static void ReturnToOriginalTheme();

		uint32_t GetActiveWidgetID() const;
	public:
		static ImVec4 m_LightShade;
		static ImVec4 m_NormalShade;
		static ImVec4 m_DarkShade;
		static ImVec4 m_LightTextShade;
		static const ImVec4 m_OriginalLightShade;
		static const ImVec4 m_OriginalNormalShade;
		static const ImVec4 m_OriginalDarkShade;
		static const ImVec4 m_OriginalLightTextShade;
		static const ImVec4 m_lightRatio;
		static const ImVec4 m_NormalRatio;
	private:
		bool m_BlockEvents = true;
		float m_Time;
	};
}