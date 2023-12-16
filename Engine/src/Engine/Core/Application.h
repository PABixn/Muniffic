#pragma once
#include "Window.h"
#include "Engine/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/Core/Timestep.h"


int main(int argc, char** argv);

namespace eg {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			EG_CORE_ASSERT(index < Count);
			return Args[index];
		}
	};

	 class Application
	{
	public:
		Application(const std::string& name = "Muniffic", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
	protected:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		ApplicationCommandLineArgs m_CommandLineArgs;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		bool m_Minimized = false;
	private:
		static Application* s_Instance;
		
	};
	 //To be defined in client
	 Application* CreateApplication(ApplicationCommandLineArgs args);
}