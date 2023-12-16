#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public eg::Application
{
public:
	Sandbox(eg::ApplicationCommandLineArgs args)
	{
		// PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{
	}
};

eg::Application* eg::CreateApplication(eg::ApplicationCommandLineArgs args)
{
	return new Sandbox(args);
}