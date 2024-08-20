#include "egpch.h"
#include "Engine/Core/Input.h"
#include <GLFW/glfw3.h>
#include "Engine/Core/Application.h"
#include "Engine/Scripting/ScriptEngine.h"
#include "Platform/Windows/WindowsWindow.h"

namespace eg {
	void Input::KeyCallback(int key, int action)
	{
		switch (action)
		{
			case GLFW_PRESS:
				EG_CORE_INFO("Key "+std::to_string(key)+" pressed");
				for (auto& scriptInstance : ScriptEngine::GetAllScriptInstances())
					scriptInstance->InvokeOnKeyPress(key);
				break;
			case GLFW_RELEASE:
				EG_CORE_INFO("Key " + std::to_string(key) + " released");
				for (auto& scriptInstance : ScriptEngine::GetAllScriptInstances())
					scriptInstance->InvokeOnKeyRelease(key);
				break;
		}
	}

	void Input::MouseCallback(int button, int action)
	{
		switch (action)
		{
			case GLFW_PRESS:
				EG_CORE_INFO("Mouse " + std::to_string(button) + " pressed");
				for (auto& scriptInstance : ScriptEngine::GetAllScriptInstances())
					scriptInstance->InvokeOnMouseButtonPress(button);
				break;
			case GLFW_RELEASE:
				EG_CORE_INFO("Mouse " + std::to_string(button) + " released");
				for (auto& scriptInstance : ScriptEngine::GetAllScriptInstances())
					scriptInstance->InvokeOnMouseButtonRelease(button);
				break;
		}
	}

	void Input::ScrollCallback(double xOffset, double yOffset)
	{
		EG_CORE_INFO("Scroll x:" + std::to_string(xOffset) + " y: " + std::to_string(yOffset));
		for (auto& scriptInstance : ScriptEngine::GetAllScriptInstances())
			scriptInstance->InvokeOnScroll(xOffset, yOffset);
	}

	char* Input::GetClipboardContent()
	{
		return (char*)glfwGetClipboardString(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()));
	}

	void Input::SetClipboardContent(char* content)
	{
		EG_CORE_INFO("Clipboard: " + (std::string)content);
		glfwSetClipboardString(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), content);
	}

	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, (int)keycode);
		return state == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, (int)button);
		return state == GLFW_PRESS;
	}

	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePositon();
		return (float)x;
	}

	float Input::GetMouseY()
	{
		auto[x,y] = GetMousePositon();
		return (float)y;
	}

	std::pair<float, float> Input::GetMousePositon()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { (float)xPos, (float)yPos };
	}
}
