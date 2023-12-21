#include <Engine.h>
#include <Engine/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public eg::Application
{
public:
	Sandbox(const eg::ApplicationSpecification& specification)
		: eg::Application(specification)
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
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Engine-Editor";
	spec.CommandLineArgs = args;
	return new Sandbox(spec);
}