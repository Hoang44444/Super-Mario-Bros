#pragma once
#include <Windows.h>

/*
	Interface to invisible the keyboard handler 
*/
class KeyEventHandler
{
public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
};

typedef KeyEventHandler * LPKEYEVENTHANDLER;
