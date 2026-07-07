#include "Collision.h"
#include "GameObject.h"

#include "debug.h"

// Hệ số để đẩy object ra khỏi va chạm (để tránh object bị kẹt trong nhau)
#define BLOCK_PUSH_FACTOR 0.01f

// Khởi tạo instance singleton tĩnh
Collision* Collision::__instance = NULL;

// Kiểm tra xem collision event có thực sự va chạm không
// Điều kiện: t trong [0,1] và object có collidable theo hướng nx/ny
int CollisionEvent::WasCollided() {
	return
		t >= 0.0f && t <= 1.0f && obj->IsDirectionColliable(nx, ny) == 1;
}

// Lấy instance singleton - tạo mới nếu chưa tồn tại
Collision* Collision::GetInstance()
{
	if (__instance == NULL) __instance = new Collision();
	return __instance;
}

/*
	SweptAABB - thuật toán va chạm cơ bản
	Tính toán thời điểm va chạm giữa moving box và static box
*/
void Collision::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t, float& nx, float& ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test - kiểm tra nhanh xem có thể va chạm không
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	// Nếu bounding box không giao nhau -> không va chạm
	if (br < sl || bl > sr || bb < st || bt > sb) return;


	// Nếu object không di chuyển -> không va chạm
	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	// Tính toán entry/exit distance cho trục X
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


	// Tính toán entry/exit distance cho trục Y
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

	// Tính toán entry/exit time cho trục X
	if (dx == 0)
	{
		tx_entry = -9999999.0f;
		tx_exit = 99999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	// Tính toán entry/exit time cho trục Y
	if (dy == 0)
	{
		ty_entry = -99999999999.0f;
		ty_exit = 99999999999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	// Nếu entry time âm hoặc > 1 -> không va chạm trong frame này
	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	// Tìm entry time lớn nhất (va chạm muộn nhất)
	t_entry = max(tx_entry, ty_entry);
	// Tìm exit time nhỏ nhất (ra khỏi sớm nhất)
	t_exit = min(tx_exit, ty_exit);

	// Nếu entry > exit -> không va chạm
	if (t_entry > t_exit) return;

	t = t_entry;

	// Xác định hướng va chạm (nx/ny)
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
	SweptAABB mở rộng - xử lý 2 object đều đang di chuyển
	Chuyển về bài toán 1 object tĩnh bằng cách trừ vận tốc
*/
LPCOLLISIONEVENT Collision::SweptAABB(LPGAMEOBJECT objSrc, DWORD dt, LPGAMEOBJECT objDest)
{
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	// Lấy vận tốc và khoảng cách di chuyển của object nguồn
	float mvx, mvy;
	objSrc->GetSpeed(mvx, mvy);
	float mdx = mvx * dt;
	float mdy = mvy * dt;

	// Lấy vận tốc và khoảng cách di chuyển của object đích
	float svx, svy;
	objDest->GetSpeed(svx, svy);
	float sdx = svx * dt;
	float sdy = svy * dt;

	//
	// Chuyển về bài toán 1 object tĩnh: trừ vận tốc
	// new m speed = original m speed - collide object speed
	// 
	float dx = mdx - sdx;
	float dy = mdy - sdy;

	objSrc->GetBoundingBox(ml, mt, mr, mb);
	objDest->GetBoundingBox(sl, st, sr, sb);

	// Gọi SweptAABB cơ bản
	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	// Tạo và trả về CollisionEvent
	CollisionEvent* e = new CollisionEvent(t, nx, ny, dx, dy, objDest, objSrc);
	return e;
}

/*
	Scan - quét tất cả object trong danh sách để tìm va chạm tiềm năng

	objSrc: object nguồn (đang kiểm tra va chạm)
	dt: thời gian giữa 2 frame (ms)
	objDests: danh sách object đích để kiểm tra va chạm
	coEvents: danh sách các collision event tiềm năng (output)
*/
void Collision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests, vector<LPCOLLISIONEVENT>& coEvents)
{
	// Duyệt qua tất cả object đích
	for (UINT i = 0; i < objDests->size(); i++)
	{
		// Tính toán va chạm với object này
		LPCOLLISIONEVENT e = SweptAABB(objSrc, dt, objDests->at(i));

		// Nếu có va chạm -> thêm vào danh sách
		if (e->WasCollided() == 1)
			coEvents.push_back(e);
		else
			delete e;  // Không va chạm -> xóa event
	}

	// Có thể sort theo thời gian va chạm (commented out)
	//std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

// Filter - lọc collision events để tìm va chạm quan trọng nhất trên X và Y
// filterBlock: chỉ lọc blocking collisions (1) hoặc tất cả (0)
// filterX/Y: xử lý va chạm trên trục X/Y (1) hoặc bỏ qua (0)
void Collision::Filter(LPGAMEOBJECT objSrc,
	vector<LPCOLLISIONEVENT>& coEvents,
	LPCOLLISIONEVENT& colX,
	LPCOLLISIONEVENT& colY,
	int filterBlock = 1,		// 1 = only filter block collisions, 0 = filter all collisions 
	int filterX = 1,			// 1 = process events on X-axis, 0 = skip events on X 
	int filterY = 1)			// 1 = process events on Y-axis, 0 = skip events on Y
{
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	// Duyệt qua tất cả collision events
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];
		if (c->isDeleted) continue;
		if (c->obj->IsDeleted()) continue;

		// Bỏ qua non-blocking collisions (coin, mushroom, v.v.) nếu filterBlock = 1
		if (filterBlock == 1 && !c->obj->IsBlocking())
		{
			continue;
		}

		// Tìm va chạm sớm nhất trên trục X
		if (c->t < min_tx && c->nx != 0 && filterX == 1) {
			min_tx = c->t; min_ix = i;
		}

		// Tìm va chạm sớm nhất trên trục Y
		if (c->t < min_ty && c->ny != 0 && filterY == 1) {
			min_ty = c->t; min_iy = i;
		}
	}

	// Gán kết quả
	if (min_ix >= 0) colX = coEvents[min_ix];
	if (min_iy >= 0) colY = coEvents[min_iy];
}

