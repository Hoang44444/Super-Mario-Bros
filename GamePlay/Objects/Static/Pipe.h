#pragma once
#include "StaticObject.h"
#include "AssetID.h"

constexpr float PIPE_BBOX_WIDTH = 32.0f;
constexpr float PIPE_BBOX_HEIGHT = 32.0f;

// Ống (pipe) - vật tĩnh dùng để đi vào hoặc làm chướng ngại vật
class Pipe : public StaticObject
{
private:
	int animationId = ANIMATION::PIPE_OVERWORLD;  // Animation ID của ống
public:
	Pipe(float x, float y, float z) : StaticObject(x, y, z) {}
	Pipe(float x, float y, float z, int animationId) : StaticObject(x, y, z) {
		this->animationId = animationId;
	}
	virtual ~Pipe() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};