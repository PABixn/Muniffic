#include "egpch.h"
//#include "soloud_wav.h"
//#include "soloud.h"
#include "Engine/Audio/BasicAudio.h"
#include <thread>
#include "SFML/Audio.hpp"
#pragma comment(lib, "Winmm.lib")
namespace eg {
	BasicAudio::BasicAudio(std::string& path)  {
		m_Path = std::filesystem::path(path);
		LoadCurrentAudio();
	}

	void BasicAudio::LoadCurrentAudio() {
		std::string a = m_Path.string();
		const char* silli = a.c_str();
		//auto s = m_Wav.load(silli);
		//EG_CORE_TRACE("file {0} loaded with code {1}", silli, s);
	}
	void BasicAudio::OpenAudio(std::string path) {
		m_Path = std::filesystem::path(path);
		LoadCurrentAudio();
	}
	bool BasicAudio::Play() {
		sf::SoundBuffer buffer;
		/*
		LoadCurrentAudio(); 
		Ref<SoLoud::Soloud> SoLoudInstance = CreateRef<SoLoud::Soloud>(SoLoud::Soloud()); 
		(*SoLoudInstance.get()).init();
		auto s = (*SoLoudInstance.get()).play(m_Wav); 
		std::thread myThread([&]() { 
			while ((*SoLoudInstance.get()).isValidVoiceHandle(s))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			(*SoLoudInstance.get()).stop(s);
		});

		// Ensure the thread is joined before returning
		myThread.detach();

		// Deinitialize SoLoud after sound finishes
		*/
		return true;
	}
	std::string BasicAudio::GetFileName() {
		return m_Path.filename().string();
	}
}