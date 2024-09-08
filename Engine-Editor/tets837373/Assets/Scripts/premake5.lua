local MunifficRootDir = "../../../../"
include(MunifficRootDir .. "vendor/premake/premake_customization/solution_items.lua")
workspace "tets837373"
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
location "tets837373"
kind "SharedLib"
language "C#"
dotnetframework "4.7.2"
targetdir("Binaries")
objdir("Intermediates")
files
{
"tets837373/Source/**.cs",
"tets837373/Properties/**.cs",
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
