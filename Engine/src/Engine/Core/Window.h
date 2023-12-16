#pragma once
#include "../../Engine/vendor/Glad/include/glad/glad.h"
#include "../../Engine/vendor/GLFW/include/GLFW/glfw3.h"
#include "Engine/Events/Event.h"

namespace eg {
	struct WindowProps
	{
		std::string Title;
		uint32_t Width, Height;

		WindowProps(const std::string& title = "Game Engine", uint32_t width = 1600, uint32_t height = 900)
			:Title(title), Width(width), Height(height)
		{}

	};

	// Interface representing a desktop system based Window
	class ENGINE_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		
		static Scope<Window> Create(const WindowProps& props = WindowProps());
		
	};
}