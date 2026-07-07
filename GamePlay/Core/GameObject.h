#pragma once
#include <vector>
#include <Windows.h>
#include "Collision.h"
#include "Scene.h"

using namespace std;


struct CollisionEvent;
typedef CollisionEvent* LPCOLLISIONEVENT;

// GameObject là base class cho tất cả object trong game
// Mọi object (Mario, enemy, item, platform) đều kế thừa từ class này
// Cung cấp các thuộc tính cơ bản: position, velocity, state, collision
class GameObject
{
protected:
    float x, y, z;           // Vị trí trong world space
    float vx, vy;            // Vận tốc (velocity)
    int direction;           // Hướng (-1: trái, 1: phải)
    int state;               // Trạng thái của object (state machine)
    bool isDeleted;          // Đã bị xóa chưa (để cleanup)
	bool isVisible;          // Có hiển thị không
	Scene* scene;            // Reference đến scene chứa object này

public:
    // Constructor mặc định - khởi tạo các giá trị mặc định
    GameObject() {
        this->x = this->y = this->z = 0;
        this->vx = this->vy = 0;
        this->direction = 1;
        this->state = -1;
        this->isDeleted = false;
		this->isVisible = true;
		scene = nullptr;
    };
    
    // Constructor với vị trí khởi tạo
    GameObject(float x, float y, float z) : GameObject() { this->x = x; this->y = y; this->z = z; };

    virtual ~GameObject() {};

    // --- POSITION ---
    void SetPosition(float x, float y, float z) { this->x = x; this->y = y; this->z = z; };
    void GetPosition(float& x, float& y, float& z) { x = this->x; y = this->y; z = this->z; };

    // --- SPEED ---
    void SetSpeed(float vx, float vy) { this->vx = vx; this->vy = vy; };
    void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; };

    // --- STATE ---
    int GetState() { return this->state; };
    virtual void SetState(int state) { this->state = state; };

    // --- LIFECYCLE ---
    virtual void Delete() { this->isDeleted = true; };
    bool IsDeleted() { return this->isDeleted; };

    // --- VISIBILITY ---
    void SetVisible(bool visible) { this->isVisible = visible; }
    bool IsVisible() { return this->isVisible; }

    // --- CORE GAME LOOP ---
    // Update logic mỗi frame - dt là thời gian giữa 2 frame (ms)
    // coObjects: danh sách object để kiểm tra va chạm (optional)
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) = 0;
    
    // Render object lên màn hình
    virtual void Render() = 0;

    // --- COLLISION ---
    // Lấy bounding box của object (l/t/r/b)
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b) = 0;
    
    // Object có thể va chạm không (true = có, false = không)
    virtual bool IsCollidable() { return false; }
    
    // Object có chặn va chạm không (true = chặn, false = đi qua được)
    // Ví dụ: platform = true, coin = false
    virtual bool IsBlocking() { return true; }

    // Callback khi không có va chạm nào xảy ra
    virtual void OnNoCollision(DWORD dt) {}
    
    // Callback khi có va chạm với object khác
    virtual void OnCollisionWith(LPCOLLISIONEVENT e) {}

    // --- EMERGE ANIMATION ---
    // Bắt đầu animation "nhảy ra khỏi block"
    // No-op mặc định; được override bởi item có thể emerge từ question block
    virtual void StartEmerge() {}

    // Static helper để kiểm tra object đã bị xóa chưa (dùng cho algorithm)
    static bool IsDeleted(const LPGAMEOBJECT& o) { return o->isDeleted; };
    
    // Kiểm tra object có collidable theo hướng nx/ny không
    // Return 1 = có, 0 = không
    virtual int IsDirectionColliable(float nx, float ny) { return 1; }

	// --- SCENE MANAGEMENT ---
	void SetScene(Scene* s) { this->scene = s; }
};
typedef GameObject* LPGAMEOBJECT;
