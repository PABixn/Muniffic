#pragma once
//#include "SoLoud/include/soloud_wav.h"
//#include "SoLoud/include/soloud.h"
namespace eg {
	class BasicAudio {
		//SoLoud::Wav m_Wav;
		std::filesystem::path m_Path; 
	public:
		std::filesystem::path GetPath() { return m_Path; }
		void SetPath(std::filesystem::path val) { m_Path = val; LoadCurrentAudio(); }
		BasicAudio(std::string& path);
		BasicAudio() { 
			EG_CORE_TRACE("what");
		};
		bool Play(/*SoLoud::Soloud& SoLoudInstance*/ );
		std::string GetFileName();
		void LoadCurrentAudio();
		void OpenAudio(std::string path);
	};

}