#pragma once
        //
        // SCENE SECTION
        //
        constexpr int SCENE_SECTION_UNKNOWN = -1;
        constexpr int SCENE_SECTION_ASSETS = 1;
        constexpr int SCENE_SECTION_OBJECTS = 2;
        constexpr int SCENE_SECTION_MAP = 3;

        //
        // GAME SECTION
        //
        constexpr int GAME_SECTION_SETTINGS = 1;
        constexpr int GAME_SECTION_SCENES = 2;
        constexpr int GAME_SECTION_TEXTURES = 3;

        //
        // ASSET SECTION
        //
        constexpr int ASSET_SECTION_UNKNOWN = -1;
        constexpr int ASSET_SECTION_SPRITES = 1;
        constexpr int ASSET_SECTION_ANIMATIONS = 3;

        //
        // OBJECT TYPES
        //
        constexpr int OBJECT_TYPE_MARIO = 0;
        constexpr int OBJECT_TYPE_BRICK = 1;
        constexpr int OBJECT_TYPE_BULLET = 2;
        constexpr int OBJECT_TYPE_BACKGROUND = 3;
		constexpr int OBJECT_TYPE_PIPE = 4;
		constexpr int OBJECT_TYPE_SWITCH_SCENE_POINT = 5;

        //
        // TEXTURE IDs
        //
        constexpr int ID_TEX_BRICK = 0;
        constexpr int ID_TEX_MARIO = 1;
        constexpr int ID_TEX_MISC = 2;
        constexpr int ID_TEX_MENUBACKGROUND = 3;
        constexpr int ID_TEX_LEVEL1_1 = 4;
        constexpr int ID_TEX_BBOX = -100;

        //
        // ANIMATION IDs
        //
        constexpr int ID_ANI_BRICK = 100;
        constexpr int ID_ANI_BULLET = 200;
		constexpr int ID_ANI_CRACKED_BRICK = 300;
		// Background
        constexpr int ID_ANI_MENU_BACKGROUND = 300;
        constexpr int ID_ANI_BACKGROUND_LEVEL1_1 = 3000;


        // BIG MARIO
        constexpr int ID_ANI_MARIO_BIG_IDLE_RIGHT = 400;
        constexpr int ID_ANI_MARIO_BIG_IDLE_LEFT = 401;
        constexpr int ID_ANI_MARIO_BIG_WALKING_RIGHT = 500;
        constexpr int ID_ANI_MARIO_BIG_WALKING_LEFT = 501;
        constexpr int ID_ANI_MARIO_BIG_RUNNING_RIGHT = 600;
        constexpr int ID_ANI_MARIO_BIG_RUNNING_LEFT = 601;
        constexpr int ID_ANI_MARIO_BIG_JUMP_WALK_RIGHT = 700;
        constexpr int ID_ANI_MARIO_BIG_JUMP_WALK_LEFT = 701;
        constexpr int ID_ANI_MARIO_BIG_JUMP_RUN_RIGHT = 800;
        constexpr int ID_ANI_MARIO_BIG_JUMP_RUN_LEFT = 801;
        constexpr int ID_ANI_MARIO_BIG_SIT_RIGHT = 900;
        constexpr int ID_ANI_MARIO_BIG_SIT_LEFT = 901;
        constexpr int ID_ANI_MARIO_BIG_BRACE_RIGHT = 1000;
        constexpr int ID_ANI_MARIO_BIG_BRACE_LEFT = 1001;

        // SMALL MARIO
        constexpr int ID_ANI_MARIO_SMALL_IDLE_RIGHT = 1100;
        constexpr int ID_ANI_MARIO_SMALL_IDLE_LEFT = 1102;
        constexpr int ID_ANI_MARIO_SMALL_WALKING_RIGHT = 1200;
        constexpr int ID_ANI_MARIO_SMALL_WALKING_LEFT = 1201;
        constexpr int ID_ANI_MARIO_SMALL_RUNNING_RIGHT = 1300;
        constexpr int ID_ANI_MARIO_SMALL_RUNNING_LEFT = 1301;
        constexpr int ID_ANI_MARIO_SMALL_BRACE_RIGHT = 1400;
        constexpr int ID_ANI_MARIO_SMALL_BRACE_LEFT = 1401;
        constexpr int ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT = 1500;
        constexpr int ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT = 1501;
        constexpr int ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT = 1600;
        constexpr int ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT = 1601;

        constexpr int ID_ANI_MARIO_DIE = 999;

		// PIPE
		constexpr int ID_ANI_PIPE_HEAD = 3100;
		constexpr int ID_ANI_PIPE_BODY = 3101;
