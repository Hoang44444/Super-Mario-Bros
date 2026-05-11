#pragma once

#include <Windows.h>
#include <string>
#include "KeyEventHandler.h"

class GameObject;
typedef GameObject* LPGAMEOBJECT;

class Scene;
typedef Scene* LPSCENE;

// --- Lớp trung gian xử lý phím (BẠN ĐANG THIẾU CÁI NÀY) ---
class SceneKeyHandler : public KeyEventHandler
{
protected:
	LPSCENE scence; // Biến lưu scene hiện tại (để ép kiểu sang MenuScene/PlayScene)
public:
	virtual void KeyState(BYTE* states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	SceneKeyHandler(LPSCENE s) : KeyEventHandler() { scence = s; }
};

class Scene
{
protected:
	LPKEYEVENTHANDLER key_handler;
	int id;
	std::wstring sceneFilePath;

public:
	Scene(int id, LPCWSTR filePath)
	{
		this->id = id;
		this->sceneFilePath = filePath;
		this->key_handler = NULL;
	}

	LPKEYEVENTHANDLER GetKeyEventHandler() { return key_handler; }

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
	virtual void AddObject(LPGAMEOBJECT obj) = 0;
};