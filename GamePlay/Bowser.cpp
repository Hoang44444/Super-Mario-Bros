#include "Bowser.h"
#include "BowserFire.h"
#include "Mario.h"
#include "PlayScene.h"
#include "AnimationManager.h"
#include "Camera.h"
#include "../Resource/AssetID.h"
#include "../../Resource/SoundManager.h"

Bowser::Bowser(float x, float y, float z) : Enemy(x, y, z)
{
    this->startX = x;
    jump_timer = GetTickCount64();
    fire_timer = GetTickCount64();
    SetState(BOWSER_STATE_WALKING);
}

void Bowser::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x; t = y; r = l + BOWSER_BBOX_WIDTH; b = t + BOWSER_BBOX_HEIGHT;
}

void Bowser::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void Bowser::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (e->ny != 0 && e->obj->IsBlocking())
    {
        vy = 0;
        if (e->ny < 0) isOnGround = true;
    }
    else if (e->nx != 0 && e->obj->IsBlocking())
    {
        vx = -vx;
    }
}

void Bowser::OnMarioCollison(Mario* mario, float ny)
{
    if (mario->GetState() == MARIO_STATE_DIE) return;

    mario->TakeDamage();
}

void Bowser::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (state == BOWSER_STATE_DIE)
    {
        // Pha 1: đứng nhấp nháy tại chỗ trong BOWSER_DIE_BLINK_TIME (không di chuyển).
        if (GetTickCount64() - die_timer < BOWSER_DIE_BLINK_TIME)
        {
            vx = 0;
            vy = 0;
            return;
        }

        // Pha 2: rơi thẳng xuống, BỎ QUA va chạm -> rơi xuyên dù cầu còn hay đã sập.
        vy += BOWSER_GRAVITY * dt;
        y += vy * dt;

        // Pha 3: rơi khỏi màn -> tự xóa (biến mất).
        float cy = Camera::GetInstance()->GetY();
        const float SCREEN_HEIGHT = 600.0f;
        const float CULL_BUFFER = 100.0f;
        if (y > cy + SCREEN_HEIGHT + CULL_BUFFER)
            Delete();

        return;
    }

    vy += BOWSER_GRAVITY * dt;

    // Mario chưa lên cầu -> Bowser chưa kích hoạt: đứng yên tại chỗ (vẫn chịu trọng lực
    // để đứng trên cầu), không đi/nhảy/phun lửa.
    if (!active)
    {
        vx = 0;
        Collision::GetInstance()->Process(this, dt, coObjects);
        return;
    }

    Mario* mario = NULL;
    for (size_t i = 0; i < coObjects->size(); i++)
    {
        if (dynamic_cast<Mario*>(coObjects->at(i))) {
            mario = dynamic_cast<Mario*>(coObjects->at(i));
            break;
        }
    }

    bool isTrackingMario = false;

    if (mario != NULL && mario->GetState() != MARIO_STATE_DIE)
    {
        float mx, my, mz;
        mario->GetPosition(mx, my, mz);

        float distanceToMario = abs(mx - this->x);
        const float TRACKING_RADIUS = 150.0f;

        // If Mario is close, lock on!
        if (distanceToMario < TRACKING_RADIUS)
        {
            this->direction = (mx > this->x) ? 1 : -1;
            isTrackingMario = true;
        }
    }

    // 3. Patrol Mode (If Mario is dead, null, or far away)
    if (!isTrackingMario)
    {
        if (this->x > startX + 50.0f) this->direction = -1;
        else if (this->x < startX - 50.0f) this->direction = 1;
    }

    if (state == BOWSER_STATE_WALKING)
    {
        vx = BOWSER_WALK_SPEED * this->direction;

        if (isOnGround && GetTickCount64() - jump_timer > 3000 + (rand() % 2000))
        {
            vy = -BOWSER_JUMP_SPEED;
            jump_timer = GetTickCount64();
            isOnGround = false;
        }

        if (GetTickCount64() - fire_timer > current_fire_cooldown)
        {
            current_fire_cooldown = 4000 + (rand() % 3000);
            SetState(BOWSER_STATE_ATTACKING);
        }
    }

    if (state == BOWSER_STATE_ATTACKING && GetTickCount64() - attack_duration > 500)
    {
        SetState(BOWSER_STATE_WALKING);
        fire_timer = GetTickCount64();
    }

    Collision::GetInstance()->Process(this, dt, coObjects);
}

void Bowser::SetState(int state)
{
    GameObject::SetState(state);
    switch (state)
    {
    case BOWSER_STATE_WALKING:
        vx = BOWSER_WALK_SPEED * this->direction;
        break;

    case BOWSER_STATE_ATTACKING:
        {
            vx = 0;
            attack_duration = GetTickCount64();

            BowserFire* fire = new BowserFire(this->x, this->y + 12.0f, this->z, this->direction);

            PlayScene* scene = dynamic_cast<PlayScene*>(this->scene);
            if (scene != NULL) scene->AddObject(fire);

            SoundManager::GetInstance()->PlaySFX(SFX::BOWSER_FIRE);
        }
        break;

    case BOWSER_STATE_DIE:
        vx = 0;
        vy = 0;
        die_timer = GetTickCount64();   // bắt đầu đếm pha nhấp nháy 2s
        SoundManager::GetInstance()->PlaySFX(SFX::BOWSER_FALLS);
        break;
    }
}

void Bowser::Render()
{
    // Nhấp nháy khi vừa chết: bỏ vẽ xen kẽ theo BOWSER_DIE_BLINK_INTERVAL để tạo hiệu ứng chớp.
    if (state == BOWSER_STATE_DIE)
    {
        ULONGLONG elapsed = GetTickCount64() - die_timer;
        if (elapsed < BOWSER_DIE_BLINK_TIME
            && (elapsed / BOWSER_DIE_BLINK_INTERVAL) % 2 == 0)
            return;
    }

    int aniId = ANIMATION::BOWSER_WALK_LEFT;

    if (state == BOWSER_STATE_ATTACKING)
    {
        aniId = (direction > 0) ? ANIMATION::BOWSER_ATTACK_RIGHT : ANIMATION::BOWSER_ATTACK_LEFT;
    }
    else
    {
        aniId = (direction > 0) ? ANIMATION::BOWSER_WALK_RIGHT : ANIMATION::BOWSER_WALK_LEFT;
    }

    LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
    if (ani != NULL)
    {
        ani->Render(x, y, z);
    }
}