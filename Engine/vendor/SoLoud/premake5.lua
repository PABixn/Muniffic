project "SoLoud"
	kind "StaticLib"
	cppdialect "c++17"
	language "C++"
	warnings "off"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.c*",
		"src/audiosource/**.c*",
		"src/filter/**.c*",
		"src/core/**.c*",
		"include/**.h"
  	}

	includedirs
	{
	  "src/**",
  		"include", 
		"vendor/SoLoud/include"
	}
	
	defines {"WITH_MINIAUDIO"}
	

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"
