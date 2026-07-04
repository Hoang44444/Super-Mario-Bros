	#include "SoundManager.h"
	#include <fstream>
	#include <iostream>
	#include "debug.h"

	// Khởi tạo instance singleton tĩnh
	SoundManager* SoundManager::__instance = nullptr;

	SoundManager::SoundManager()
	{
		// Constructor - các thành viên được khởi tạo mặc định
	}

	int SoundManager::ClampVolume(int volume)
	{
		// Giới hạn volume trong khoảng 0-100
		if (volume < 0) return 0;
		if (volume > 100) return 100;
		return volume;
	}

	long SoundManager::VolumeToDecibels(int volume)
	{
		volume = ClampVolume(volume);
		if (volume == 0) return DSBVOLUME_MIN;

		// Mapping volume 0-100 sang decibel cho DirectSound
		// Công thức này giúp mid-range values (như 49) không bị im lặng
		// Volume 100 -> 0dB (max), Volume 0 -> -10000dB (im lặng)
		return -3000 + (volume * 30);
	}

	SoundManager::~SoundManager()
	{
		// Destructor - ComPtr tự động release DirectSound resources
	}

	SoundManager* SoundManager::GetInstance()
	{
		// Lấy instance singleton - tạo mới nếu chưa tồn tại
		if (__instance == nullptr)
		{
			__instance = new SoundManager();
		}
		return __instance;
	}

	void SoundManager::ReleaseInstance()
	{
		// Giải phóng instance singleton - gọi khi đóng game
		if (__instance)
		{
			delete __instance;
			__instance = nullptr;
		}
	}

	bool SoundManager::Init(HWND hWnd)
	{
		this->hWnd = hWnd;

		// Bước 1: Tạo DirectSound device
		HRESULT result = DirectSoundCreate8(nullptr, &dsound, nullptr);
		if (FAILED(result))
		{
			DebugOut(L"[ERROR] DirectSoundCreate8 failed with HRESULT: 0x%08X\n", result);
			return false;
		}

		// Bước 2: Set cooperative level - DSSCL_PRIORITY cho phép full control
		result = dsound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
		if (FAILED(result))
		{
			DebugOut(L"[ERROR] SetCooperativeLevel failed with HRESULT: 0x%08X\n", result);
			return false;
		}

		// Bước 3: Tạo primary buffer - chỉ dùng để cấu hình format âm thanh chung
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

		return true;
	}

	bool SoundManager::LoadWAV(const std::string& filePath, ComPtr<IDirectSoundBuffer>& buffer, bool isMusic)
	{
		
		// Bước 1: Mở file WAV
		std::ifstream file(filePath, std::ios::binary);
		if (!file.is_open())
		{
			DebugOut(L"[ERROR] Failed to open audio file: %s\n", std::wstring(filePath.begin(), filePath.end()).c_str());
			return false;
		}

		// Bước 2: Đọc và validate WAV header (RIFF và WAVE)
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

		// Bước 3: Parse các chunk trong file WAV (fmt, data, LIST, v.v.)
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

			if (strncmp(chunkId, "fmt ", 4) == 0)
			{
				// Chunk fmt chứa thông tin format âm thanh (sample rate, channels, v.v.)
				ZeroMemory(&wf, sizeof(WAVEFORMATEX));

				DWORD bytesToRead = min(chunkSize, (DWORD)sizeof(WAVEFORMATEX));
				file.read((char*)&wf, bytesToRead);

				if (chunkSize > bytesToRead)
					file.seekg(chunkSize - bytesToRead, std::ios::cur);

				// WAV chunks phải align theo word boundary (2 bytes)
				if (chunkSize % 2 != 0)
					file.seekg(1, std::ios::cur);

				foundFmt = true;
			}
			else if (strncmp(chunkId, "data", 4) == 0)
			{
				// Chunk data chứa dữ liệu âm thanh thực tế
				dataSize = chunkSize;
				dataOffset = (DWORD)file.tellg();
				foundData = true;
				break;
			}
			else if (strncmp(chunkId, "LIST", 4) == 0)
			{
				// Chunk LIST chứa metadata - bỏ qua
				file.seekg(chunkSize, std::ios::cur);
				if (chunkSize % 2 != 0)
					file.seekg(1, std::ios::cur);
			}
			else
			{
				// Bỏ qua các chunk không xác định
				file.seekg(chunkSize, std::ios::cur);
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

		// Bước 4: Kiểm tra format - DirectSound chỉ hỗ trợ PCM
		if (wf.wFormatTag != WAVE_FORMAT_PCM)
		{
			DebugOut(L"[ERROR] WAV file is not PCM format (tag: %d). DirectSound only supports PCM.\n", wf.wFormatTag);
			file.close();
			return false;
		}

		// Bước 5: Tạo secondary buffer để chứa dữ liệu âm thanh
		if (!CreateBuffer(buffer, wf, dataSize, isMusic))
		{
			file.close();
			return false;
		}

		// Bước 6: Lock buffer để ghi dữ liệu âm thanh vào
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

		// Bước 7: Đọc dữ liệu âm thanh từ file và ghi vào buffer
		file.seekg(dataOffset);
		file.read((char*)audioPtr1, audioBytes1);
		if (audioBytes2 > 0)
		{
			file.read((char*)audioPtr2, audioBytes2);
		}

		// Bước 8: Unlock buffer và đóng file
		buffer->Unlock(audioPtr1, audioBytes1, audioPtr2, audioBytes2);
		file.close();

		return true;
	}

	bool SoundManager::CreateBuffer(ComPtr<IDirectSoundBuffer>& buffer, const WAVEFORMATEX& wf, DWORD dataSize, bool isMusic)
	{
		DSBUFFERDESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
		bufferDesc.dwSize = sizeof(DSBUFFERDESC);

		// Cấu hình flags cho buffer
		// DSBCAPS_CTRLVOLUME: cho phép điều chỉnh volume
		// DSBCAPS_CTRLPAN: cho phép điều chỉnh balance (trái/phải)
		// DSBCAPS_CTRLPAN: cho phép thay đổi tần số (chỉ cho music)
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

		return true;
	}

	bool SoundManager::LoadSFX(int id, const std::string& filePath)
	{
		ComPtr<IDirectSoundBuffer> buffer;
		
		// Load file WAV vào buffer (isMusic=false)
		if (!LoadWAV(filePath, buffer, false))
		{
			DebugOut(L"[ERROR] Failed to load SFX %d from %s\n", id, std::wstring(filePath.begin(), filePath.end()).c_str());
			return false;
		}

		sfxBuffers[id] = buffer;
		ApplyVolumeSettings();  // Áp dụng volume settings ngay sau khi load
		return true;
	}

	bool SoundManager::LoadBGM(int id, const std::string& filePath)
	{
		ComPtr<IDirectSoundBuffer> buffer;
		
		// Load file WAV vào buffer (isMusic=true - có thêm flag frequency control)
		if (!LoadWAV(filePath, buffer, true))
		{
			return false;
		}

		bgmBuffers[id] = buffer;
		ApplyVolumeSettings();
		return true;
	}

	void SoundManager::PlaySFX(int id, bool loop)
	{
		auto it = sfxBuffers.find(id);
		if (it != sfxBuffers.end())
		{
			// Reset về đầu buffer trước khi play
			it->second->SetCurrentPosition(0);
			DWORD flags = loop ? DSBPLAY_LOOPING : 0;
			HRESULT result = it->second->Play(0, 0, flags);
			if (FAILED(result))
			{
				DebugOut(L"[ERROR] Failed to play SFX %d with HRESULT: 0x%08X\n", id, result);
			}

		}
		else
		{
			DebugOut(L"[WARNING] SFX %d not found in sfxBuffers\n", id);
		}
	}

	void SoundManager::PlayBGM(int id, bool loop)
	{
		StopBGM();  // Dừng BGM đang play trước (nếu có)

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
		}
		else
		{
			DebugOut(L"[WARNING] BGM %d not found in bgmBuffers\n", id);
		}

		
	}

	void SoundManager::StopBGM()
	{
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
		if (currentBGM != -1 && isBGMPlaying)
		{
			auto it = bgmBuffers.find(currentBGM);
			if (it != bgmBuffers.end())
			{
				// Stop buffer nhưng giữ currentBGM để có thể resume
				it->second->Stop();
			}
			isBGMPlaying = false;
		}
	}

	void SoundManager::ResumeBGM()
	{
		if (currentBGM != -1 && !isBGMPlaying)
		{
			auto it = bgmBuffers.find(currentBGM);
			if (it != bgmBuffers.end())
			{
				// Play lại từ vị trí hiện tại (không reset về đầu)
				it->second->Play(0, 0, DSBPLAY_LOOPING);
				isBGMPlaying = true;
			}
		}
	}

	void SoundManager::StopSFX(int id)
	{
		auto it = sfxBuffers.find(id);
		if (it != sfxBuffers.end())
		{
			it->second->Stop();
			it->second->SetCurrentPosition(0);  // Reset về đầu buffer
		}
	}

	void SoundManager::StopAllSFX()
	{
		for (auto& pair : sfxBuffers)
		{
			pair.second->Stop();
			pair.second->SetCurrentPosition(0);
		}
	}

	void SoundManager::SetSFXVolume(int id, int volume)
	{
		volume = ClampVolume(volume);
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
		ApplyVolumeSettings();  // Áp dụng ngay lập tức
	}

	void SoundManager::ApplyVolumeSettings()
	{
		// Tính volume thực tế = master * category / 100
		int effectiveMusic =
			(masterVolume * musicVolume) / 100;

		int effectiveSfx =
			(masterVolume * sfxVolume) / 100;

		SetBGMVolume(effectiveMusic);

		// Áp dụng volume cho tất cả SFX buffers
		for (auto& pair : sfxBuffers)
			SetSFXVolume(pair.first, effectiveSfx);
	}

	void SoundManager::Update()
	{
		// Kiểm tra xem BGM đã dừng chưa (cho trường hợp non-looping BGM)
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
					isBGMPlaying = false;
				}
			}
		}
	}
