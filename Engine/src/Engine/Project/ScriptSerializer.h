#pragma once


namespace eg {

	class ScriptSerializer
	{
	public:
		ScriptSerializer();

		static bool Serialize(const std::string&, const std::string&);
	};
}