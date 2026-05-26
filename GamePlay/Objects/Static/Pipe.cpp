#include"Pipe.h"

void Pipe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	// Pipes are static, so no update logic is needed
}

void Pipe::Render() {
	// Render the pipe using its position and state
}

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b) {
	l = x;
	t = y;
	r = x + PIPE_BBOX_WIDTH;
	b = y + PIPE_BBOX_HEIGHT;
}