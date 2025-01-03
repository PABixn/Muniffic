#pragma once
#include <filesystem>
#include "Engine/Core/Timestep.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Scene/Entity.h"
#include <Engine/Scene/ColliderEvents.h>
#include "ScriptingTypes.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoString MonoString;
}

namespace eg {

	

	struct ScriptField
	{
		ScriptField() = default;
		ScriptField(const std::string& name, ScriptFieldType type, MonoClassField* valuePtr = nullptr);

		std::string Name;
		ScriptFieldType Type = ScriptFieldType::None;
		MonoClassField* ClassField = nullptr;
	};

	struct ScriptMethod
	{
		ScriptMethod() = default;
		ScriptMethod(const std::string& name, MonoMethod* method);

		std::string Name;
		MonoMethod* Method = nullptr;
	};

	// ScriptField + data storage
	struct ScriptFieldInstance
	{
		 ScriptField Field;

		 ScriptFieldInstance()
		 {
			 memset(m_Buffer, 0, sizeof(m_Buffer));
		 }

		 template<typename T>
		 T GetValue()
		 {
			 static_assert(sizeof(T) <= 16, "Type is too large");
			 return *(T*)m_Buffer;
		 }

		 template<typename T>
		 void SetValue(T value)
		 {
			static_assert(sizeof(T) <= 16, "Type is too large");
			memcpy(m_Buffer, &value, sizeof(T));
		 }
	private:
		 uint8_t m_Buffer[16];

		 friend class ScriptEngine;
		 friend class ScriptInstance;
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
		const std::unordered_map<std::string, ScriptMethod>& GetMethods() const { return m_Methods; }
		
	private:
		std::string m_ClassNamespace;
		std::string m_ClassName;

		std::unordered_map<std::string, ScriptField> m_Fields;
		std::unordered_map<std::string, ScriptMethod> m_Methods;

