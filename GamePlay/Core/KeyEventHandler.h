#pragma once
#include <Windows.h>

// Interface cho keyboard handler
// Mỗi scene có thể có key handler riêng để xử lý input theo cách khác nhau
// Ví dụ: MenuKeyHandler cho menu, PlayKeyHandler cho gameplay
class KeyEventHandler
{
public:
	// Xử lý keyboard state (được gọi mỗi frame khi phím đang được giữ)
	// states: mảng 256 byte chứa trạng thái của tất cả phím
	virtual void KeyState(BYTE *states) = 0;
	
	// Xử lý khi phím được nhấn xuống (key down event)
	virtual void OnKeyDown(int KeyCode) = 0;
	
	// Xử lý khi phím được thả ra (key up event)
	virtual void OnKeyUp(int KeyCode) = 0;
};

typedef KeyEventHandler * LPKEYEVENTHANDLER;
