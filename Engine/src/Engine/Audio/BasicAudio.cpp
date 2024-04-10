#include "egpch.h"
#include "BasicAudio.h"

namespace eg {
	BasicAudio::BasicAudio(std::string& path) {
		m_Path = std::filesystem::path(path);
		//LoadCurrentAudio();
		//SDL_Init(SDL_INIT_AUDIO);
	}

	void BasicAudio::InitializeXAudio2() {
		std::cout << "loaded" << std::endl;
		hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr))
			EG_CORE_TRACE(hr);
		pXAudio2 = nullptr;
		if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
			EG_CORE_TRACE(hr);

		pMasterVoice = nullptr;
		if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
			EG_CORE_TRACE(hr);

	}
	HRESULT BasicAudio::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());

		DWORD dwChunkType;
		DWORD dwChunkDataSize;
		DWORD dwRIFFDataSize = 0;
		DWORD dwFileType;
		DWORD bytesRead = 0;
		DWORD dwOffset = 0;

		while (hr == S_OK)
		{
			DWORD dwRead;
			if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			switch (dwChunkType)
			{
			case fourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4;
				if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());
				break;

			default:
				if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
					return HRESULT_FROM_WIN32(GetLastError());
			}

			dwOffset += sizeof(DWORD) * 2;

			if (dwChunkType == fourcc)
			{
				dwChunkSize = dwChunkDataSize;
				dwChunkDataPosition = dwOffset;
				return S_OK;
			}

			dwOffset += dwChunkDataSize;

			if (bytesRead >= dwRIFFDataSize) return S_FALSE;

		}

		return S_OK;

	}

	HRESULT BasicAudio::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());
		DWORD dwRead;
		if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
			return HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}
	void BasicAudio::LoadCurrentAudio() {
		InitializeXAudio2();
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
		WAVEFORMATEXTENSIBLE wfx = { 0 };
		XAUDIO2_BUFFER buffer = { 0 };
#ifdef _XBOX
		char* strFileName = m_Path;
#else
		std::string audio_path = m_Path.string();
		TCHAR* strFileName = new TCHAR[audio_path.size() + 1];
		strFileName[audio_path.size()] = 0;
		std::copy(audio_path.begin(), audio_path.end(), strFileName);
#endif
		// Open the file
		HANDLE hFile = CreateFile(
			strFileName,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
			EG_CORE_TRACE(HRESULT_FROM_WIN32(GetLastError()));

		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
			EG_CORE_TRACE(HRESULT_FROM_WIN32(GetLastError()));

		DWORD dwChunkSize;
		DWORD dwChunkPosition;

		FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
		DWORD filetype;
		ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (filetype != fourccWAVE)
			EG_CORE_TRACE(S_FALSE);

		FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
		ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
		BYTE* pDataBuffer = new BYTE[dwChunkSize];
		ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

		buffer.AudioBytes = dwChunkSize;  //size of the audio buffer in bytes
		buffer.pAudioData = pDataBuffer;  //buffer containing audio data
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		if (looped) buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		//IXAudio2SourceVoice* pSourceVoice;
		if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&wfx))) EG_CORE_TRACE(hr);
		pSourceVoice->SetVolume(1.0f);
		if (FAILED(hr = pSourceVoice->SubmitSourceBuffer(&buffer)))
			EG_CORE_TRACE(hr);
		if (FAILED(hr = pSourceVoice->Start(0))) EG_CORE_TRACE(hr);
			

		


		//std::string music = "play C:/Users/mniedziolka/Downloads/arpeggiator-end-credits-wav-14644.wav";
		//std::thread t1([=]() {mciSendString(L"play C:/Users/mniedziolka/Downloads/arpeggiator-end-credits-wav-14644.wav", NULL, 0, NULL); });
		//std::thread t2([=]() {mciSendString(L"play C:/Users/mniedziolka/Desktop/Muniffic/Engine - Editor/SandboxProject/Assets/fail.wav", NULL, 0, NULL); });
		//t1.join();
		//t2.join();
		//std::thread worker([=]() { PlaySound(m_Path.c_str(), 0, SND_APPLICATION | SND_SYNC); });
		//worker.join();
		//std::cout << looped << playingFromStart << std::endl;
		//if (!looped) PlaySound(m_Path.c_str(), 0, SND_APPLICATION | SND_ASYNC);
		//else PlaySound(m_Path.c_str(), 0, SND_APPLICATION | SND_ASYNC | SND_LOOP);
		//work.join();
		//WaveMixInit()
		//mciSendString(L"play C:/Users/mniedziolka/Downloads/arpeggiator-end-credits-wav-14644.wav", NULL, 0, NULL);
		return true;
	}

	void BasicAudio::Stop() {
		pSourceVoice->Stop();
	}

	std::string BasicAudio::GetFileName() {
		return m_Path.filename().string();
	}
}