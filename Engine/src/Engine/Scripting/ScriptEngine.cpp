#include "egpch.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

namespace eg {

	

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		ScriptClass* EntityClass = nullptr;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;

		//Runtime
		Scene* SceneContext = nullptr;
	};

	static ScriptEngineData* s_Data;

	namespace Utils {

		//TOOD: move this to Filesystem class
		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath.string(), &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}

	}

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();
		LoadAssembly("Resources/Scripts/Muniffic-ScriptCore.dll");
		LoadAssemblyClasses(s_Data->CoreAssembly);

		ScriptGlue::RegisterFunctions();
		s_Data->EntityClass = new ScriptClass("eg", "Entity");
#if 0
		

		// Retrieve adn instantiate class (with constructor)
		MonoObject* object = s_Data->EntityClass->Instantiate();

		// Call method
		MonoMethod* printMessageFunc = s_Data->EntityClass->GetMethod("PrintMessage", 0);
		s_Data->EntityClass->InvokeMethod(object, printMessageFunc, nullptr);
		// Call method with parameters
		MonoMethod* printMessageFunc2 = s_Data->EntityClass->GetMethod("PrintMessage", 1);
		void* args[1];
		args[0] = mono_string_new(s_Data->AppDomain, "Hello from C++");
		s_Data->EntityClass->InvokeMethod(object, printMessageFunc2, args);

		MonoMethod* printMessageFunc3 = s_Data->EntityClass->GetMethod("PrintInts", 2);
		void* args2[2];
		int val = 5;
		int val2 = 10;
		args2[0] = &val;
		args2[1] = &val2;

		s_Data->EntityClass->InvokeMethod(object, printMessageFunc3, args2);
#endif
	}

	void ScriptEngine::Shutdown()
	{
		delete s_Data;
		ShutdownMono();
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* domain = mono_jit_init("MunifficJitRuntime");
		if (!domain)
		{
			EG_CORE_ERROR("Could not initialize Mono");
			return;
		}

		s_Data->RootDomain = domain;
	}

	void ScriptEngine::ShutdownMono()
	{
		s_Data->AppDomain = nullptr;
		s_Data->RootDomain = nullptr;
	}

	void ScriptEngine::LoadAssemblyClasses(MonoAssembly* assembly)
	{
		s_Data->EntityClasses.clear();

		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		MonoClass* entityClass = mono_class_from_name(image, "eg", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, name);
			else
				fullName = name;

			MonoClass* monoClass = mono_class_from_name(image, nameSpace, name);

			if (monoClass == entityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);

			if (isEntity)
			{
				s_Data->EntityClasses[fullName] = CreateRef<ScriptClass>(nameSpace, name);
			}
			
			EG_CORE_TRACE("{}.{}", nameSpace, name);
		}
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{

		s_Data->AppDomain = mono_domain_create_appdomain((char*)"MunifficScriptRuntime", nullptr);

		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath.string());
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;
		s_Data->EntityInstances.clear();
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullClassName)
	{
		return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& nsc = entity.GetComponent<ScriptComponent>();
		if (ScriptEngine::EntityClassExists(nsc.Name))
		{
			Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->EntityClasses[nsc.Name], entity);
			s_Data->EntityInstances[entity.GetUUID()] = instance;
			instance->InvokeOnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, Timestep ts)
	{
		UUID entityID = entity.GetUUID();
		EG_CORE_ASSERT(s_Data->EntityInstances.find(entityID) != s_Data->EntityInstances.end(), "Entity has no script instance!");
		Ref<ScriptInstance> instance = s_Data->EntityInstances[entityID];
		instance->InvokeOnUpdate(ts);
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	std::unordered_map<std::string, Ref<ScriptClass>>& ScriptEngine::GetEnityClasses()
	{
		return s_Data->EntityClasses;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* object = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(object);
		return object;
	}
	

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className) 
		: m_ClassNamespace(classNamespace), m_ClassName(className) {
		m_Class = mono_class_from_name(s_Data->CoreAssemblyImage, m_ClassNamespace.c_str(), m_ClassName.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_Class);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_Class, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		: m_ScriptClass(scriptClass)
	{
		m_Instance = m_ScriptClass->Instantiate();

		m_Constructor = s_Data->EntityClass->GetMethod(".ctor", 1);
		m_OnCreateMethod = m_ScriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = m_ScriptClass->GetMethod("OnUpdate", 1);

		// Calling entity constructor
		{
			UUID uuid = entity.GetUUID();
			void* arg = &uuid;
			m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &arg);
		}
	}

	void ScriptInstance::InvokeOnCreate()
	{
		m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		void* arg = &ts;
		m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &arg);
	}

}