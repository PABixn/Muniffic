#include "egpch.h"
#include "BasicAudio.h"

namespace eg {
	BasicAudio::BasicAudio(std::string& path) {
		m_Path = std::filesystem::path(path);
		//LoadCurrentAudio();
	}

	void BasicAudio::LoadCurrentAudio() {
		//audio = TEXT("play C:/Users/mniedziolka/Downloads/arpeggiator-end-credits-wav-14644.wav");
		//const char * command = "play " + path;
		//audio = new TCHAR[path.size() + 1];
		//audio[path.size()] = 0;
		//std::copy(path.begin(), path.end(), audio);
		//EG_CORE_TRACE(audio);
		//std::wstring stemp = std::wstring(path.begin(), path.end());
		//_tcscpy(audio, path);
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
		//std::string music = "play C:/Users/mniedziolka/Downloads/arpeggiator-end-credits-wav-14644.wav";
		//std::thread t1([=]() {mciSendString(L"play C:/Users/mniedziolka/Downloads/arpeggiator-end-credits-wav-14644.wav", NULL, 0, NULL); });
		//std::thread t2([=]() {mciSendString(L"play C:/Users/mniedziolka/Desktop/Muniffic/Engine - Editor/SandboxProject/Assets/fail.wav", NULL, 0, NULL); });
		//t1.join();
		//t2.join();
		//std::thread worker([=]() { PlaySound(m_Path.c_str(), 0, SND_APPLICATION | SND_SYNC); });
		//worker.join();
		//std::cout << looped << playingFromStart << std::endl;
		if (!looped) PlaySound(m_Path.c_str(), 0, SND_APPLICATION | SND_ASYNC);
		else PlaySound(m_Path.c_str(), 0, SND_APPLICATION | SND_ASYNC | SND_LOOP);
		//work.join();
		//WaveMixInit()
		//mciSendString(L"play C:/Users/mniedziolka/Downloads/arpeggiator-end-credits-wav-14644.wav", NULL, 0, NULL);
		return true;
	}

	void BasicAudio::Stop() {
		PlaySound(0, 0, 0);
	}

	std::string BasicAudio::GetFileName() {
		return m_Path.filename().string();
	}
}