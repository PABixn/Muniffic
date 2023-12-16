#include "egpch.h"
#include "Window.h"

#ifdef EG_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace eg
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
#ifdef EG_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		EG_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
	}

}