#pragma once
#include "InvisibleObject.h"
#include "Scene.h"
class SwitchScenePoint : public InvisibleObject {
private:
	Scene* targetScene;
public:
	SwitchScenePoint(float x, float y, float z) : InvisibleObject(x, y, z){
		this->targetScene = nullptr;
	}
	virtual ~SwitchScenePoint() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override;
	virtual void Render() override {};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) override {
		l = x;
		t = y;
		r = x + 32.0f;
		b = y + 32.0f;
	}
	virtual void OnMarioCollision(Mario* mario) override;
};

