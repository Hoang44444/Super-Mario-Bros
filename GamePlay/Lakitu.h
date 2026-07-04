#pragma once
#include "Enemy.h"

#define LAKITU_FLY_SPEED 0.05f
#define LAKITU_GRAVITY 0.002f 
#define LAKITU_THROW_COOLDOWN 4000

#define LAKITU_BBOX_WIDTH 16
#define LAKITU_BBOX_HEIGHT 24

#define LAKITU_STATE_FLYING 100
#define LAKITU_STATE_DIE 200

// Sau khi kích hoạt, Lakitu neo ở góc TRÊN-PHẢI khung nhìn (theo camera). Khoảng thụt
// vào từ mép phải / mép trên để sprite không bị cắt.
#define LAKITU_CORNER_MARGIN_X 24.0f
#define LAKITU_CORNER_MARGIN_Y 24.0f
// Lúc kích hoạt, Lakitu bắt đầu ở phía trên đỉnh màn hình (ẩn) rồi bay xuống hover.
#define LAKITU_ENTRY_START_OFFSET 100.0f  // khởi điểm phía trên mép trên khung nhìn
#define LAKITU_ENTRY_SPEED 0.12f          // tốc độ bay từ đỉnh xuống vị trí hover
// Tốc độ Lakitu bay lên khỏi màn sau khi Mario đã vượt qua endX.
#define LAKITU_LEAVE_SPEED 0.15f

// Lakitu - enemy bay trên trời ném Spiny, theo Mario và kích hoạt khi Mario đến spawnX
class Lakitu : public Enemy
{
    bool isFacingRight;  // Hướng đang nhìn
    float spawnX;  // Mép phải màn chạm x này thì Lakitu kích hoạt (bay vào từ đỉnh)
    float endX;  // Mép phải màn vượt qua x này thì Lakitu bay đi
    bool activated = false;  // Mario đã tới spawnX kích hoạt Lakitu chưa
    bool entering = false;  // Đang bay từ đỉnh màn xuống vị trí hover
    bool leaving = false;  // Đang bay khỏi màn (Mario đã qua endX)
    ULONGLONG throw_start = 0;  // Tick khi bắt đầu throw Spiny

    int max_spinies_on_screen = 3;  // Số Spiny tối đa trên màn

public:
    Lakitu(float x, float y, float z = 0, float endX = 1e9f);
	
    virtual ~Lakitu() {}

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
    virtual void SetState(int state);

    virtual void OnNoCollision(DWORD dt) {}
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    virtual void OnMarioCollison(Mario* mario, float ny) override;
    virtual void OnHitByBullet() override { SetState(LAKITU_STATE_DIE); }
    virtual bool IsCollidable() override;
};
