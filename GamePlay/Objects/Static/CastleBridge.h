#pragma once
#include "StaticObject.h"

constexpr float CASTLE_BRIDGE_BBOX_WIDTH  = 16.0f;
constexpr float CASTLE_BRIDGE_BBOX_HEIGHT = 16.0f;
constexpr float CASTLE_BRIDGE_GRAVITY     = 0.0015f;

class CastleBridge : public StaticObject
{
private:
	DWORD collapseDelay = 0;
	bool isCollapsing = false;
	bool isFalling    = false;

public:
	CastleBridge(float x, float y, float z) : StaticObject(x, y, z) {}
	virtual ~CastleBridge() {}

	void Collapse(DWORD delay);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	virtual bool IsBlocking() override { return !isFalling; }
};
