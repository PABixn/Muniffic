#pragma once
#include "egpch.h"
#include "Windows.h"
#include "mmsystem.h"

#pragma comment(lib, "winmm.lib")

namespace eg {
	class BasicAudio {
		std::filesystem::path m_Path; 
		bool looped = true;
		bool playingFromStart = true;
	public:
		std::filesystem::path GetPath() { return m_Path; }
		void SetPath(std::filesystem::path val) { m_Path = val; LoadCurrentAudio(); }
		BasicAudio(std::string& path);
		BasicAudio() { 
			EG_CORE_TRACE("what");
		};
		bool Play();
		bool* IsLoopedPtr() { return &looped; }
		bool* IsPlayingFromStartPtr() { return &playingFromStart; }
		bool IsPlayingFromStart() { return playingFromStart;  }
		std::string GetFileName();
		void LoadCurrentAudio();
		void OpenAudio(std::string path);
		void Stop();
		TCHAR *audio;
	};

}