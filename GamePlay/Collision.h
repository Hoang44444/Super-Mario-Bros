#pragma once
#include <Windows.h>
#include <vector>
#include "GameObject.h"

using namespace std;

struct CCollisionEvent
{
	LPGAMEOBJECT src_obj;		// The object that is moving
	LPGAMEOBJECT obj;			// The object it collided with
	float t, nx, ny;			// t: time of collision, nx/ny: collision normal (direction)

	float dx, dy;				// distance to collision

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL, LPGAMEOBJECT src_obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
		this->src_obj = src_obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

class Collision
{
public:
	static void SweptAABB(
		float ml, float mt, float mr, float mb,
		float dx, float dy,
		float sl, float st, float sr, float sb,
		float& t, float& nx, float& ny);

	static LPCOLLISIONEVENT Process(LPGAMEOBJECT src, DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};
