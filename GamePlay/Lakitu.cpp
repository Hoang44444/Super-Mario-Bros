#include "Lakitu.h"
#include "Spiny.h"
#include "BuzzyBeetle.h"
#include "Koopa.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Camera.h"
#include "AnimationManager.h"
#include "../Resource/AssetID.h"
#include "Core/ScoreManager.h"

Lakitu::Lakitu(float x, float y, float z, float endX) : Enemy(x, y, z)
{
    this->spawnX = x;
    this->endX = endX;
    SetState(LAKITU_STATE_FLYING);
    throw_start = GetTickCount64();
}

void Lakitu::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x; t = y; r = l + LAKITU_BBOX_WIDTH; b = t + LAKITU_BBOX_HEIGHT;
}

Mario* Lakitu::FindMario(vector<LPGAMEOBJECT>* coObjects)
{
	for (size_t i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Mario*>(coObjects->at(i)))
		{
			return dynamic_cast<Mario*>(coObjects->at(i));
		}
	}
	return nullptr;
}

int Lakitu::CountActiveSpinies(vector<LPGAMEOBJECT>* coObjects, float camX)
{
	const float SCREEN_WIDTH = 800.0f;
	int activeSpinies = 0;

	for (size_t i = 0; i < coObjects->size(); i++)
	{
		Spiny* spiny = dynamic_cast<Spiny*>(coObjects->at(i));

		if (spiny != nullptr && !spiny->IsDeleted())
		{
			float sx, sy, sz;
			spiny->GetPosition(sx, sy, sz);

			if (sx >= camX && sx <= camX + SCREEN_WIDTH)
			{
				activeSpinies++;
			}
		}
	}
	return activeSpinies;
}

void Lakitu::HandleActivationAndEntry(float anchorX, float camY, float hoverY, DWORD dt)
{
	// Chưa kích hoạt: nấp phía trên đỉnh màn (ẩn) cho tới khi vị trí Lakitu chạm spawnX.
	if (!activated)
	{
		this->x = anchorX;
		this->y = camY - LAKITU_ENTRY_START_OFFSET;

		if (anchorX >= spawnX)
		{
			activated = true;
			entering = true;
		}
		return;
	}

	// Đã kích hoạt nhưng Mario lùi làm vị trí Lakitu ra khỏi mép trái vùng (< spawnX):
	// ngừng di chuyển và ngừng ném, lơ lửng đứng yên tại chỗ tới khi trở lại vùng.
	if (anchorX < spawnX)
	{
		return;
	}

	// Trong vùng [spawnX, endX]: neo theo mép phải màn hình.
	this->x = anchorX;

	// Đang bay từ đỉnh màn xuống vị trí hover: chưa ném Spiny cho tới khi vào vị trí.
	if (entering)
	{
		this->y += LAKITU_ENTRY_SPEED * dt;
		if (this->y >= hoverY)
		{
			this->y = hoverY;
			entering = false;
		}
	}
}

void Lakitu::HandleLeaving(float camY, DWORD dt)
{
	y -= LAKITU_LEAVE_SPEED * dt;

	const float CULL_BUFFER = 100.0f;
	if (this->y < camY - CULL_BUFFER)
		this->Delete();
}

void Lakitu::HandleHoverAndThrow(vector<LPGAMEOBJECT>* coObjects, float anchorX, float hoverY, float mx)
{
	// Hover ổn định ở góc trên-phải.
	this->x = anchorX;
	this->y = hoverY;
	isFacingRight = (mx >= this->x);

	if (GetTickCount64() - throw_start > LAKITU_THROW_COOLDOWN)
	{
		throw_start = GetTickCount64();

		float cx = Camera::GetInstance()->GetX();

		int activeSpinies = CountActiveSpinies(coObjects, cx);

		if (activeSpinies < 4)
		{
			// Trứng thả ngay tại vị trí Lakitu (góc trên-phải), rơi thẳng đứng xuống.
			Spiny* egg = new Spiny(this->x, this->y, this->z);
			egg->SetState(SPINY_STATE_EGG);

			int lakituDirection = (mx >= this->x) ? 1 : -1;   // spiny đi về phía Mario sau khi tiếp đất
			egg->SetFacingDirection(lakituDirection);

			PlayScene* currentScene = dynamic_cast<PlayScene*>(this->scene);
			if (currentScene != nullptr)
			{
				currentScene->AddObject(egg);
			}
		}
	}
}

