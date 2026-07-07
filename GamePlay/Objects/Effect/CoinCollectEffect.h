#pragma once

#include "GameObject.h"

class CoinCollectEffect : public GameObject
{
private:
	static const DWORD LIFETIME = 400;
	static constexpr float RISE_SPEED = -0.08f;

	DWORD elapsedTime;

public:
	CoinCollectEffect(float x, float y, float z);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }
};
