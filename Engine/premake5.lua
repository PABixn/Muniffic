project "Engine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" ..outputdir.. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/"..outputdir.. "/%{prj.name}")

	pchheader "egpch.h"
	pchsource "src/egpch.cpp"

	defines "SFML_STATIC"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp",
	}

	includedirs 
	{
		"src",
		".",
		"vendor",
		"vendor/spdlog/include",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.filewatch}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.SFML}",
	}
	links
	{
		"Box2D",
		"GLFW",
		"opengl32.lib",
		"dwmapi.lib",
		"yaml-cpp",
		"Glad",
		"Imgui",
		"msdf-atlas-gen",
		"%{Library.mono}",
		--sfml static linking required libs :3
		--"sfml-audio-d-2.dll",
		--"openal32.lib",
		--"openal32.dll",
		--"flac.lib",
		--"vorbisenc.lib",
		--"vorbisfile.lib",
		--"vorbis.lib",
		--"ogg.lib",
		"sfml-audio-d.lib",
		--"sfml-system-d.lib",
		--end of sfml static linking required libs
	}
	libdirs
	{
		"%{LibraryDir.SFML}",
		"%{LibraryDir.SFMLdyn}",
	}

	defines{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	filter "files:vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		links
		{
			"%{Library.WinSock}",
			"%{Library.WinMM}",
			"%{Library.WinVersion}",
			"%{Library.BCrypt}",
		}

		defines
		{
			"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING"
		}

	filter "configurations:Debug"
		defines "EG_DEBUG"
		runtime "Debug"
		symbols "on"

        links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "EG_RELEASE"
		runtime "Release"
		optimize "on"

        links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "EG_DIST"
		runtime "Release"
		optimize "on"

        links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}