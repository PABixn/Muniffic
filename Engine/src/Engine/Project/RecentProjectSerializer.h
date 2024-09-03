#pragma once

#include <vector>

namespace eg {

	class RecentProjectSerializer
	{
	public:
		RecentProjectSerializer();

		bool Serialize(const std::string&, const std::string&);
		bool Deserialize(const std::string&);
		void DeleteProject(int id);
		std::vector<std::string> getProjectList() { return m_ProjectList; }
	private:
		std::vector<std::string> m_ProjectList;
	};
}