#pragma once

#include <Windows.h>
#include <d3dx10.h>

class Mario;

// HUD (Heads-Up Display) - hiển thị thông tin game (score, coins, time, lives, world)
// Render text và icon ở góc màn hình hoặc giữa màn hình (intro/death)
class HUD
{
private:
	static const int START_TIME = 400;  // Thời gian bắt đầu (giây)

	Mario* mario;  // Con trỏ đến Mario để lấy trạng thái
	int world;     // World hiện tại (1-1, 1-2, ...)
	int stage;     // Stage hiện tại
	int remainingTime;  // Thời gian còn lại
	DWORD elapsedMs;    // Thời gian đã trôi qua (ms) để đếm giây
	ID3DX10Font* font;  // Font để vẽ text
	bool warningBGMPlaying;  // Đang chơi nhạc cảnh báo (thời gian <= 100s)
	bool stageClearOverride;  // Nếu true, bỏ qua logic cảnh báo (trong stage clear)

	void CreateFontIfNeeded();  // Tạo font nếu chưa có
	void DrawTextLine(const wchar_t* text, int x, int y, int width, int height);  // Vẽ text tại vị trí
	void DrawCenteredTextLine(const wchar_t* text, int y, int height);  // Vẽ text căn giữa màn hình
	void RenderTextureRectAtScreen(int textureId, const RECT& rect, float screenX, float screenY, float z = 0.9f);  // Vẽ texture tại vị trí screen
	void RenderCoinIcon(float screenX, float screenY);  // Vẽ icon coin (animation)
	float GetMarioLivesStartX() const;  // Tính vị trí X bắt đầu cho icon lives
	void RenderMarioLivesIcon(int y);  // Vẽ icon Mario nhỏ
	void DrawMarioLivesText(int lives, int y);  // Vẽ text số mạng

public:
	HUD(Mario* mario, int world = 1, int stage = 1);
	~HUD();

	void SetMario(Mario* mario) { this->mario = mario; }
	void ResetTimer();  // Reset timer về START_TIME
	void SetRemainingTime(int time) { remainingTime = time; }
	void SetStageClearActive(bool active) { stageClearOverride = active; }  // Bật/tắt override stage clear
	void Update(DWORD dt);
	void Render();

	int GetRemainingTime() const { return remainingTime; }
};
