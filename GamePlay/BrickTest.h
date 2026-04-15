#pragma once
#include "GameObject.h"
class Brick : GameObject
{
private:
	float leftLimit, rightLimit;
public:
	Brick(float x, float y, float leftLimit, float rightLimit) : GameObject(x, y) {
		this->leftLimit = leftLimit;
		this->rightLimit = rightLimit;
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
};

