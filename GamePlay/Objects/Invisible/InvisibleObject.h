#pragma once
#include "GameObject.h"
#include "Mario.h"
// Base class cho các đối tượng vô hình (không render nhưng có va chạm)
// Dùng cho các vùng đặc biệt như death zone, switch scene point, turn block
class InvisibleObject : public GameObject
{
public:
	InvisibleObject(float x, float y, float z) : GameObject(x, y, z) {};
	virtual ~InvisibleObject() {};

	// Xử lý khi Mario va chạm với đối tượng vô hình (mỗi class con định nghĩa riêng)
	virtual void OnMarioCollision(Mario* mario) {};

	bool IsCollidable() { return true; }
	bool IsBlocking() { return false; }


};

