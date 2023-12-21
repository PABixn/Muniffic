#include "egpch.h"
#include "ScriptEngine.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

namespace eg {

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
	};

	static ScriptEngineData* s_Data;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();
	}

	void ScriptEngine::Shutdown()
	{
		delete s_Data;
		ShutdownMono();
	}

	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
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

	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

		if (status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
			// Log some error message using the errorMessage data
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		// Don't forget to free the file data
		delete[] fileData;

		return assembly;
	}

	void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			EG_CORE_TRACE("{}.{}", nameSpace, name);
		}
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
		s_Data->AppDomain = mono_domain_create_appdomain((char*)"MunifficScriptRuntime", nullptr);

		mono_domain_set(s_Data->AppDomain, true);
		
		s_Data->CoreAssembly = LoadCSharpAssembly("Resources/Scripts/Muniffic-ScriptCore.dll");
		PrintAssemblyTypes(s_Data->CoreAssembly);

		

		MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		MonoClass* monoClass = mono_class_from_name(assemblyImage, "eg", "Main");

		// 1. create an object (and call constructor)
		MonoObject* object = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(object);

		// 2. call function
		MonoMethod* printMessageFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		mono_runtime_invoke(printMessageFunc, object, nullptr, nullptr);
		// 3. call function with parameters
		MonoMethod* printMessageFunc2 = mono_class_get_method_from_name(monoClass, "PrintMessage", 1);
		void* args[1];
		args[0] = mono_string_new(s_Data->AppDomain, "Hello from C++");
		mono_runtime_invoke(printMessageFunc2, object, args, nullptr);

		MonoMethod* printMessageFunc3 = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
		void* args2[2];
		int val = 5;
		int val2 = 10;
		args2[0] = &val;
		args2[1] = &val2;

		mono_runtime_invoke(printMessageFunc3, object, args2, nullptr);
		

	}

	void ScriptEngine::ShutdownMono()
	{
		s_Data->AppDomain = nullptr;
		s_Data->RootDomain = nullptr;
	}

}