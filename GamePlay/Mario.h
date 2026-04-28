#pragma once
#include "GameObject.h"

#define MARIO_LEVEL_SMALL	1
#define MARIO_LEVEL_BIG		2

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_SIT				400
#define MARIO_STATE_SIT_RELEASE		401


class Mario : public GameObject
{
private:
	int level; 
	float gravity = 0.002f;
	float accelX = 0.0f;
	void MovementUpdate(DWORD dt);
public:
	Mario(float x, float y) : GameObject(x, y) {
		level = MARIO_LEVEL_SMALL;
	};
	~Mario() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

