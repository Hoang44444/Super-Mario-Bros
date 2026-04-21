#pragma once
#include <unordered_map>
#include "Animation.h"

using namespace std;

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
