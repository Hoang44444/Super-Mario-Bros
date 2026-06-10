#include "Axe.h"
#include "AnimationManager.h"
#include "PlayScene.h"
#include "AssetID.h"

void Axe::Render()
{
	AnimationManager::GetInstance()->Get(ANIMATION::AXE)->Render(x, y, z);
}

void Axe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x; t = y;
	r = x + AXE_BBOX_WIDTH;
	b = y + AXE_BBOX_HEIGHT;
}

void Axe::OnMarioCollision(Mario* mario)
{
	PlayScene* playScene = dynamic_cast<PlayScene*>(scene);
	if (playScene != nullptr)
		playScene->StartCastleEndSequence();
	Delete();
}
