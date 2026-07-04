#include "Pipe.h"
#include "AnimationManager.h"
#include "AssetID.h"

void Pipe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	// Ống là vật tĩnh, không cần update logic
}

void Pipe::Render() {
	AnimationManager::GetInstance()->Get(animationId)->Render(x, y, z);
}

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + PIPE_BBOX_WIDTH;
	b = y + PIPE_BBOX_HEIGHT;
}