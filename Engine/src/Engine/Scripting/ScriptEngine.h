#pragma once
#include <filesystem>
#include "Engine/Core/Timestep.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace eg {

	enum class ScriptFieldType
	{
		None = 0,
		Float, Vec2, Vec3, Vec4,
		Int32, Int64, Bool, Double, Short, Byte, Char,
		UByte, UInt, UInt64, UShort, SByte,
		String, Object, Void,
		Entity//, TransformComponent, RigidBodyComponent
	};

	struct ScriptField
	{
		ScriptField() = default;
		ScriptField(const std::string& name, ScriptFieldType type, MonoClassField* valuePtr = nullptr);

		std::string Name;
		ScriptFieldType Type = ScriptFieldType::None;
		MonoClassField* ClassField = nullptr;
	};


	class ScriptClass {
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		inline MonoClass* GetClass() const { return m_Class; }
		inline const std::string& GetName() const { return m_ClassName; }
		inline const std::string& GetNamespace() const { return m_ClassNamespace; }

		const std::unordered_map<std::string, ScriptField>& GetFields() const { return m_Fields; }

		
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		std::unordered_map<std::string, ScriptField> m_Fields;

		MonoClass* m_Class;

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);
		
		Ref<ScriptClass> GetScriptClass() const { return m_ScriptClass; }

		

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();

			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, const T& value)
		{
			SetFieldValueInternal(name, &value);
		}
	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
	private:
		Ref<ScriptClass> m_ScriptClass;

		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
		inline static char s_FieldValueBuffer[8];
	};

	class ScriptEngine
	{
	public:

		static void Init();
		static void Shutdown();

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExists(const std::string& fullClassName);
		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, Timestep ts);

		static Scene* GetSceneContext();
		static Ref<ScriptInstance> GetEntityScriptInstance(UUID uuid);

		static std::unordered_map<std::string, Ref<ScriptClass>>& GetEnityClasses();
		static MonoImage* GetCoreAssemblyImage();
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		

		friend class ScriptClass;
		friend class ScriptGlue;
	};

	

}