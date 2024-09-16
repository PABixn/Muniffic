#include "egpch.h"
#include "ScriptSerializer.h"

#include <fstream>

namespace eg
{
	 ScriptSerializer::ScriptSerializer()
	{

	}

	 bool ScriptSerializer::Serialize(const std::string& path, const std::string& name)
	{/*
		std::ofstream myfile("..\\Engine-Editor\\" + path + "\\premake5.lua");
		myfile << "local MunifficRootDir = \"../../../../\"\n";
		myfile << "include(MunifficRootDir .. \"vendor/premake/premake_customization/solution_items.lua\")\n";
		myfile << "workspace \""<<name<<"\"\n";
		

		myfile << "architecture \"x86_64\"\n";
		myfile << "startproject \"Game\"\n";
		myfile << "configurations\n";
		myfile << "{\n";
		myfile << "\"Debug\",\n";
		myfile << "\"Release\",\n";
		myfile << "\"Dist\"\n";
		myfile << "}\n";

		myfile << "flags\n";
		myfile << "{\n";
		myfile << "\"MultiProcessorCompile\"\n";
		myfile << "}\n";
		myfile << "project \"Game\"\n";
		myfile << "location \""<<name<<"\"\n";
		myfile << "kind \"SharedLib\"\n";
		myfile << "language \"C#\"\n";
		myfile << "dotnetframework \"4.7.2\"\n";

		myfile << "targetdir(\"Binaries\")\n";
		myfile << "objdir(\"Intermediates\")\n";

		myfile << "files\n";
		myfile << "{\n";
		myfile << "\""<<name<<"/Source/**.cs\",\n";
		myfile << "\""<<name<< "/Properties/**.cs\",\n";
		myfile << "}\n";

		myfile << "links\n";
		myfile << "{\n";
		myfile << "\"Muniffic-ScriptCore\"\n";
		myfile << "}\n";

		myfile << "filter \"configurations:Debug\"\n";
		myfile << "symbols \"Default\"\n";
		myfile << "optimize \"off\"\n";

		myfile << "filter \"configurations:Release\"\n";
		myfile << "optimize \"on\"\n";
		myfile << "symbols \"Default\"\n";

		myfile << "filter \"configurations:Dist\"\n";
		myfile << "optimize \"Full\"\n";
		myfile << "symbols \"Off\"\n";

		myfile << "group \"Muniffic\"\n";
		myfile << "include(MunifficRootDir .. \"Muniffic-ScriptCore\")\n";
		myfile << "group \"\"\n";


		myfile.close();

		std::ofstream batFile("..\\Engine-Editor\\" + path + "\\Win-GenProjects.bat");
		batFile << "@echo off\n";
		batFile << "cd "+path+"\n";
		batFile << "call ..\\..\\..\\. .\\vendor\\premake\\bin\\premake5.exe vs2022\n";
		batFile.close();

		std::string command = path+"\\Win-GenProjects.bat";
		std::system(command.c_str());
		*/
		return true;
	}

	
}