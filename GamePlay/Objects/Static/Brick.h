#pragma once
#include "StaticObject.h"
#include "AssetID.h"

constexpr float BRICK_BBOX_WIDTH = 14.0f;
constexpr float BRICK_BBOX_HEIGHT = 14.0f;

// Gạch có thể phá vỡ - Mario Big/Fire đụng từ dưới sẽ vỡ
class Brick : public StaticObject
{
private:
	int animationId = ANIMATION::BRICK_OVERWORLD;
	void Break();  // Phá gạch thành 4 mảnh
public:
	Brick(float x, float y, float z) : StaticObject(x, y, z) {
		this->state = 0;
	}
	Brick(float x, float y, float z, int animationId) : StaticObject(x, y, z) {
		this->state = 0;
		this->animationId = animationId;
	}
	virtual ~Brick() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr) override; 
	void Render() override ;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
	void OnMarioCollision(Mario* mario, LPCOLLISIONEVENT e) override;
};

