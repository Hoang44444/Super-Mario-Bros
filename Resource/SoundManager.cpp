	#include "SoundManager.h"
	#include <fstream>
	#include <iostream>
	#include "debug.h"

	SoundManager* SoundManager::__instance = nullptr;

	SoundManager::SoundManager()
	{
	}

	int SoundManager::ClampVolume(int volume)
	{
		if (volume < 0) return 0;
		if (volume > 100) return 100;
		return volume;
	}

	long SoundManager::VolumeToDecibels(int volume)
	{
		volume = ClampVolume(volume);
		if (volume == 0) return DSBVOLUME_MIN;

		// Keep the UI's 0-100 values usable: the old linear -10000..0 mapping
		// made mid-range values like 49 effectively silent.
		return -3000 + (volume * 30);
	}

	SoundManager::~SoundManager()
	{
		DebugOut(L"[INFO] SoundManager destructor called\n");
	}

	SoundManager* SoundManager::GetInstance()
	{
		if (__instance == nullptr)
		{
			__instance = new SoundManager();
		}
		return __instance;
	}

	void SoundManager::ReleaseInstance()
	{
		if (__instance)
		{
			delete __instance;
			__instance = nullptr;
		}
	}

	bool SoundManager::Init(HWND hWnd)
	{
		DebugOut(L"[INFO] SoundManager::Init() called\n");
		this->hWnd = hWnd;

		HRESULT result = DirectSoundCreate8(nullptr, &dsound, nullptr);
		if (FAILED(result))
		{
			DebugOut(L"[ERROR] DirectSoundCreate8 failed with HRESULT: 0x%08X\n", result);
			return false;
		}
		DebugOut(L"[INFO] DirectSoundCreate8 succeeded\n");

		result = dsound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
		if (FAILED(result))
		{
			DebugOut(L"[ERROR] SetCooperativeLevel failed with HRESULT: 0x%08X\n", result);
			return false;
		}
		DebugOut(L"[INFO] SetCooperativeLevel succeeded\n");

		// Create primary buffer
		DSBUFFERDESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
		bufferDesc.dwSize = sizeof(DSBUFFERDESC);
		bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
		bufferDesc.dwBufferBytes = 0;
		bufferDesc.lpwfxFormat = nullptr;

		result = dsound->CreateSoundBuffer(&bufferDesc, &primaryBuffer, nullptr);
		if (FAILED(result))
		{
			DebugOut(L"[ERROR] CreateSoundBuffer (primary) failed with HRESULT: 0x%08X\n", result);
			return false;
		}
		DebugOut(L"[INFO] Primary buffer created successfully\n");

		DebugOut(L"[INFO] SoundManager initialized successfully\n");
		return true;
	}

	bool SoundManager::LoadWAV(const std::string& filePath, ComPtr<IDirectSoundBuffer>& buffer, bool isMusic)
	{
		DebugOut(L"[INFO] LoadWAV() called for path: %s, isMusic: %d\n", std::wstring(filePath.begin(), filePath.end()).c_str(), isMusic);
		std::ifstream file(filePath, std::ios::binary);
		if (!file.is_open())
		{
			DebugOut(L"[ERROR] Failed to open audio file: %s\n", std::wstring(filePath.begin(), filePath.end()).c_str());
			return false;
		}
		DebugOut(L"[INFO] File opened successfully\n");

		// Read WAV header
		char riff[4];
		file.read(riff, 4);
		if (strncmp(riff, "RIFF", 4) != 0)
		{
			DebugOut(L"[ERROR] Invalid WAV file: not RIFF format\n");
			file.close();
			return false;
		}

		DWORD fileSize;
		file.read((char*)&fileSize, sizeof(DWORD));

		char wave[4];
		file.read(wave, 4);
		if (strncmp(wave, "WAVE", 4) != 0)
		{
			DebugOut(L"[ERROR] Invalid WAV file: not WAVE format\n");
			file.close();
			return false;
		}
		DebugOut(L"[INFO] WAV header validated\n");

		// Find fmt chunk
		char chunkId[4];
		DWORD chunkSize;
		WAVEFORMATEX wf;
		ZeroMemory(&wf, sizeof(WAVEFORMATEX));

		bool foundFmt = false;
		bool foundData = false;
		DWORD dataSize = 0;
		DWORD dataOffset = 0;

		while (file.read(chunkId, 4))
		{
			DWORD chunkSize;
			file.read((char*)&chunkSize, sizeof(DWORD));

			DebugOut(L"[INFO] Chunk: %.4S, size=%u, filePos=%u\n", chunkId, chunkSize, (DWORD)file.tellg());

			if (strncmp(chunkId, "fmt ", 4) == 0)
			{
				ZeroMemory(&wf, sizeof(WAVEFORMATEX));

				DWORD bytesToRead = min(chunkSize, (DWORD)sizeof(WAVEFORMATEX));
				file.read((char*)&wf, bytesToRead);

				if (chunkSize > bytesToRead)
					file.seekg(chunkSize - bytesToRead, std::ios::cur);

				// Pad to word boundary
				if (chunkSize % 2 != 0)
					file.seekg(1, std::ios::cur);

				foundFmt = true;
			}
			else if (strncmp(chunkId, "data", 4) == 0)
			{
				dataSize = chunkSize;
				dataOffset = (DWORD)file.tellg();
				foundData = true;
				break;
			}
			else if (strncmp(chunkId, "LIST", 4) == 0)
			{
				// Skip LIST chunks (metadata)
				file.seekg(chunkSize, std::ios::cur);
				// Pad to word boundary
				if (chunkSize % 2 != 0)
					file.seekg(1, std::ios::cur);
			}
			else
			{
				// Skip unknown chunks
				file.seekg(chunkSize, std::ios::cur);
				// Pad to word boundary
				if (chunkSize % 2 != 0)
					file.seekg(1, std::ios::cur);
			}
		}

		if (!foundFmt || !foundData)
		{
			DebugOut(L"[ERROR] Invalid WAV file: missing fmt or data chunk\n");
			file.close();
			return false;
		}

		// Check if format is PCM (DirectSound requires PCM)
		if (wf.wFormatTag != WAVE_FORMAT_PCM)
		{
			DebugOut(L"[ERROR] WAV file is not PCM format (tag: %d). DirectSound only supports PCM.\n", wf.wFormatTag);
			file.close();
			return false;
		}
		DebugOut(L"[INFO] WAV format is PCM (supported)\n");

		// Create secondary buffer
		if (!CreateBuffer(buffer, wf, dataSize, isMusic))
		{
			file.close();
			return false;
		}
		DebugOut(L"[INFO] Secondary buffer created\n");

		// Lock buffer and write data
		LPVOID audioPtr1 = nullptr;
		LPVOID audioPtr2 = nullptr;
		DWORD audioBytes1 = 0;
		DWORD audioBytes2 = 0;

		HRESULT result = buffer->Lock(0, dataSize, &audioPtr1, &audioBytes1, &audioPtr2, &audioBytes2, 0);
		if (FAILED(result))
		{
			DebugOut(L"[ERROR] Failed to lock buffer with HRESULT: 0x%08X\n", result);
			file.close();
			return false;
		}
		DebugOut(L"[INFO] Buffer locked successfully\n");

		file.seekg(dataOffset);
		file.read((char*)audioPtr1, audioBytes1);
		if (audioBytes2 > 0)
		{
			file.read((char*)audioPtr2, audioBytes2);
		}
		DebugOut(L"[INFO] Audio data written to buffer\n");

		buffer->Unlock(audioPtr1, audioBytes1, audioPtr2, audioBytes2);
		file.close();
		DebugOut(L"[INFO] Buffer unlocked and file closed\n");

		return true;
	}

	bool SoundManager::CreateBuffer(ComPtr<IDirectSoundBuffer>& buffer, const WAVEFORMATEX& wf, DWORD dataSize, bool isMusic)
	{
		DebugOut(L"[INFO] CreateBuffer() called, dataSize: %d, isMusic: %d\n", dataSize, isMusic);
		DSBUFFERDESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
		bufferDesc.dwSize = sizeof(DSBUFFERDESC);

		if (isMusic)
		{
			bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
		}
		else
		{
			bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN;
		}

		bufferDesc.dwBufferBytes = dataSize;
		bufferDesc.lpwfxFormat = (WAVEFORMATEX*)&wf;

		HRESULT result = dsound->CreateSoundBuffer(&bufferDesc, &buffer, nullptr);
		if (FAILED(result))
		{
			DebugOut(L"[ERROR] CreateSoundBuffer failed with HRESULT: 0x%08X\n", result);
			return false;
		}
		DebugOut(L"[INFO] Secondary buffer created successfully\n");

		return true;
	}

	bool SoundManager::LoadSFX(int id, const std::string& filePath)
	{
		DebugOut(L"[SOUND_DEBUG] LoadSFX() called for ID %d, path: %S\n", id, filePath.c_str());
		ComPtr<IDirectSoundBuffer> buffer;
		if (!LoadWAV(filePath, buffer, false))
		{
			DebugOut(L"[ERROR] Failed to load SFX %d from %s\n", id, std::wstring(filePath.begin(), filePath.end()).c_str());
			return false;
		}

		sfxBuffers[id] = buffer;
		ApplyVolumeSettings();
		DebugOut(L"[INFO] Successfully loaded SFX %d from %s\n", id, std::wstring(filePath.begin(), filePath.end()).c_str());
		return true;
	}

	bool SoundManager::LoadBGM(int id, const std::string& filePath)
	{
		DebugOut(L"[SOUND_DEBUG] LoadBGM() called for ID %d, path: %S\n", id, filePath.c_str());
		ComPtr<IDirectSoundBuffer> buffer;
		if (!LoadWAV(filePath, buffer, true))
		{
			DebugOut(L"[SOUND_DEBUG] Failed to load BGM %d from %S\n", id, filePath.c_str());
			return false;
		}

		bgmBuffers[id] = buffer;
		ApplyVolumeSettings();
		DebugOut(L"[SOUND_DEBUG] Successfully loaded BGM %d\n", id);
		return true;
	}

	void SoundManager::PlaySFX(int id, bool loop)
	{
		DebugOut(L"[INFO] PlaySFX() called for ID %d, loop: %d\n", id, loop);
		auto it = sfxBuffers.find(id);
		if (it != sfxBuffers.end())
		{
			it->second->SetCurrentPosition(0);
			DWORD flags = loop ? DSBPLAY_LOOPING : 0;
			HRESULT result = it->second->Play(0, 0, flags);
			if (FAILED(result))
			{
				DebugOut(L"[ERROR] Failed to play SFX %d with HRESULT: 0x%08X\n", id, result);
			}
			else
			{
				DebugOut(L"[INFO] Successfully playing SFX %d\n", id);
			}
		}
		else
		{
			DebugOut(L"[WARNING] SFX %d not found in sfxBuffers\n", id);
		}
	}

	void SoundManager::PlayBGM(int id, bool loop)
	{
		DebugOut(L"[SOUND_DEBUG] PlayBGM() called for ID %d, loop: %d\n", id, loop);
		StopBGM();

		auto it = bgmBuffers.find(id);
		if (it != bgmBuffers.end())
		{
			currentBGM = id;
			isBGMPlaying = true;
			it->second->SetCurrentPosition(0);
			ApplyVolumeSettings();
			DWORD flags = loop ? DSBPLAY_LOOPING : 0;
			HRESULT result = it->second->Play(0, 0, flags);
			if (FAILED(result))
			{
				DebugOut(L"[ERROR] Failed to play BGM %d with HRESULT: 0x%08X\n", id, result);
			}
			else
			{
				DebugOut(L"[INFO] Successfully playing BGM %d\n", id);
			}
		}
		else
		{
			DebugOut(L"[WARNING] BGM %d not found in bgmBuffers\n", id);
		}

		
	}

	void SoundManager::StopBGM()
	{
		DebugOut(L"[INFO] StopBGM() called\n");
		if (currentBGM != -1)
		{
			auto it = bgmBuffers.find(currentBGM);
			if (it != bgmBuffers.end())
			{
				it->second->Stop();
			}
			currentBGM = -1;
			isBGMPlaying = false;
		}
	}

	void SoundManager::PauseBGM()
	{
		DebugOut(L"[INFO] PauseBGM() called\n");
		if (currentBGM != -1 && isBGMPlaying)
		{
			auto it = bgmBuffers.find(currentBGM);
			if (it != bgmBuffers.end())
			{
				it->second->Stop();
			}
			isBGMPlaying = false;
		}
	}

	void SoundManager::ResumeBGM()
	{
		DebugOut(L"[INFO] ResumeBGM() called\n");
		if (currentBGM != -1 && !isBGMPlaying)
		{
			auto it = bgmBuffers.find(currentBGM);
			if (it != bgmBuffers.end())
			{
				it->second->Play(0, 0, DSBPLAY_LOOPING);
				isBGMPlaying = true;
			}
		}
	}

	void SoundManager::StopSFX(int id)
	{
		DebugOut(L"[INFO] StopSFX() called for ID %d\n", id);
		auto it = sfxBuffers.find(id);
		if (it != sfxBuffers.end())
		{
			it->second->Stop();
			it->second->SetCurrentPosition(0);
		}
	}

	void SoundManager::StopAllSFX()
	{
		DebugOut(L"[INFO] StopAllSFX() called\n");
		for (auto& pair : sfxBuffers)
		{
			pair.second->Stop();
			pair.second->SetCurrentPosition(0);
		}
	}

	void SoundManager::SetSFXVolume(int id, int volume)
	{
		volume = ClampVolume(volume);
		DebugOut(L"[INFO] SetSFXVolume() called for ID %d, volume: %d\n", id, volume);
		auto it = sfxBuffers.find(id);
		if (it != sfxBuffers.end())
		{
			long decibels = VolumeToDecibels(volume);
			HRESULT result = it->second->SetVolume(decibels);
			if (FAILED(result))
			{
				DebugOut(L"[ERROR] SetVolume failed with HRESULT: 0x%08X\n", result);
			}
		}
	}

	void SoundManager::SetBGMVolume(int volume)
	{
		volume = ClampVolume(volume);
		DebugOut(L"[INFO] SetBGMVolume() called with volume: %d\n", volume);
		if (currentBGM != -1)
		{
			auto it = bgmBuffers.find(currentBGM);
			if (it != bgmBuffers.end())
			{
				long decibels = VolumeToDecibels(volume);
				HRESULT result = it->second->SetVolume(decibels);
				if (FAILED(result))
				{
					DebugOut(L"[ERROR] SetVolume failed with HRESULT: 0x%08X\n", result);
				}
			}
		}
	}

	void SoundManager::SetVolumeSettings(int master, int music, int sfx)
	{
		masterVolume = ClampVolume(master);
		musicVolume = ClampVolume(music);
		sfxVolume = ClampVolume(sfx);
		ApplyVolumeSettings();
	}

	void SoundManager::ApplyVolumeSettings()
	{
		int effectiveMusic =
			(masterVolume * musicVolume) / 100;

		int effectiveSfx =
			(masterVolume * sfxVolume) / 100;

		SetBGMVolume(effectiveMusic);

		for (auto& pair : sfxBuffers)
			SetSFXVolume(pair.first, effectiveSfx);
	}

	void SoundManager::Update()
	{
		// Check if BGM has stopped
		if (currentBGM != -1 && isBGMPlaying)
		{
			auto it = bgmBuffers.find(currentBGM);
			if (it != bgmBuffers.end())
			{
				DWORD status;
				HRESULT result = it->second->GetStatus(&status);
				if (FAILED(result))
				{
					DebugOut(L"[ERROR] GetStatus failed with HRESULT: 0x%08X\n", result);
				}
				else if (!(status & DSBSTATUS_PLAYING))
				{
					DebugOut(L"[INFO] BGM stopped playing\n");
					isBGMPlaying = false;
				}
			}
		}
	}
