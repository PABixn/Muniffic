#include "egpch.h"
#include "BasicAudio.h"

namespace eg {
	BasicAudio::BasicAudio(std::string& path)  {
		m_Path = std::filesystem::path(path);
		//LoadCurrentAudio();
	}

	void BasicAudio::LoadCurrentAudio() {
		//std::wstring stemp = std::wstring(m_Path.string().begin(), m_Path.string().end());
		//sound_path = stemp.c_str();
		//auto s = m_Wav.load(silli);
		//EG_CORE_TRACE("file {0} loaded with code {1}", silli, s);
	}
	void BasicAudio::OpenAudio(std::string path) {
		m_Path = std::filesystem::path(path);
		LoadCurrentAudio();
	}
	bool BasicAudio::Play() {
		PlaySound(m_Path.c_str(), 0, SND_APPLICATION | SND_SYNC);
		
		return true;
	}
	std::string BasicAudio::GetFileName() {
		return m_Path.filename().string();
	}
}