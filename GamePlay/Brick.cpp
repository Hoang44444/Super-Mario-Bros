#include "Brick.h"

void Brick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	// Bricks are static, so no update logic is needed
}

void Brick::Render() {
	// Render the brick using its position and state
}

void Brick::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}