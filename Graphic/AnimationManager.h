#pragma once
#include <unordered_map>
#include "Animation.h"

using namespace std;

// Singleton manager để load và lưu animation
// AnimationManager quản lý tất cả animation trong game
// Cách dùng:
//   1. Gọi AnimationManager::GetInstance() để lấy instance singleton
//   2. Add() để thêm animation với ID
//   3. Get() để lấy animation theo ID
//   4. Clear() để giải phóng khi chuyển scene
class AnimationManager
{
	static AnimationManager* __instance;  // Instance singleton

	// Map lưu animation theo ID
	unordered_map<int, LPANIMATION> animations;

public:
	// Thêm animation vào manager với ID cho trước
	void Add(int id, LPANIMATION ani);
	
	// Lấy animation theo ID - trả về NULL nếu không tìm thấy
	LPANIMATION Get(int id);
	
	// Giải phóng tất cả animation - gọi khi chuyển scene
	void Clear();

	// Lấy instance singleton - tạo mới nếu chưa tồn tại
	static AnimationManager* GetInstance();
};
