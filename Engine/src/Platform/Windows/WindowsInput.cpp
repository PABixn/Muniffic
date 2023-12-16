#include "egpch.h"
#include "Engine/Core/Input.h"
#include <GLFW/glfw3.h>
#include "Engine/Core/Application.h"

namespace eg {
	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, (int)keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
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
