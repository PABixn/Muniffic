#pragma once
namespace eg {
	// Class VRenderer is a wrapper. Static members communicate with the actual Renderer
	class VRenderer
	{
	public:
		static void Init();
		static void Render();
		static void Shutdown();
		static void ImGuiInit();
	private:

	};
}