#include "Pipe.h"

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PIPE_BBOX_WIDTH;
	b = y + PIPE_BBOX_HEIGHT;
}