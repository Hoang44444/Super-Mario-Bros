#pragma once
#include <unordered_map>
#include <d3dx10.h>

#include "Texture.h"

using namespace std;

// Singleton manager để load và lưu texture
// Gọi TextureManager::GetInstance() để lấy instance singleton
// Dùng Add() để load texture với ID, sau đó Get() để lấy ra
// Gọi Clear() khi chuyển scene để giải phóng bộ nhớ
class TextureManager
{
	static TextureManager* __instance;  // Con trỏ instance singleton

	unordered_map<int, LPTEXTURE> textures;  // Key: ID texture, Value: Con trỏ Texture

public:
	TextureManager();

	// Load texture từ file và lưu với ID cho trước
	// Cách dùng: TextureManager::GetInstance()->Add(100, L"mario.png");
	void Add(int id, LPCWSTR filePath);

	// Lấy texture theo ID - trả về NULL nếu không tìm thấy
	LPTEXTURE Get(unsigned int i);

	// Giải phóng tất cả texture đã load - gọi khi chuyển scene
	void Clear();

	// Phương thức nội bộ để load texture từ đường dẫn file
	LPTEXTURE LoadTexture(LPCWSTR texturePath);

	// Lấy instance singleton - tạo mới nếu chưa tồn tại
	static TextureManager* GetInstance();
};
