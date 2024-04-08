include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Muniffic"
	architecture "x86_64"
	startproject "Engine-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Engine/vendor/Box2D"
	include "Engine/vendor/GLFW"
	include "Engine/vendor/Glad"
	include "Engine/vendor/imgui"
	include "Engine/vendor/msdf-atlas-gen"
	include "Engine/vendor/yaml-cpp"
group ""

group "Core"
	include "Engine"
	include "Muniffic-ScriptCore"
group ""

group "Tools"
	include "Engine-Editor"
group ""
