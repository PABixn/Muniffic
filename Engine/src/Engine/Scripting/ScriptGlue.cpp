#include "egpch.h"
#include "ScriptGlue.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "ScriptEngine.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/Input.h"

namespace eg {

#define EG_ADD_INTERNAL_CALL(Name) mono_add_internal_call("eg.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* text, int number)
	{
		char* cStr = mono_string_to_utf8(text);
		std::string msg(cStr);

		for (int i = 0; i < number; i++)
		{
			std::cout << msg << std::endl;
		}

		mono_free(cStr);
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		EG_CORE_WARN("Value: {0}", *parameter);
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* param) {
		EG_CORE_WARN("Value: {0}", *param);
		return glm::dot(*param, *param);
	}

	static void Entity_GetTranslation(UUID uuid, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void Entity_SetTranslation(UUID uuid, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(uuid);
		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	void ScriptGlue::RegisterFunctions()
	{
		EG_ADD_INTERNAL_CALL(NativeLog);
		EG_ADD_INTERNAL_CALL(NativeLog_Vector);
		EG_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		EG_ADD_INTERNAL_CALL(Entity_GetTranslation);
		EG_ADD_INTERNAL_CALL(Entity_SetTranslation);
		EG_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}