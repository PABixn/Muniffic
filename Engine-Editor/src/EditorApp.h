#pragma once
#include "Engine.h"
#include "Engine/Core/EntryPoint.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "EditorLayer.h"
#include "Commands/Commands.h"
#include "GLFW/include/GLFW/glfw3.h"
#include "Platform/Windows/WindowsWindow.h"
#include "stb_image.h"


namespace eg {


	class Editor : public Application
	{
	public:
		Editor(ApplicationSpecification spec)
			: Application(spec) {
			PushLayer(new EditorLayer());
			ChangeNameWithCurrentProject(true);
			TCHAR buffer[MAX_PATH] = { 0 };
			GetModuleFileName(NULL, buffer, MAX_PATH);
			std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
			std::wstring finalPath = std::wstring(buffer).substr(0, pos);
			pos = finalPath.find_last_of(L"\\/");
			finalPath = finalPath.substr(0, pos);
			pos = finalPath.find_last_of(L"\\/");
			finalPath = finalPath.substr(0, pos);
			pos = finalPath.find_last_of(L"\\/");
			finalPath = finalPath.substr(0, pos);
			std::string str;
			size_t size;
			str.resize(finalPath.length());
			wcstombs_s(&size, &str[0], str.size() + 1, finalPath.c_str(), finalPath.size());
			str += "/Engine-Editor/resources/icons/EditorIcon/logo.png";
			GLFWimage images[1]; 
			images[0].pixels = stbi_load(str.c_str(), &images[0].width, &images[0].height, 0, 4); 
			const char* failReason = stbi_failure_reason();

			glfwSetWindowIcon((GLFWwindow*)this->GetWindow().GetNativeWindow(), 1, images);
			stbi_image_free(images[0].pixels);
		};

		~Editor() {}

		void OnUpdate() {}
		bool OnWindowClose(WindowCloseEvent& e);
	};
}