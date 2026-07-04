#pragma once

#include <vector>
#include "Mushroom.h"
#include "GameObject.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"

using namespace std;

class Mario;

#define MUSHROOM_1UP_STATE_SPAWNING 1
#define MUSHROOM_1UP_STATE_WALKING  2

// Mushroom1Up - item 1-up (nấm xanh) cộng thêm mạng khi ăn
class Mushroom1Up : public Mushroom {
protected:
	float start_y;  // Vị trí Y ban đầu (để check khi spawn xong)
public:
	Mushroom1Up(float x, float y, float z) : Mushroom(x, y, z) 
	{
		vx = 0.0f;
		vy = -0.05f; 
		start_y = y;
		state = MUSHROOM_1UP_STATE_SPAWNING;
	}

	virtual ~Mushroom1Up() {}

	void Render() override {
		LPANIMATION ani = AnimationManager::GetInstance()->Get(ANIMATION::ITEM_MUSHROOM_1UP);
		if (ani != nullptr)
			ani->Render(x, y, z);
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) override;
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void OnNoCollision(DWORD dt) override;

	void OnMarioCollision(Mario* mario) override;

	// 1Up đã rise ra khỏi block qua state SPAWNING của riêng nó, nên
	// generic emerge animation là no-op ở đây.
	void StartEmerge() override {}
};
