#include "Collision.h"
#include <algorithm>
#include <limits>

/*
	Standard SweptAABB algorithm
	Returns t (time of collision), nx, ny (collision normal)
*/
void Collision::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{
	float dx_entry, dx_exit;
	float dy_entry, dy_exit;

	float tx_entry, tx_exit;
	float ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	// 
	// Broad-phase test 
	//
	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}
	else
	{
		dx_entry = 0;
		dx_exit = 0;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}
	else
	{
		dy_entry = 0;
		dy_exit = 0;
	}

	if (dx == 0)
	{
		tx_entry = -std::numeric_limits<float>::infinity();
		tx_exit = std::numeric_limits<float>::infinity();
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0)
	{
		ty_entry = -std::numeric_limits<float>::infinity();
		ty_exit = std::numeric_limits<float>::infinity();
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if (tx_entry > ty_entry)
	{
		t_entry = tx_entry;
		t_exit = ty_exit;
	}
	else
	{
		t_entry = ty_entry;
		t_exit = tx_exit;
	}

	if (t_entry > t_exit || (tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

/*
	Process collision between a source object and a list of co-objects
*/
LPCOLLISIONEVENT Collision::Process(LPGAMEOBJECT src, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	float sl, st, sr, sb;		// src bounding box
	float ml, mt, mr, mb;		// static object bounding box
	float dx, dy;				// distance moved in this frame

	src->GetBoundingBox(sl, st, sr, sb);
	src->GetSpeed(dx, dy);
	dx *= dt;
	dy *= dt;

	float min_t = 1.0f;
	float min_nx = 0, min_ny = 0;
	LPGAMEOBJECT col_obj = NULL;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (obj->IsCollidable())
		{
			float t, nx, ny;
			obj->GetBoundingBox(ml, mt, mr, mb);

			SweptAABB(ml, mt, mr, mb, -dx, -dy, sl, st, sr, sb, t, nx, ny);

			if (t > 0 && t < min_t)
			{
				min_t = t;
				min_nx = nx;
				min_ny = ny;
				col_obj = obj;
			}
		}
	}

	if (col_obj != NULL)
		return new CCollisionEvent(min_t, min_nx, min_ny, dx, dy, col_obj, src);
	
	return NULL;
}
