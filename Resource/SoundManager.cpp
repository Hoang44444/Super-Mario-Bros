#include "SoundManager.h"
#include <fstream>
#include <iostream>

SoundManager* SoundManager::__instance = nullptr;

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
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
	this->hWnd = hWnd;

	HRESULT result = DirectSoundCreate8(nullptr, &dsound, nullptr);
	if (FAILED(result))
	{
		return false;
	}

	result = dsound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

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
		return false;
	}

	return true;
}

bool SoundManager::LoadWAV(const std::string& filePath, ComPtr<IDirectSoundBuffer>& buffer, bool isMusic)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
	{
		return false;
	}

	// Read WAV header
	char riff[4];
	file.read(riff, 4);
	if (strncmp(riff, "RIFF", 4) != 0)
	{
		file.close();
		return false;
	}

	DWORD fileSize;
	file.read((char*)&fileSize, sizeof(DWORD));

	char wave[4];
	file.read(wave, 4);
	if (strncmp(wave, "WAVE", 4) != 0)
	{
		file.close();
		return false;
	}

	// Find fmt chunk
	char chunkId[4];
	DWORD chunkSize;
	WAVEFORMATEX wf;
	ZeroMemory(&wf, sizeof(WAVEFORMATEX));

	bool foundFmt = false;
	bool foundData = false;
	DWORD dataSize = 0;
	DWORD dataOffset = 0;

	while (file)
	{
		file.read(chunkId, 4);
		file.read((char*)&chunkSize, sizeof(DWORD));

		if (strncmp(chunkId, "fmt ", 4) == 0)
		{
			file.read((char*)&wf, sizeof(WAVEFORMATEX));
			foundFmt = true;
		}
		else if (strncmp(chunkId, "data", 4) == 0)
		{
			dataSize = chunkSize;
			dataOffset = file.tellg();
			foundData = true;
			break;
		}
		else
		{
			file.seekg(chunkSize, std::ios::cur);
		}
	}

	if (!foundFmt || !foundData)
	{
		file.close();
		return false;
	}

	// Create secondary buffer
	if (!CreateBuffer(buffer, wf, dataSize, isMusic))
	{
		file.close();
		return false;
	}

	// Lock buffer and write data
	LPVOID audioPtr1 = nullptr;
	LPVOID audioPtr2 = nullptr;
	DWORD audioBytes1 = 0;
	DWORD audioBytes2 = 0;

	HRESULT result = buffer->Lock(0, dataSize, &audioPtr1, &audioBytes1, &audioPtr2, &audioBytes2, 0);
	if (FAILED(result))
	{
		file.close();
		return false;
	}

	file.seekg(dataOffset);
	file.read((char*)audioPtr1, audioBytes1);
	if (audioBytes2 > 0)
	{
		file.read((char*)audioPtr2, audioBytes2);
	}

	buffer->Unlock(audioPtr1, audioBytes1, audioPtr2, audioBytes2);
	file.close();

	return true;
}

bool SoundManager::CreateBuffer(ComPtr<IDirectSoundBuffer>& buffer, const WAVEFORMATEX& wf, DWORD dataSize, bool isMusic)
{
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
		return false;
	}

	return true;
}

bool SoundManager::LoadSFX(int id, const std::string& filePath)
{
	ComPtr<IDirectSoundBuffer> buffer;
	if (!LoadWAV(filePath, buffer, false))
	{
		return false;
	}

	sfxBuffers[id] = buffer;
	return true;
}

bool SoundManager::LoadBGM(int id, const std::string& filePath)
{
	ComPtr<IDirectSoundBuffer> buffer;
	if (!LoadWAV(filePath, buffer, true))
	{
		return false;
	}

	bgmBuffers[id] = buffer;
	return true;
}

void SoundManager::PlaySFX(int id, bool loop)
{
	auto it = sfxBuffers.find(id);
	if (it != sfxBuffers.end())
	{
		it->second->SetCurrentPosition(0);
		DWORD flags = loop ? DSBPLAY_LOOPING : 0;
		it->second->Play(0, 0, flags);
	}
}

void SoundManager::PlayBGM(int id, bool loop)
{
	StopBGM();

	auto it = bgmBuffers.find(id);
	if (it != bgmBuffers.end())
	{
		currentBGM = id;
		isBGMPlaying = true;
		it->second->SetCurrentPosition(0);
		DWORD flags = loop ? DSBPLAY_LOOPING : 0;
		it->second->Play(0, 0, flags);
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
		it->second->SetCurrentPosition(0);
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
	auto it = sfxBuffers.find(id);
	if (it != sfxBuffers.end())
	{
		// Volume: 0 = -10000dB (silent), 100 = 0dB (max)
		long decibels = (long)(-10000 + (volume * 100));
		it->second->SetVolume(decibels);
	}
}

void SoundManager::SetBGMVolume(int volume)
{
	if (currentBGM != -1)
	{
		auto it = bgmBuffers.find(currentBGM);
		if (it != bgmBuffers.end())
		{
			long decibels = (long)(-10000 + (volume * 100));
			it->second->SetVolume(decibels);
		}
	}
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
			it->second->GetStatus(&status);
			if (!(status & DSBSTATUS_PLAYING))
			{
				isBGMPlaying = false;
			}
		}
	}
}
