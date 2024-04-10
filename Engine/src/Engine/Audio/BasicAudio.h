#pragma once
#include "egpch.h"
#include "Windows.h"
#include "mmsystem.h"
#include <thread>
#include <xaudio2.h>
#ifdef _XBOX //Big-Endian
#define fourccRIFF 'RIFF'
#define fourccDATA 'data'
#define fourccFMT 'fmt '
#define fourccWAVE 'WAVE'
#define fourccXWMA 'XWMA'
#define fourccDPDS 'dpds'
#endif

#ifndef _XBOX //Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'
#endif

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
		void InitializeXAudio2();
		HRESULT hr;
		IXAudio2* pXAudio2;
		IXAudio2MasteringVoice* pMasterVoice;
		IXAudio2SourceVoice* pSourceVoice;
		HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
		HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
	};

}