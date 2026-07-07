#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "GameObject.h"
#include "AnimationManager.h"
#include "Renderer.h"
#include "Camera.h"
#include "debug.h"
#include "AssetID.h"

struct MenuOptionEntry
{
	int   aniWhite;
	int   aniYellow;
	float w, h;
	float posY;
	int   targetScene;
};

class MenuOptions : public GameObject
{
	std::vector<MenuOptionEntry> options;
	int selected = 0;

public:
	MenuOptions(float x, float y, float z, LPCWSTR optionFile) : GameObject(x, y, z)
	{
		LoadOptions(optionFile);
	}

	int  GetCount() const { return (int)options.size(); }
	int  GetSelected() const { return selected; }

	void MoveSelection(int delta)
	{
		int n = (int)options.size();
		if (n <= 0) return;
		selected = ((selected + delta) % n + n) % n;
	}

	int  GetSelectedTarget() const
	{
		if (selected < 0 || selected >= (int)options.size()) return -1;
		return options[selected].targetScene;
	}

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) override {}
	void GetBoundingBox(float& l, float& t, float& r, float& b) override { l = t = r = b = 0; }
	bool IsCollidable() override { return false; }
	bool IsBlocking() override { return false; }

	void Render() override
	{
		Renderer* r = Renderer::GetInstance();
		float scale = r->GetGlobalScale();
		float logicalW = r->GetBackBufferWidth() / scale;
		float camX = Camera::GetInstance()->GetX();
		float camY = Camera::GetInstance()->GetY();

		for (int i = 0; i < (int)options.size(); i++)
		{
			const MenuOptionEntry& o = options[i];
			int aniId = (i == selected) ? o.aniYellow : o.aniWhite;

			LPANIMATION ani = AnimationManager::GetInstance()->Get(aniId);
			if (ani == nullptr) continue;

			float drawX = camX + (logicalW - o.w) / 2.0f;
			float drawY = camY + o.posY;
			ani->RenderScaled(drawX, drawY, z, o.w, o.h);
		}
	}

private:
	void LoadOptions(LPCWSTR optionFile)
	{
		std::ifstream f;
		f.open(optionFile);
		if (!f.is_open())
		{
			DebugOut(L"[ERROR] Failed to open menu options file: %s\n", optionFile);
			return;
		}

		char str[1024];
		while (f.getline(str, 1024))
		{
			std::string line(str);
			if (!line.empty() && line.back() == '\r') line.pop_back();
			if (line.empty() || line[0] == '#') continue;

			std::stringstream ss(line);
			std::vector<std::string> tk;
			std::string t;
			while (ss >> t)
			{
				if (!t.empty() && t[0] == '#') break;
				tk.push_back(t);
			}
			if (tk.size() < 6) continue;

			MenuOptionEntry o;
			o.aniWhite    = atoi(tk[0].c_str());
			o.aniYellow   = atoi(tk[1].c_str());
			o.w           = (float)atof(tk[2].c_str());
			o.h           = (float)atof(tk[3].c_str());
			o.posY        = (float)atof(tk[4].c_str());
			o.targetScene = atoi(tk[5].c_str());
			options.push_back(o);
		}
		f.close();

		DebugOut(L"[INFO] Loaded %d menu option(s) from %s\n", (int)options.size(), optionFile);
	}
};
