project "Sandbox"
	kind "ConsoleApp"
	language "c++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" ..outputdir.. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/"..outputdir.. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs 
	{
		"%{wks.location}/Engine/vendor/spdlog/include",
		"%{wks.location}/Engine/src",
		"%{wks.location}/Engine/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Engine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
			"YAML_CPP_STATIC_DEFINE"
		}

	filter "configurations:Debug"
		defines "EG_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EG_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EG_DIST"
		runtime "Release"
		optimize "on"