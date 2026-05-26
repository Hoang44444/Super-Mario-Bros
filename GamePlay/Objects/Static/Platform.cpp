#include "Platform.h"

void Platform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
}

void Platform::Render() {
}

void Platform::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + PLATFORM_BBOX_WIDTH;
	b = y + PLATFORM_BBOX_HEIGHT;
}
