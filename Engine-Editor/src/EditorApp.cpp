#include "Engine.h"
#include "Engine/Core/EntryPoint.h"
#include "Imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"

namespace eg {

	class Editor : public Application 
	{
	public:
		Editor(ApplicationSpecification spec)
			: Application(spec)
		{
			PushLayer(new EditorLayer());
		}

		~Editor() {}

		void OnUpdate()  {
		}
	};
	
	Application* CreateApplication(ApplicationCommandLineArgs args) {
		ApplicationSpecification spec;
		spec.Name = "Editor";
		spec.CommandLineArgs = args;
		return new Editor(spec);
	};
}