#pragma once
#include <Windows.h>
#include <dsound.h>
#include <unordered_map>
#include <string>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class SoundManager
{
private:
	static SoundManager* __instance;
	ComPtr<IDirectSound8> dsound;
	ComPtr<IDirectSoundBuffer> primaryBuffer;

	std::unordered_map<int, ComPtr<IDirectSoundBuffer>> sfxBuffers;
	std::unordered_map<int, ComPtr<IDirectSoundBuffer>> bgmBuffers;

	int currentBGM = -1;
	bool isBGMPlaying = false;
	int masterVolume = 100;
	int musicVolume = 100;
	int sfxVolume = 100;

	HWND hWnd;

	bool LoadWAV(const std::string& filePath, ComPtr<IDirectSoundBuffer>& buffer, bool isMusic = false);
	bool CreateBuffer(ComPtr<IDirectSoundBuffer>& buffer, const WAVEFORMATEX& wf, DWORD dataSize, bool isMusic = false);
	int ClampVolume(int volume);
	long VolumeToDecibels(int volume);

	SoundManager();
	~SoundManager();

public:
	static SoundManager* GetInstance();
	static void ReleaseInstance();

	bool Init(HWND hWnd);
	bool LoadSFX(int id, const std::string& filePath);
	bool LoadBGM(int id, const std::string& filePath);

	void PlaySFX(int id, bool loop = false);
	void PlayBGM(int id, bool loop = true);
	void StopBGM();
	void PauseBGM();
	void ResumeBGM();

	void StopSFX(int id);
	void StopAllSFX();

	void SetSFXVolume(int id, int volume); // 0-100
	void SetBGMVolume(int volume); // 0-100
	void SetVolumeSettings(int master, int music, int sfx);
	int GetMasterVolume() { return masterVolume; }
	int GetMusicVolume() { return musicVolume; }
	int GetSFXVolume() { return sfxVolume; }
	void ApplyVolumeSettings();

	void Update(); // For BGM looping and other updates
};
