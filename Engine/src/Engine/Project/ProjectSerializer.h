#pragma once

#include "Project.h"

namespace eg {

	class ProjectSerializer
	{
	public:
		ProjectSerializer(Ref<Project> project);

		bool Serialize(const std::filesystem::path& path);
		bool Deserialize(const std::filesystem::path& path);
	private:
		Ref<Project> m_Project;
	};
}