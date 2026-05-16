#pragma once
#include "GameObject.h"
#include "Scene.h"
class SwitchScenePoint : public GameObject {
private:
	Scene* targetScene;
public:
	SwitchScenePoint(float x, float y, float z) : GameObject(x, y, z){
		this->targetScene = nullptr;
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override {};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override;

	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnNoCollision(DWORD dt) override;
	void OnCollisionWithMario(LPCOLLISIONEVENT e);
	bool IsCollidable() { return true; }
	bool IsBlocking() { return false; }
};

