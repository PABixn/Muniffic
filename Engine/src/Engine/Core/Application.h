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

	struct ApplicationSpecification
	{
		std::string Name = "Muniffic Application";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	 class Application
	{
	public:

		Application(const ApplicationSpecification specification);
		~Application();

		

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		const ApplicationSpecification& GetSpecification() const { return m_Specification; }
		inline Window& GetWindow() { return *m_Window; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		void SubmitToMainThread(std::function<void()> function);
	private:
		void Run();
		virtual bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		void ExecuteMainThreadQueue();
		
	private:
		ApplicationSpecification m_Specification;
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		bool m_Minimized = false;

		std::vector<std::function<void()>> m_MainThreadQueue;
		std::mutex m_MainThreadQueueMutex;
	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	protected:
		void SetRunning(bool val);
	};
	 //To be defined in client
	 Application* CreateApplication(ApplicationCommandLineArgs args);
}