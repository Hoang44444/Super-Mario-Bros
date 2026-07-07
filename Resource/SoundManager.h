#pragma once
#include <Windows.h>
#include <dsound.h>
#include <unordered_map>
#include <string>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

// Singleton manager để quản lý âm thanh game (DirectSound 8)
// Hỗ trợ 2 loại âm thanh: SFX (hiệu ứng) và BGM (nhạc nền)
// Cách dùng:
//   1. Gọi Init(hwnd) khi khởi tạo game
//   2. Load âm thanh với LoadSFX(id, path) và LoadBGM(id, path)
//   3. Play với PlaySFX(id) và PlayBGM(id)
//   4. Điều chỉnh volume với SetVolumeSettings(master, music, sfx)
//   5. Gọi Update() mỗi frame để kiểm tra trạng thái BGM
//   6. Gọi ReleaseInstance() khi đóng game
class SoundManager
{
private:
	static SoundManager* __instance;  // Instance singleton
	ComPtr<IDirectSound8> dsound;      // DirectSound 8 device
	ComPtr<IDirectSoundBuffer> primaryBuffer;  // Primary buffer (chỉ để cấu hình)

	// Map lưu buffer âm thanh theo ID
	std::unordered_map<int, ComPtr<IDirectSoundBuffer>> sfxBuffers;  // Hiệu ứng âm thanh
	std::unordered_map<int, ComPtr<IDirectSoundBuffer>> bgmBuffers;  // Nhạc nền

	int currentBGM = -1;       // ID BGM đang play
	bool isBGMPlaying = false; // BGM có đang play không
	int masterVolume = 100;    // Volume master (0-100)
	int musicVolume = 100;     // Volume nhạc nền (0-100)
	int sfxVolume = 100;       // Volume hiệu ứng (0-100)

	HWND hWnd;  // Handle cửa sổ game

	// Phương thức nội bộ - load file WAV vào buffer
	bool LoadWAV(const std::string& filePath, ComPtr<IDirectSoundBuffer>& buffer, bool isMusic = false);
	
	// Phương thức nội bộ - tạo secondary buffer với format cho trước
	bool CreateBuffer(ComPtr<IDirectSoundBuffer>& buffer, const WAVEFORMATEX& wf, DWORD dataSize, bool isMusic = false);
	
	// Giới hạn volume trong khoảng 0-100
	int ClampVolume(int volume);
	
	// Chuyển volume 0-100 sang decibel (dùng bởi DirectSound)
	long VolumeToDecibels(int volume);

	SoundManager();
	~SoundManager();

public:
	// Lấy instance singleton - tạo mới nếu chưa tồn tại
	static SoundManager* GetInstance();
	
	// Giải phóng instance singleton - gọi khi đóng game
	static void ReleaseInstance();

	// Khởi tạo DirectSound - phải gọi trước khi load/play âm thanh
	bool Init(HWND hWnd);
	
	// Load hiệu ứng âm thanh từ file WAV với ID cho trước
	bool LoadSFX(int id, const std::string& filePath);
	
	// Load nhạc nền từ file WAV với ID cho trước
	bool LoadBGM(int id, const std::string& filePath);

	// Play hiệu ứng âm thanh - loop=false mặc định (chỉ play 1 lần)
	void PlaySFX(int id, bool loop = false);
	
	// Play nhạc nền - loop=true mặc định (play lặp lại)
	void PlayBGM(int id, bool loop = true);
	
	// Dừng nhạc nền đang play
	void StopBGM();
	
	// Tạm dừng nhạc nền (có thể resume lại)
	void PauseBGM();
	
	// Tiếp tục play nhạc nền đã pause
	void ResumeBGM();

	// Dừng hiệu ứng âm thanh cụ thể
	void StopSFX(int id);
	
	// Dừng tất cả hiệu ứng âm thanh
	void StopAllSFX();

	// Set volume cho hiệu ứng cụ thể (0-100)
	void SetSFXVolume(int id, int volume);
	
	// Set volume cho nhạc nền (0-100)
	void SetBGMVolume(int volume);
	
	// Set tất cả volume cùng lúc (master, music, sfx đều 0-100)
	void SetVolumeSettings(int master, int music, int sfx);
	
	int GetMasterVolume() { return masterVolume; }
	int GetMusicVolume() { return musicVolume; }
	int GetSFXVolume() { return sfxVolume; }
	
	// Áp dụng volume settings lên tất cả buffer đang play
	void ApplyVolumeSettings();

	// Update mỗi frame - kiểm tra trạng thái BGM (đã dừng chưa)
	void Update();
};