/*
*  Process - xử lý va chạm hoàn chỉnh cho một object
*  Bao gồm: Scan -> Filter -> Update position -> OnCollisionWith
*  NOTE: Có thể cần cải thiện dựa trên game logic cụ thể
*/
void Collision::UpdatePositionFromCollisions(LPGAMEOBJECT objSrc, LPCOLLISIONEVENT colX, LPCOLLISIONEVENT colY, float min_tx, float min_ty, float nx, float ny)
{
	float x, y, z, vx, vy;
	objSrc->GetPosition(x, y, z);
	objSrc->GetSpeed(vx, vy);

	if (min_tx <= min_ty)
	{
		if (colX != NULL)
		{
			x += colX->t * colX->dx + nx * 0.1f;
			y += colX->t * colX->dy + ny * 0.1f;
		}
		else
		{
			x += min_tx * vx + nx * 0.1f;
			y += min_ty * vy + ny * 0.1f;
		}
	}
	else
	{
		if (colY != NULL)
		{
			x += colY->t * colY->dx + nx * 0.1f;
			y += colY->t * colY->dy + ny * 0.1f;
		}
		else
		{
			x += min_tx * vx + nx * 0.1f;
			y += min_ty * vy + ny * 0.1f;
		}
	}

	objSrc->SetPosition(x, y, z);
}

void Collision::HandleNonBlockingCollisions(LPGAMEOBJECT objSrc, vector<LPCOLLISIONEVENT>& coEvents, LPCOLLISIONEVENT colX, LPCOLLISIONEVENT colY)
{
	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT e = coEvents[i];
		if (e->isDeleted) continue;
		if (e->obj->IsBlocking()) continue;
		if (e == colX || e == colY) continue;

		objSrc->OnCollisionWith(e);
	}
}

