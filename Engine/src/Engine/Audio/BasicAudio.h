#pragma once
namespace eg {
	class BasicAudio {
		bool m_IsGood;
		std::filesystem::path m_Path;
	public:
		std::filesystem::path GetPath() { return m_Path; }
		void SetPath(std::filesystem::path val) { m_Path = val; OpenCurrentAudio(); }
		BasicAudio(std::string& path);
		BasicAudio() { 
			EG_CORE_TRACE("are you silli?");
		};
		bool Play();
		std::string GetFileName();
		void OpenCurrentAudio();
		void OpenAudio(std::string path);
	};

}