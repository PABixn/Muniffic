#include "egpch.h"
#include "Engine/Audio/BasicAudio.h"
#include <Mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "Winmm.lib")
namespace eg {
	BasicAudio::BasicAudio(std::string& path) {
		m_Path = std::filesystem::path(path);
		OpenCurrentAudio();
	}
	void BasicAudio::OpenCurrentAudio() {
		std::string message = "open \"";
		message.append(m_Path.string());
		message.append("\" type mpegvideo alias mp3");
		std::wstring stemp = std::wstring(message.begin(), message.end());
		LPCWSTR sw = stemp.c_str();
		m_IsGood = mciSendString(sw, NULL, 0, NULL) == 0 ? true : false;
	}
	void BasicAudio::OpenAudio(std::string path) {
		m_Path = std::filesystem::path(path);
		OpenCurrentAudio();
	}
	bool BasicAudio::Play() {
		OpenCurrentAudio();
		auto s = mciSendString(L"play mp3 from 0", NULL, 0, NULL) == 0;
		return s;
	}
	std::string BasicAudio::GetFileName() {
		return m_Path.filename().string();
	}
}