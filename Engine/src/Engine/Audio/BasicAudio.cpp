#include "egpch.h"
#include "soloud_wav.h"
#include "soloud.h"
#include "Engine/Audio/BasicAudio.h"
#include <thread>

#pragma comment(lib, "Winmm.lib")
namespace eg {
	BasicAudio::BasicAudio(std::string& path)  {
		m_Path = std::filesystem::path(path);
		LoadCurrentAudio();
		m_Wav.load(path.c_str());
	}

	void BasicAudio::LoadCurrentAudio() {
		std::string a = m_Path.string();
		const char* silli = a.c_str();
		m_Wav.load(silli);
	}
	void BasicAudio::OpenAudio(std::string path) {
		m_Path = std::filesystem::path(path);
		LoadCurrentAudio();
	}
	bool BasicAudio::Play() {
		LoadCurrentAudio();
		 SoLoud::Soloud SoLoudInstance =  SoLoud::Soloud();
		//SoLoudInstance.reset();
		SoLoudInstance.init();
		/*
		std::thread myThread([&]() {
		auto ss = */SoLoudInstance.play(m_Wav);/*
			while (SoLoudInstance.isValidVoiceHandle(ss))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		});
		myThread.detach();*/
		//SoLoudInstance.deinit();
		return true;
	}
	std::string BasicAudio::GetFileName() {
		return m_Path.filename().string();
	}
}