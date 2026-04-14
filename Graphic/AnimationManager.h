#pragma once
#include "Animation.h"
#include <unordered_map>

class AnimationManager
{
	static AnimationManager* __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();

	static AnimationManager* GetInstance();
};

