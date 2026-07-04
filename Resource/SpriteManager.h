#pragma once
#include <unordered_map>
#include "Sprite.h"

using namespace std;

// Singleton manager để load và lưu định nghĩa sprite
// Sprite định nghĩa vùng trên texture để render đối tượng game riêng lẻ
// Gọi SpriteManager::GetInstance() để lấy instance singleton
// Dùng Add() để định nghĩa sprite, sau đó Get() để lấy ra để render
class SpriteManager
{
	static SpriteManager* __instance;  // Con trỏ instance singleton

	unordered_map<int, LPSPRITE> sprites;  // Key: ID sprite, Value: Con trỏ Sprite

public:
	// Định nghĩa vùng sprite trên texture với ID cho trước
	// Cách dùng: SpriteManager::GetInstance()->Add(100, 0, 0, 32, 32, texture);
	void Add(int id, int left, int top, int right, int bottom, LPTEXTURE tex);

	// Lấy sprite theo ID - trả về NULL nếu không tìm thấy
	LPSPRITE Get(int id);

	// Giải phóng tất cả sprite đã load - gọi khi chuyển scene
	void Clear();

	// Lấy instance singleton - tạo mới nếu chưa tồn tại
	static SpriteManager* GetInstance();
};
