local MunifficRootDir = "../../../../"

include (MunifficRootDir .. "vendor/premake/premake_customization/solution_items.lua")

workspace "Quest for the Solar Sombrero"
	architecture "x86_64"
	startproject "Quest"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

project "Quest"
	location "Quest for the Solar Sombrero"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("Binaries")
	objdir ("Intermediates")

	files
	{
		"Quest for the Solar Sombrero/Source/**.cs",
		"Quest for the Solar Sombrero/Properties/**.cs",
	}

	links
	{
		"Muniffic-ScriptCore"
	}

	filter "configurations:Debug"
		symbols "Default"
		optimize "off"
	
	filter "configurations:Release"
		optimize "on"
		symbols "Default"
	
	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

group "Muniffic"
	include (MunifficRootDir .. "Muniffic-ScriptCore")
group ""