#include "egpch.h"
#include "Application.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Engine/Renderer/Renderer.h"
#include "glad/glad.h"
#include "Engine/Utils/PlatformUtils.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Engine/Project/Project.h"



namespace eg
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application *Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification applicationSpec)
		: m_Specification(applicationSpec)
	{
		EG_PROFILE_FUNCTION();
		EG_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// Set working directory here
		if (!m_Specification.WorkingDirectory.empty())
			std::filesystem::current_path(applicationSpec.WorkingDirectory);

		m_Window = Window::Create(WindowProps(applicationSpec.Name));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		m_LayerStack.PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		EG_PROFILE_FUNCTION();
		ScriptEngine::Shutdown();
		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer *layer)
	{
		EG_PROFILE_FUNCTION();
		m_LayerStack.PushLayer(layer);
	}
	
	void Application::PushOverlay(Layer *layer)
	{
		EG_PROFILE_FUNCTION();
		m_LayerStack.PushOverlay(layer);
	}

	void Application::OnEvent(Event &e)
	{
		EG_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		EG_PROFILE_FUNCTION();
		while (m_Running)
		{
			EG_PROFILE_SCOPE("RunLoop");
			float time = Time::GetTime(); // Platform::GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			ExecuteMainThreadQueue();

			if (!m_Minimized)
			{
				{
					EG_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer *layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
			}

			m_ImGuiLayer->Begin();
			{
				EG_PROFILE_SCOPE("LayerStack OnImGuiRender");
				for (Layer *layer : m_LayerStack)
					layer->OnImGuiRender();
			}

			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent &e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent &e)
	{
		EG_PROFILE_FUNCTION();
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

	void Application::SubmitToMainThread(std::function<void()> function)
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);
		m_MainThreadQueue.emplace_back(function);
	}

	void Application::ExecuteMainThreadQueue()
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		for (auto &function : m_MainThreadQueue)
			function();

		m_MainThreadQueue.clear();
	}

	void Application::SetRunning(bool val) {
		m_Running = val;
	}

	void Application::ChangeName(const char* text) {
		if (auto WinWindow = dynamic_cast<WindowsWindow*>(&this->GetWindow()))
			glfwSetWindowTitle((WinWindow)->GetGLFWwindow(), text);

	}

	void Application::ChangeNameWithCurrentProject(bool saved) {
		std::string TitleText = "Muniffic editor [";
		TitleText += Project::GetActive().get()->GetProjectName();
		TitleText += "]";
		TitleText += saved ? "" : "*";
		if (auto WinWindow = dynamic_cast<WindowsWindow*>(&this->GetWindow()))
			glfwSetWindowTitle((WinWindow)->GetGLFWwindow(), TitleText.c_str());

	}
}