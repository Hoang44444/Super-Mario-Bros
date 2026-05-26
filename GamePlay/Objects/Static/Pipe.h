#pragma once
#include "StaticObject.h"

constexpr float PIPE_BBOX_WIDTH = 32.0f;
constexpr float PIPE_BBOX_HEIGHT = 32.0f;

class Pipe : public StaticObject {
public:
	Pipe(float x, float y, float z) : StaticObject(x, y, z) {}
	virtual ~Pipe() {};
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	void Render() override;
	void GetBoundingBox(float& l, float& t, float& r, float& b) override;
};