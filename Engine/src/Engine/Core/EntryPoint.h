#pragma once
#include "Log.h"
#include "Application.h"
#ifdef EG_PLATFORM_WINDOWS


extern eg::Application* eg::CreateApplication(ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	eg::Log::Init();

	EG_PROFILE_BEGIN_SESSION("Startup", "EngineProfile-Startup.json");
	auto app = eg::CreateApplication({ argc, argv });
	EG_PROFILE_END_SESSION();

	EG_PROFILE_BEGIN_SESSION("Runtime", "EngineProfile-Runtime.json");
	app->Run();
	EG_PROFILE_END_SESSION();

	EG_PROFILE_BEGIN_SESSION("Shutdown", "EngineProfile-Shutdown.json");
	delete app;
	EG_PROFILE_END_SESSION();

}
#endif