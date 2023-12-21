project "Muniffic-ScriptCore"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/Engine-Editor/Resources/Scripts")
	objdir ("%{wks.location}/Engine-Editor/Resources/Scripts/Intermediates")

	files
	{
		"Source/**.cs",
		"Properties/**.cs",
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

	