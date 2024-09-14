#include "egpch.h"
#include "Engine/Core/Application.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"
#include "Engine/Core/Buffer.h"
#include "Engine/Core/FileSystem.h"
#include "Engine/Project/Project.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
#include "mono/metadata/attrdefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"

#include "Filewatch.h"

#include "../Engine-Editor/src/Panels/ConsolePanel.h"

namespace eg
{

	static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
		{

			{"System.Byte", ScriptFieldType::Byte},
			{"System.Int16", ScriptFieldType::Short},
			{"System.UInt16", ScriptFieldType::UShort},
			{"System.SByte", ScriptFieldType::SByte},
			{"System.Char", ScriptFieldType::Char},
			{"System.UInt32", ScriptFieldType::UInt32},
			{"System.Int32", ScriptFieldType::Int32},
			{"System.UInt64", ScriptFieldType::UInt64},
			{"System.Int64", ScriptFieldType::Int64},
			{"System.Single", ScriptFieldType::Float},
			{"System.Double", ScriptFieldType::Double},
			{"System.Boolean", ScriptFieldType::Bool},
			{"System.String", ScriptFieldType::String},
			{"System.Object", ScriptFieldType::Object},
			{"System.Void", ScriptFieldType::Void},
			{"eg.Vector2", ScriptFieldType::Vector2},
			{"eg.Vector3", ScriptFieldType::Vector3},
			{"eg.Vector4", ScriptFieldType::Vector4},
			{"eg.Entity", ScriptFieldType::Entity},
			//{ "eg.TransformComponent", ScriptFieldType::TransformComponent },
			//{ "eg.RigidBodyComponent", ScriptFieldType::RigidBodyComponent },
	};

	struct ScriptEngineData
	{
		MonoDomain *RootDomain = nullptr;
		MonoDomain *AppDomain = nullptr;

		MonoAssembly *CoreAssembly = nullptr;
		MonoImage *CoreAssemblyImage = nullptr;

		MonoAssembly *AppAssembly = nullptr;
		MonoImage *AppAssemblyImage = nullptr;

		std::filesystem::path CoreAssemblyPath;
		std::filesystem::path AppAssemblyPath;

		ScriptClass EntityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, std::unordered_map<std::string, Ref<ScriptInstance>>> EntityInstances;

		using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;
		std::unordered_map<UUID, ScriptFieldMap> EntityFields;

		Scope<filewatch::FileWatch<std::string>> AppAssemblyFileWatcher;
		bool AssemblyReloadPending = false;

		bool EnableDebugging = false;

		// Runtime
		Scene *SceneContext = nullptr;
	};

	static ScriptEngineData *s_Data;

	namespace Utils
	{

		// TOOD: move this to Filesystem class

		MonoAssembly* LoadMonoAssembly(const std::filesystem::path &assemblyPath, bool loadPDB = false)
		{
			ScopedBuffer fileData = FileSystem::ReadFileBinary(assemblyPath.string());

			if (!fileData)
			{
				// Log some error message using the errorMessage data
				return nullptr;
			}

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData.As<char>(), fileData.Size(), 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char *errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			MonoAssembly *assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
			mono_image_close(image);

			return assembly;
		}

		ScriptFieldType MonoTypeToScriptFieldType(MonoType *monoType)
		{
			std::string typeName = mono_type_get_name(monoType);

			auto it = s_ScriptFieldTypeMap.find(typeName);
			if (it == s_ScriptFieldTypeMap.end())
			{
				return ScriptFieldType::None;
			}
			return it->second;
		}

	}

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();
		bool status = LoadAssembly("Resources/Scripts/Debug/Muniffic-ScriptCore.dll");
		if (!status)
		{
			EG_CORE_ERROR("Failed to load core assembly!");
			return;
		}
		auto scriptModulePath = Project::GetProjectDirectory() / Project::GetAssetDirectory() / Project::GetActive()->GetScriptModulePath();
		status = LoadAppAssembly(scriptModulePath);
		if (!status)
		{
			EG_CORE_ERROR("Failed to load app assembly!");
			return;
		}
		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterFunctions();

