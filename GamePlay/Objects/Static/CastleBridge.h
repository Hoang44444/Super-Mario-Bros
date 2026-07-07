#pragma once
#include "StaticObject.h"

constexpr float CASTLE_BRIDGE_BBOX_WIDTH  = 16.0f;
constexpr float CASTLE_BRIDGE_BBOX_HEIGHT = 16.0f;
constexpr float CASTLE_BRIDGE_GRAVITY     = 0.0015f;

// Cầu castle - Mario chạm rìu sẽ làm cầu sập (dùng ở cuối castle)
// Có độ trễ trước khi sập để tạo hiệu ứng sequence
class CastleBridge : public StaticObject
{
private:
	DWORD collapseDelay = 0;  // Độ trễ trước khi sập
	bool isCollapsing = false;  // Đang trong quá trình sập
	bool isFalling    = false;  // Đang rơi xuống

public:
	CastleBridge(float x, float y, float z) : StaticObject(x, y, z) {}
	virtual ~CastleBridge() {}

	void Collapse(DWORD delay);  // Bắt đầu quá trình sập sau delay ms

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override;
	virtual void Render() override;
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	virtual bool IsBlocking() override { return !isFalling; }  // Chỉ chặn khi không rơi
};
