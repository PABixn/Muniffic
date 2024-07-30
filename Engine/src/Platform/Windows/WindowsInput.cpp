#include "egpch.h"
#include "Engine/Core/Input.h"
#include <GLFW/glfw3.h>
#include "Engine/Core/Application.h"
#include "Engine/Scripting/ScriptEngine.h"

namespace eg {
	void Input::key_callback(auto window, int key, int scancode, int action, int mods)
	{
		switch (action) {
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

	void Input::Init() 
	{
		glfwSetKeyCallback(static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow()), key_callback);
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