		MonoClass* m_Class;

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity, UUID uuid);
		~ScriptInstance();
		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);
		void InvokeOn2DCollisionEnter(InternalCollision2DEvent collision);
		void InvokeOn2DCollisionExit(InternalCollision2DEvent collision);
		void InvokeOnKeyPress(int keycode);
		void InvokeOnKeyRelease(int keycode);
		void InvokeOnMouseButtonPress(int button);
		void InvokeOnMouseButtonRelease(int button);
		void InvokeOnScroll(double xOffset, double yOffset);
		
		Ref<ScriptClass> GetScriptClass() const { return m_ScriptClass; }

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			static_assert(sizeof(T) <= 16, "Type is too large");

			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);
			if (!success)
				return T();

			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const std::string& name, const T& value)
		{
			static_assert(sizeof(T) <= 16, "Type is too large");

			SetFieldValueInternal(name, &value);
		}

		MonoObject* GetManagedObject() const { return m_Instance; }

		UUID GetUUID() const { return m_UUID; }

	private:
		bool GetFieldValueInternal(const std::string& name, void* buffer);
		bool SetFieldValueInternal(const std::string& name, const void* value);
	private:
		UUID m_UUID;
		UUID m_EntityUUID;
		Ref<ScriptClass> m_ScriptClass;

		uint32_t m_InstanceHandle = 0;
		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;
		MonoMethod* m_OnCollisionEnterMethod = nullptr;
		MonoMethod* m_OnCollisionExitMethod = nullptr;
		MonoMethod* m_OnKeyPress = nullptr;
		MonoMethod* m_OnKeyRelease = nullptr;
		MonoMethod* m_OnMouseButtonPress = nullptr;
		MonoMethod* m_OnMouseButtonRelease = nullptr;
		MonoMethod* m_OnScroll = nullptr;
		inline static char s_FieldValueBuffer[16];

		friend class ScriptEngine;
		friend class ScriptFieldInstance;
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;
	using ScriptMethodMap = std::unordered_map<std::string, ScriptMethod>;

	class ScriptEngine
	{
	public:

		static void Init();
		static void Shutdown();

		static bool LoadAssembly(const std::filesystem::path& filepath);
		static bool LoadAppAssembly(const std::filesystem::path& filepath);

		static void ReloadAssembly();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool EntityClassExists(const std::string& fullClassName);
		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, Timestep ts);
		static std::vector<MonoClass*> GetBaseClasses(MonoClass* klass, MonoClass* entityClass);

		static Scene* GetSceneContext();
		static Ref<ScriptInstance> GetEntityScriptInstance(UUID uuid, std::string name);
		static std::vector<Ref<ScriptInstance>> GetAllScriptInstances();
		static std::vector<Ref<ScriptInstance>> GetEntityScriptInstances(UUID uuid);

		static Ref<ScriptClass> GetEntityClass(const std::string& name);
		static std::unordered_map<std::string, Ref<ScriptClass>>& GetEnityClasses();
		static ScriptFieldMap& GetScriptFieldMap(Entity uuid);
		static const ScriptMethodMap& GetScriptMethodMap(const std::string& className);
		static const std::unordered_map<std::string,ScriptMethodMap>& GetAllScriptMethodMaps();

		static void CallMethod(UUID entityID, const std::string& className, const std::string& methodName, int parameterCount = 0,void** params = nullptr);

		static bool isMethodInternal(const std::string& methodName);

		static int GetMethodParameterCount(const std::string& className, const std::string& methodName);

		static MonoImage* GetCoreAssemblyImage();

		static MonoObject* GetManagedInstance(UUID uuid, std::string name);

		static MonoString* CreateString(const char* string);

		static bool IsInitialized() { return s_Initialized; }
	private:
		static bool s_Initialized;

		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		friend class ScriptClass;
		friend class ScriptGlue;
		friend class ScriptInstance;
	};

	namespace Utils
	{
		inline const char*ScriptFieldTypeToString(ScriptFieldType type)
		{
			switch (type)
			{
				case ScriptFieldType::Float: return		"float";
				case ScriptFieldType::Vector2: return	"vector2";
				case ScriptFieldType::Vector3: return	"vector3";	
				case ScriptFieldType::Vector4: return	"vector4";
				case ScriptFieldType::Int32: return		"int";
				case ScriptFieldType::UInt32: return		"uint";
				case ScriptFieldType::Int64: return		"int64";
				case ScriptFieldType::Bool: return		"bool";
				case ScriptFieldType::Double: return	"double";
				case ScriptFieldType::Short: return		"short";
				case ScriptFieldType::Byte: return		"byte";
				case ScriptFieldType::UShort: return	"ushort";
				case ScriptFieldType::UInt64: return	"uint64";
				case ScriptFieldType::SByte: return		"sbyte";
				case ScriptFieldType::Char: return		"char";
				case ScriptFieldType::String: return	"string";
				case ScriptFieldType::Object: return	"object";
				case ScriptFieldType::Void: return		"void";
				case ScriptFieldType::Entity: return	"entity";
				case ScriptFieldType::None: return		"none";
			}

			//EG_CORE_ASSERT(false, "Unknown type!");
			return "<Invalid>";
		}

		inline ScriptFieldType ScriptFieldTypeFromString(std::string_view fieldType)
		{
			if (fieldType == "none")		return ScriptFieldType::None;
			if (fieldType == "float")		return ScriptFieldType::Float;
			if (fieldType == "double")		return ScriptFieldType::Double;
			if (fieldType == "bool")		return ScriptFieldType::Bool;
			if (fieldType == "char")		return ScriptFieldType::Char;
			if (fieldType == "byte")		return ScriptFieldType::Byte;
			if (fieldType == "sbyte")		return ScriptFieldType::SByte;
			if (fieldType == "short")		return ScriptFieldType::Short;
			if (fieldType == "int")			return ScriptFieldType::Int32;
			if (fieldType == "int64")		return ScriptFieldType::Int64;
			if (fieldType == "UByte")		return ScriptFieldType::UByte;
			if (fieldType == "UShort")		return ScriptFieldType::UShort;
			if (fieldType == "UInt")		return ScriptFieldType::UInt32;
			if (fieldType == "uint64")		return ScriptFieldType::UInt64;
			if (fieldType == "vector2")		return ScriptFieldType::Vector2;
			if (fieldType == "vector3")		return ScriptFieldType::Vector3;
			if (fieldType == "vector4")		return ScriptFieldType::Vector4;
			if (fieldType == "string")		return ScriptFieldType::String;
			if (fieldType == "object")		return ScriptFieldType::Object;
			if (fieldType == "void")		return ScriptFieldType::Void;
			if (fieldType == "entity")		return ScriptFieldType::Entity;

			EG_CORE_ASSERT(false, "Unknown ScriptFieldType");
			return ScriptFieldType::None;
		}
	}
	

}