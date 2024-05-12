#include "egpch.h"
#include "AssistantManager.h"

namespace eg
{
	AssistantManager::AssistantManager()
	{
		Init();
	}

	void AssistantManager::Init()
	{
		Py_Initialize();
	}
}