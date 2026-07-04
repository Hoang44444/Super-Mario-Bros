#pragma once
#include <Windows.h>
#include <string>
#include "KeyEventHandler.h"

class GameObject;
typedef GameObject* LPGAMEOBJECT;

// Scene là base class cho tất cả scene trong game
// Mỗi scene (menu, level, end screen) là một instance của Scene
// Quản lý object trong scene và key handler riêng
class Scene
{
protected:
	LPKEYEVENTHANDLER key_handler;  // Handler xử lý input cho scene này
	int id;                          // ID của scene (xác định trong AssetID.h)
	std::wstring sceneFilePath;     // Đường dẫn file config của scene

public:
	Scene(int id, LPCWSTR filePath)
	{
		this->id = id;
		this->sceneFilePath = filePath;
		this->key_handler = NULL;
	}

	// Lấy key handler của scene
	LPKEYEVENTHANDLER GetKeyEventHandler() { return key_handler; }

	// Load scene từ file config (được gọi khi chuyển scene)
	virtual void Load() = 0;
	
	// Unload scene - giải phóng resources (được gọi khi chuyển scene khác)
	virtual void Unload() = 0;
	
	// Update logic scene mỗi frame
	virtual void Update(DWORD dt) = 0;
	
	// Render scene
	virtual void Render() = 0;
	
	// Thêm object vào scene
	virtual void AddObject(LPGAMEOBJECT obj) = 0;
};

typedef Scene* LPSCENE;