void Lakitu::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (this->vx > 0) isFacingRight = true;
    else if (this->vx < 0) isFacingRight = false;

    if (state == LAKITU_STATE_DIE)
    {
        vy += LAKITU_GRAVITY * dt;
        y += vy * dt;

        float cy = Camera::GetInstance()->GetY();
        const float SCREEN_HEIGHT = 600.0f;
        const float CULL_BUFFER = 100.0f;

        if (this->y > cy + SCREEN_HEIGHT + CULL_BUFFER)
        {
            this->Delete();
        }

        return;
    }

    // Mario đã vượt qua endX: Lakitu bay lên khỏi màn rồi biến mất, không bám/ném nữa.
    if (leaving)
    {
        y -= LAKITU_LEAVE_SPEED * dt;

        float cy = Camera::GetInstance()->GetY();
        const float CULL_BUFFER = 100.0f;
        if (this->y < cy - CULL_BUFFER)
            this->Delete();

        return;
    }

    Mario* mario = FindMario(coObjects);

    if (mario == nullptr || mario->GetState() == MARIO_STATE_DIE)
        return;

    float mx, my, mz;
    mario->GetPosition(mx, my, mz);

    // Vị trí neo góc TRÊN-PHẢI khung nhìn (theo camera) và độ cao hover cuối cùng.
    int screenW = 0, screenH = 0;
    Camera::GetInstance()->GetSize(screenW, screenH);
    float camX = Camera::GetInstance()->GetX();
    float camY = Camera::GetInstance()->GetY();

    float anchorX = camX + (float)screenW - LAKITU_BBOX_WIDTH - LAKITU_CORNER_MARGIN_X;
    float hoverY  = camY + LAKITU_CORNER_MARGIN_Y;

    // Lakitu chỉ hoạt động khi VỊ TRÍ của nó (mép phải màn) nằm trong vùng [spawnX, endX].
    // Vị trí Lakitu vượt qua endX -> bay đi vĩnh viễn.
    if (anchorX > endX)
    {
        leaving = true;
        return;
    }

    // Handle activation and entry
    HandleActivationAndEntry(anchorX, camY, hoverY, dt);

    // Handle leaving
    if (leaving)
    {
        HandleLeaving(camY, dt);
        return;
    }

    // Handle hover and throw
    HandleHoverAndThrow(coObjects, anchorX, hoverY, mx);
}

bool Lakitu::IsCollidable()
{
    if (state == LAKITU_STATE_DIE) return false;
    return true;
}

void Lakitu::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (dynamic_cast<Koopa*>(e->obj))
    {
        Koopa* koopa = dynamic_cast<Koopa*>(e->obj);
        if (koopa->GetState() == KOOPA_STATE_SPINNING)
        {
            this->SetState(LAKITU_STATE_DIE);
            // Award score for shell kill (no combo for shell kills)
            ScoreManager::Get().AddScore(SCORE_VALUES::SHELL_KICK);
            return;
        }
    }
    else if (dynamic_cast<BuzzyBeetle*>(e->obj))
    {
        BuzzyBeetle* beetle = dynamic_cast<BuzzyBeetle*>(e->obj);
        if (beetle->GetState() == BUZZY_STATE_SPINNING)
        {
            this->SetState(LAKITU_STATE_DIE);
            // Award score for shell kill (no combo for shell kills)
            ScoreManager::Get().AddScore(SCORE_VALUES::SHELL_KICK);
            return;
        }
    }
}

void Lakitu::OnMarioCollison(Mario* mario, float ny)
{
    if (mario->GetState() == MARIO_STATE_DIE) return;

    if (state == LAKITU_STATE_FLYING)
    {
        if (ny < 0)
        {
            SetState(LAKITU_STATE_DIE);
            float mvx, mvy;
            mario->GetSpeed(mvx, mvy);
            mario->SetSpeed(mvx, -0.4f);

            // Award score with combo system for stomp
            ScoreManager& scoreMgr = ScoreManager::Get();
            scoreMgr.IncrementCombo();
            
            if (scoreMgr.IsMaxCombo())
            {
                mario->AddLife(1);
            }
            else
            {
                mario->AddScore(scoreMgr.GetComboScore());
            }
        }
        else
        {
            mario->TakeDamage();
        }
    }
}

void Lakitu::SetState(int state)
{
    GameObject::SetState(state);
    switch (state)
    {
    case LAKITU_STATE_FLYING:
        vx = -LAKITU_FLY_SPEED;
        vy = 0;
        break;
    case LAKITU_STATE_DIE:
        vx = 0;
        vy = 0.2f;
        break;
    }
}

void Lakitu::Render()
{
    int aniId = ANIMATION::LAKITU_FLY_RIGHT;
    if (state == LAKITU_STATE_FLYING || state == LAKITU_STATE_DIE)
    {
        if (isFacingRight) aniId = ANIMATION::LAKITU_FLY_RIGHT;
        else aniId = ANIMATION::LAKITU_FLY_LEFT;
    }
    
    LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
    if (ani != NULL) ani->Render(x, y, z);
}