void Collision::Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	LPCOLLISIONEVENT colX = NULL;
	LPCOLLISIONEVENT colY = NULL;

	coEvents.clear();

	// Bước 1: Scan để tìm tất cả va chạm tiềm năng
	if (objSrc->IsCollidable())
	{
		Scan(objSrc, dt, coObjects, coEvents);
	}

	// Bước 2: Nếu không có va chạm -> OnNoCollision
	if (coEvents.size() == 0)
	{
		objSrc->OnNoCollision(dt);
	}
	else
	{
		// Bước 3: Filter để tìm va chạm quan trọng nhất trên X và Y
		Filter(objSrc, coEvents, colX, colY);

		// Lấy position và velocity hiện tại
		float x, y, z, vx, vy, dx, dy;
		objSrc->GetPosition(x, y, z);
		objSrc->GetSpeed(vx, vy);
		dx = vx * dt;
		dy = vy * dt;

		// Bước 4: Xử lý va chạm trên cả 2 trục
		if (colX != NULL && colY != NULL)
		{
			if (colY->t < colX->t)	// Va chạm trên Y xảy ra trước
			{
				// Cập nhật position theo va chạm Y
				y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
				objSrc->SetPosition(x, y, z);

				// Callback va chạm Y
				objSrc->OnCollisionWith(colY);

				// Kiểm tra xem sau khi sửa Y, có còn va chạm X không?
				LPCOLLISIONEVENT colX_other = NULL;

				// Xóa va chạm X cũ
				colX->isDeleted = true;

				// Tính lại va chạm X với position đã sửa
				coEvents.push_back(SweptAABB(objSrc, dt, colX->obj));

				// Filter lại chỉ trên X
				Filter(objSrc, coEvents, colX_other, colY, /*filterBlock = */ 1, 1, /*filterY=*/0);

				if (colX_other != NULL)
				{
					// Có va chạm X -> cập nhật position
					x += colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colX_other);
				}
				else
				{
					// Không có va chạm X -> di chuyển bình thường
					x += dx;
				}
			}
			else // Va chạm trên X xảy ra trước
			{
				// Cập nhật position theo va chạm X
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
				objSrc->SetPosition(x, y, z);

				// Callback va chạm X
				objSrc->OnCollisionWith(colX);

				// Kiểm tra xem sau khi sửa X, có còn va chạm Y không?
				LPCOLLISIONEVENT colY_other = NULL;

				// Xóa va chạm Y cũ
				colY->isDeleted = true;

				// Tính lại va chạm Y với position đã sửa
				coEvents.push_back(SweptAABB(objSrc, dt, colY->obj));

				// Filter lại chỉ trên Y
				Filter(objSrc, coEvents, colX, colY_other, /*filterBlock = */ 1, /*filterX=*/0, /*filterY=*/1);

				if (colY_other != NULL)
				{
					// Có va chạm Y -> cập nhật position
					y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colY_other);
				}
				else
				{
					// Không có va chạm Y -> di chuyển bình thường
					y += dy;
				}
			}
		}
		else
			// Chỉ có va chạm trên 1 trục
			if (colX != NULL)
			{
				x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
				y += dy;
				objSrc->OnCollisionWith(colX);
			}
			else
				if (colY != NULL)
				{
					x += dx;
					y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
					objSrc->OnCollisionWith(colY);
				}
				else // Không có va chạm nào
				{
					x += dx;
					y += dy;
				}

		// Set position cuối cùng
		objSrc->SetPosition(x, y, z);
	}

	//
	// Bước 5: Xử lý non-blocking collisions (coin, mushroom, v.v.)
	//
	HandleNonBlockingCollisions(objSrc, coEvents, colX, colY);


	// Cleanup: xóa tất cả collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}
