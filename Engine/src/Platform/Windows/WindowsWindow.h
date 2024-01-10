#pragma once
#include "Engine/Core/Window.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace eg
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height; }

		//Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		virtual void* GetNativeWindow() const override { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GraphicsContext* m_Context;

		struct WindowData
		{
			std::string Title;
			uint32_t Width = 0, Height = 0;
			bool VSync = true;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
		float m_Time;
		GLFWwindow* m_Window;
	public:
		inline GLFWwindow* GetGLFWwindow() const { return m_Window; };
	};
	
}