		s_Data->EntityClass = ScriptClass("eg", "DefaultBehaviour", true);
	}

	void ScriptEngine::Shutdown()
	{
		delete s_Data;
		ShutdownMono();
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain *domain = mono_jit_init("MunifficJitRuntime");
		EG_CORE_ASSERT(domain, "Could not initialize Mono");

		s_Data->RootDomain = domain;
	}

	void ScriptEngine::ShutdownMono()
	{
		if (s_Data == nullptr)return; //Means Mono hasn't been initialized yet (Closed app before choosing project)

		// mono_domain_set(mono_get_root_domain(), false);

		// mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;

		// mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_Data->EntityClasses.clear();

		// MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo *typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "eg", "DefaultBehaviour");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char *nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char *className = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);
			std::string fullName;
			if (strlen(nameSpace) != 0)
				fullName = fmt::format("{}.{}", nameSpace, className);
			else
				fullName = className;

			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, className);
			

			if (monoClass == entityClass)
				continue;

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);

			if (!isEntity)
				continue;

			std::vector<MonoClass*> baseClasses = GetBaseClasses(monoClass, entityClass);

			Ref<ScriptClass> scriptClass = CreateRef<ScriptClass>(nameSpace, className);
			s_Data->EntityClasses[fullName] = scriptClass;

			int fieldCount = mono_class_num_fields(monoClass);
			EG_CORE_WARN("Fields of class: {}, with: {} fields", fullName, fieldCount);
			void *iterator = nullptr;
			while (MonoClassField *field = mono_class_get_fields(monoClass, &iterator))
			{
				const char *fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);

				EG_CORE_WARN("{} flags: {}", fieldName, flags);
				if (flags & MONO_FIELD_ATTR_PUBLIC)
				{
					MonoType *monoType = mono_field_get_type(field);

					ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(monoType);
					const char *MunifficFieldType = Utils::ScriptFieldTypeToString(fieldType);

					EG_CORE_WARN("{} type: {}", fieldName, MunifficFieldType);
					scriptClass->m_Fields[fieldName] = {fieldName, fieldType, field};
				}
			}

			for (MonoClass* baseClass : baseClasses)
			{
				if (baseClass != nullptr)
				{
					int fieldCount = mono_class_num_fields(baseClass);
					EG_CORE_WARN("Fields of class: {}, with: {} fields", mono_class_get_name(baseClass), fieldCount);
					void* iterator = nullptr;
					while (MonoClassField* field = mono_class_get_fields(baseClass, &iterator))
					{
						const char* fieldName = mono_field_get_name(field);
						uint32_t flags = mono_field_get_flags(field);

						EG_CORE_WARN("{} flags: {}", fieldName, flags);
						if (flags & MONO_FIELD_ATTR_PUBLIC)
						{
							MonoType* monoType = mono_field_get_type(field);

							ScriptFieldType fieldType = Utils::MonoTypeToScriptFieldType(monoType);
							const char* MunifficFieldType = Utils::ScriptFieldTypeToString(fieldType);

							EG_CORE_WARN("{} type: {}", fieldName, MunifficFieldType);
							scriptClass->m_Fields[fieldName] = { fieldName, fieldType, field };
						}
					}
				}
			}

			EG_CORE_TRACE("{}.{}", nameSpace, className);
		}

		// mono_field_get_value
	}

	std::vector<MonoClass*> ScriptEngine::GetBaseClasses(MonoClass* monoClass, MonoClass* entityClass)
	{
		std::vector<MonoClass*> baseClasses;

		MonoClass* baseClass = mono_class_get_parent(monoClass);

		while (baseClass != nullptr && mono_class_get_name(baseClass) != mono_class_get_name(entityClass))
		{
			baseClasses.push_back(baseClass);
			baseClass = mono_class_get_parent(baseClass);
		}
		return baseClasses;
	}

	ScriptFieldMap& ScriptEngine::GetScriptFieldMap(Entity entity)
	{
		EG_CORE_ASSERT(entity);

		UUID uuid = entity.GetUUID();
		// EG_CORE_ASSERT(s_Data->EntityFields.find(uuid) != s_Data->EntityFields.end(), "Entity has no script fields!");

		// if(s_Data->EntityFields.find(uuid) == s_Data->EntityFields.end())
		// s_Data->EntityFields[uuid] = ScriptFieldMap();

		return s_Data->EntityFields[uuid];
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}

	MonoObject* ScriptEngine::GetManagedInstance(UUID uuid, std::string name)
	{
		EG_CORE_ASSERT(s_Data->EntityInstances.find(uuid) != s_Data->EntityInstances.end());
		EG_CORE_ASSERT(s_Data->EntityInstances.at(uuid).find(name) != s_Data->EntityInstances.at(uuid).end());

		return s_Data->EntityInstances.at(uuid).at(name)->GetManagedObject();
	}

	static void onAppAssemblyFileSystemEvent(const std::string &path, const filewatch::Event change_type)
	{
		if (!s_Data->AssemblyReloadPending && change_type == filewatch::Event::modified)
		{
			s_Data->AssemblyReloadPending = true;
			Application::Get().SubmitToMainThread([]()
												  {
				s_Data->AppAssemblyFileWatcher.reset();
				ScriptEngine::ReloadAssembly(); });
		}
	}

	bool ScriptEngine::LoadAssembly(const std::filesystem::path &filepath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain((char*)"MunifficScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);
		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath.string(), s_Data->EnableDebugging);
		if (!s_Data->CoreAssembly)
			return false;

		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);

		s_Data->CoreAssemblyPath = filepath;
		return true;
	}

	bool ScriptEngine::LoadAppAssembly(const std::filesystem::path &filepath)
	{
		s_Data->AppAssemblyPath = filepath;
		if (!std::filesystem::exists(filepath)) return false;
		s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath.string(), s_Data->EnableDebugging);
		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);

		if (!s_Data->AppAssembly)
			return false;


		s_Data->AppAssemblyFileWatcher = CreateScope<filewatch::FileWatch<std::string>>(
			filepath.string(),
			onAppAssemblyFileSystemEvent);
		s_Data->AssemblyReloadPending = false;
		return true;
	}

	void ScriptEngine::ReloadAssembly()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);

		LoadAssembly(s_Data->CoreAssemblyPath);
		std::filesystem::path fajlpath = s_Data->AppAssemblyPath;
		LoadAppAssembly(fajlpath);
		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();

		s_Data->EntityClass = ScriptClass("eg", "DefaultBehaviour", true);
	}

	void ScriptEngine::OnRuntimeStart(Scene *scene)
	{
		s_Data->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;
		s_Data->EntityInstances.clear();
	}

	bool ScriptEngine::EntityClassExists(const std::string &fullClassName)
	{
		return s_Data->EntityClasses.find(fullClassName) != s_Data->EntityClasses.end();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		UUID uuid = entity.GetUUID();
		const auto &nsc = entity.GetComponent<ScriptComponent>();

		s_Data->EntityInstances[uuid] = std::unordered_map<std::string, Ref<ScriptInstance>>();

		for (UUID scriptUUID : nsc.Scripts)
		{
			if(!ResourceDatabase::IsScriptEnabled(scriptUUID))
				continue;

			std::string scriptName = ResourceDatabase::GetResourceName(scriptUUID);

			if (ScriptEngine::EntityClassExists(scriptName))
			{
				Ref<ScriptInstance> instance = CreateRef<ScriptInstance>(s_Data->EntityClasses[scriptName], entity, scriptUUID);

				// Copy field values

				//Change scriptName to scriptUUID
				s_Data->EntityInstances.at(uuid)[scriptName] = instance;

				if (s_Data->EntityFields.find(uuid) != s_Data->EntityFields.end())
				{
					const ScriptFieldMap& fieldMap = s_Data->EntityFields.at(uuid);
					for (const auto& [name, fieldInstance] : fieldMap)
						instance->SetFieldValueInternal(name, fieldInstance.m_Buffer);
				}

				instance->InvokeOnCreate();
			}
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, Timestep ts)
	{
		UUID entityID = entity.GetUUID();
		if (s_Data->EntityInstances.find(entityID) != s_Data->EntityInstances.end())
		{
			for (auto& [key, value] : s_Data->EntityInstances.at(entityID))
				if(ResourceDatabase::IsScriptEnabled(value->GetUUID()) && s_Data->SceneContext->EntityExists(entityID))
					value->InvokeOnUpdate(ts);
			/*Ref<ScriptInstance> instance = s_Data->EntityInstances[entityID];
			instance->InvokeOnUpdate(ts);*/
		}
		else
		{
			EG_CORE_ERROR("Couldn't find script instance for entity {}!", entityID);
		}
	}

	Scene *ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	std::vector<Ref<ScriptInstance>> ScriptEngine::GetAllScriptInstances()
	{
		if (s_Data == nullptr) return std::vector<Ref<ScriptInstance>>();
		std::vector<Ref<ScriptInstance>> result;
		for (auto& [key, value] : s_Data->EntityInstances)
			for (auto& [key, value] : value)
				result.push_back(value);
		return result;
	}

	std::vector<Ref<ScriptInstance>> ScriptEngine::GetEntityScriptInstances(UUID uuid)
	{
		std::vector<Ref<ScriptInstance>> result;
		if (s_Data->EntityInstances.find(uuid) != s_Data->EntityInstances.end())
		{
			for (auto& [key, value] : s_Data->EntityInstances.at(uuid))
				result.push_back(value);
		}
		return result;
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID uuid, std::string name)
	{
		auto it = s_Data->EntityInstances.find(uuid);
		if (it == s_Data->EntityInstances.end())
			return nullptr;
		if(it->second.find(name) == it->second.end())
			return nullptr;
		return it->second.at(name);
	}

	Ref<ScriptClass> ScriptEngine::GetEntityClass(const std::string &name)
	{
		ScriptEngineData* data = s_Data;
		if (s_Data->EntityClasses.find(name) == s_Data->EntityClasses.end())
			return nullptr;
		return s_Data->EntityClasses.at(name);
	}

	std::unordered_map<std::string, Ref<ScriptClass>> &ScriptEngine::GetEnityClasses()
	{
		return s_Data->EntityClasses;
	}

	MonoObject *ScriptEngine::InstantiateClass(MonoClass *monoClass)
	{
		MonoObject *object = mono_object_new(s_Data->AppDomain, monoClass);
		
		mono_runtime_object_init(object);
		return object;
	}

	ScriptClass::ScriptClass(const std::string &classNamespace, const std::string &className, bool isCore)
		: m_ClassNamespace(classNamespace), m_ClassName(className), m_Fields(std::unordered_map<std::string, ScriptField>())
	{
		m_Class = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, m_ClassNamespace.c_str(), m_ClassName.c_str());
	}

	MonoObject *ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_Class);
	}

	MonoMethod *ScriptClass::GetMethod(const std::string &name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_Class, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void **params)
	{
		MonoObject *exception = nullptr;
		return mono_runtime_invoke(method, instance, params, &exception);
	}

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity, UUID uuid)
		: m_ScriptClass(scriptClass)
	{
		m_Instance = m_ScriptClass->Instantiate();
		m_InstanceHandle = mono_gchandle_new(m_Instance, true);

		m_UUID = uuid;
		m_EntityUUID = entity.GetUUID();

		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = m_ScriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = m_ScriptClass->GetMethod("OnUpdate", 1);
		m_OnCollisionEnterMethod = m_ScriptClass->GetMethod("OnCollisionEnter", 1);
		m_OnCollisionExitMethod = m_ScriptClass->GetMethod("OnCollisionExit", 1);
		m_OnKeyPress = m_ScriptClass->GetMethod("OnKeyPress", 1);
		m_OnKeyRelease = m_ScriptClass->GetMethod("OnKeyRelease", 1);
		m_OnMouseButtonPress = m_ScriptClass->GetMethod("OnMouseButtonPress", 1);
		m_OnMouseButtonRelease = m_ScriptClass->GetMethod("OnMouseButtonRelease", 1);
		m_OnScroll = m_ScriptClass->GetMethod("OnScroll", 1);

		{
			UUID uuid = entity.GetUUID();
			void *arg = &uuid;
			m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &arg);
		}
	}

	ScriptInstance::~ScriptInstance()
	{
		mono_gchandle_free(m_InstanceHandle);
	}

	MonoString* ScriptEngine::CreateString(const char* string)
	{
		return mono_string_new(s_Data->AppDomain, string);
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if (m_OnCreateMethod)
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
	}

	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		if (m_OnUpdateMethod)
		{
			void *arg = &ts;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &arg);
		}
	}

	void ScriptInstance::InvokeOn2DCollisionEnter(InternalCollision2DEvent collision)
	{
		if (m_OnCollisionEnterMethod)
		{
			UUID uuid = m_EntityUUID == collision.entityA ? collision.entityB : collision.entityA;
			Collision2D* args = new Collision2D(uuid, collision.contactPoints, collision.friction, collision.restitution, collision.tangentSpeed);
			void* arg = args;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCollisionEnterMethod, &arg);
		}
	}

	void ScriptInstance::InvokeOn2DCollisionExit(InternalCollision2DEvent collision)
	{
		if (m_OnCollisionExitMethod)
		{
			UUID uuid = m_EntityUUID == collision.entityA ? collision.entityB : collision.entityA;
			Collision2D* args = new Collision2D(uuid, collision.contactPoints, collision.friction, collision.restitution, collision.tangentSpeed);
			void* arg = args;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCollisionExitMethod, &arg);
		}
	}

	void ScriptInstance::InvokeOnKeyPress(int keycode)
	{
		if (m_OnKeyPress)
		{
			void* args = &keycode;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnKeyPress, &args);
		}
	}

	void ScriptInstance::InvokeOnKeyRelease(int keycode)
	{
		if (m_OnKeyRelease)
		{
			void* args = &keycode;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnKeyRelease, &args);
		}
	}

	void ScriptInstance::InvokeOnMouseButtonPress(int button)
	{
		if (m_OnMouseButtonPress)
		{
			void* args = &button;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnMouseButtonPress, &args);
		}
	}

	void ScriptInstance::InvokeOnMouseButtonRelease(int button)
	{
		if (m_OnMouseButtonRelease)
		{
			void* args = &button;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnMouseButtonRelease, &args);
		}
	}

	void ScriptInstance::InvokeOnScroll(double xOffset, double yOffset)
	{
		if (m_OnScroll)
		{
			float offsets[] = { (float)xOffset, (float)yOffset };
			void* args = &offsets;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnMouseButtonRelease, &args);
		}
	}

	ScriptField::ScriptField(const std::string &name, ScriptFieldType type, MonoClassField *valuePtr)
		: Name(name), Type(type), ClassField(valuePtr)
	{
	}

	bool ScriptInstance::GetFieldValueInternal(const std::string &name, void *buffer)
	{
		const auto &fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField &field = it->second;
		mono_field_get_value(m_Instance, field.ClassField, buffer);
		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string &name, const void *value)
	{
		const auto &fields = m_ScriptClass->GetFields();
		auto it = fields.find(name);
		if (it == fields.end())
			return false;

		const ScriptField &field = it->second;
		mono_field_set_value(m_Instance, field.ClassField, (void *)value);
		return true;
	}
}