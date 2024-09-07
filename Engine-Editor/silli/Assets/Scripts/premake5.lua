local MunifficRootDir = "../../../../"
include(MunifficRootDir .. "vendor/premake/premake_customization/solution_items.lua")
workspace "SILLI"
architecture "x86_64"
startproject "Game"
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
project "Game"
location "SILLI"
kind "SharedLib"
language "C#"
dotnetframework "4.7.2"
targetdir("Binaries")
objdir("Intermediates")
files
{
"SILLI/Source/**.cs",
"SILLI/Properties/**.cs",
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
include(MunifficRootDir .. "Muniffic-ScriptCore")
group ""
