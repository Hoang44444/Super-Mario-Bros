#pragma once

#include <Windows.h>
#include <vector>
#include <algorithm>

using namespace std;

class GameObject;
typedef GameObject* LPGAMEOBJECT;

struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;

// CollisionEvent đại diện cho một sự kiện va chạm giữa 2 object
// Sử dụng thuật toán SweptAABB để phát hiện va chạm trong tương lai
struct CollisionEvent
{
	LPGAMEOBJECT src_obj;		// Object nguồn (object đang di chuyển)
	LPGAMEOBJECT obj;			// Object đích (object tĩnh hoặc đang di chuyển khác)

	float t, nx, ny;			// t: thời gian va chạm (0-1), nx/ny: hướng va chạm (-1, 0, 1)

	float dx, dy;				// Khoảng cách di chuyển tương đối giữa 2 object
	bool isDeleted;				// Đã bị xóa chưa (để filter)

	CollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0,
		LPGAMEOBJECT obj = NULL, LPGAMEOBJECT src_obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
		this->src_obj = src_obj;
		this->isDeleted = false;
	}

	// Kiểm tra xem có thực sự va chạm không (t trong [0,1] và object có collidable)
	int WasCollided();

	// So sánh 2 collision event theo thời gian t (để sort)
	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};

// Collision class quản lý hệ thống va chạm sử dụng thuật toán SweptAABB
// Đây là singleton class xử lý tất cả các va chạm trong game
// Cách dùng:
//   1. Gọi Collision::GetInstance() để lấy singleton
//   2. Process() để xử lý va chạm cho một object với danh sách object khác
class Collision
{
	static Collision* __instance;  // Instance singleton
public:
	// SweptAABB cơ bản - tính toán va chạm giữa moving box và static box
	// ml/mt/mr/mb: moving box bounds
	// dx/dy: khoảng cách di chuyển
	// sl/st/sr/sb: static box bounds
	// t/nx/ny: output - thời gian va chạm và hướng va chạm
	static void SweptAABB(
		float ml,			// moving left
		float mt,			// moving top
		float mr,			// moving right
		float mb,			// moving bottom
		float dx,			// delta X
		float dy,			// delta Y
		float sl,			// static left
		float st,			// static top
		float sr,			// static right
		float sb,			// static bottom
		float& t,			// output: thời gian va chạm
		float& nx,			// output: hướng va chạm X
		float& ny);			// output: hướng va chạm Y

	// SweptAABB mở rộng - xử lý 2 object đều đang di chuyển
	// Trả về CollisionEvent chứa thông tin va chạm
	LPCOLLISIONEVENT SweptAABB(
		LPGAMEOBJECT objSrc,
		DWORD dt,
		LPGAMEOBJECT objDest);
	
	// Scan - quét tất cả object trong danh sách để tìm va chạm tiềm năng
	void Scan(
		LPGAMEOBJECT objSrc,
		DWORD dt,
		vector<LPGAMEOBJECT>* objDests,
		vector<LPCOLLISIONEVENT>& coEvents);

	// Filter - lọc collision events để tìm va chạm quan trọng nhất trên X và Y
	// filterBlock: chỉ lọc blocking collisions (1) hoặc tất cả (0)
	// filterX/Y: xử lý va chạm trên trục X/Y (1) hoặc bỏ qua (0)
	void Filter(
		LPGAMEOBJECT objSrc,
		vector<LPCOLLISIONEVENT>& coEvents,
		LPCOLLISIONEVENT& colX,
		LPCOLLISIONEVENT& colY,
		int filterBlock,
		int filterX,
		int filterY);

	// Process - xử lý va chạm hoàn chỉnh cho một object
	// Bao gồm: Scan -> Filter -> Update position -> OnCollisionWith
	void Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects);

	// Lấy instance singleton
	static Collision* GetInstance();
};