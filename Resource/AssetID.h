#pragma once

namespace SCENE
{
    constexpr int SECTION_UNKNOWN = -1;
    constexpr int SECTION_ASSETS = 1;
    constexpr int SECTION_OBJECTS = 2;
    constexpr int SECTION_MAP = 3;

    constexpr int MENU = 0;
    constexpr int WORLD_1_1 = 1;
    constexpr int WORLD_1_2 = 2;
    constexpr int WORLD_1_3 = 3;
    constexpr int WORLD_1_4 = 4;
    constexpr int WORLD_2_1 = 5;
    constexpr int WORLD_2_2 = 6;
    constexpr int WORLD_2_3 = 7;
    constexpr int WORLD_2_4 = 8;
    constexpr int WORLD_3_1 = 9;
    constexpr int WORLD_3_2 = 10;
    constexpr int WORLD_3_3 = 11;
    constexpr int WORLD_3_4 = 12;
    constexpr int END = 13;
}

namespace GAME
{
    constexpr int SECTION_SETTINGS = 1;
    constexpr int SECTION_SCENES = 2;
    constexpr int SECTION_TEXTURES = 3;
}

namespace ASSET
{
    constexpr int SECTION_UNKNOWN = -1;
    constexpr int SECTION_SPRITES = 1;
    constexpr int SECTION_ANIMATIONS = 3;
}

namespace OBJECT
{
    constexpr int MARIO = 0;
    constexpr int PLATFORM = 1;
    constexpr int BULLET = 2;
    constexpr int BACKGROUND = 3;
    constexpr int PIPE = 4;
    constexpr int SWITCH_SCENE_POINT = 5;
	constexpr int BRICK_TEST = 6;
	constexpr int DYNAMIC_PLATFORM = 7;
	constexpr int BRICK = 8;
	constexpr int DEATH_ZONE = 9;
	constexpr int QUESTION_BLOCK = 10;
}

namespace TEXTURE
{
    constexpr int BRICK = 0;
    constexpr int MARIO = 1;
    constexpr int MISC = 2;
    constexpr int MENUBACKGROUND = 3;
    constexpr int LEVEL1_1 = 4;
    constexpr int BBOX = -100;
}

namespace ANIMATION
{
    constexpr int BRICK = 100;
    constexpr int BRICK_TEST = 101;
    constexpr int DYNAMIC_PLATFORM = 102;
    constexpr int BRICK_FRAGMENT_UP_LEFT = 110;
    constexpr int BRICK_FRAGMENT_UP_RIGHT = 111;
    constexpr int BRICK_FRAGMENT_BOTTOM_LEFT = 112;
    constexpr int BRICK_FRAGMENT_BOTTOM_RIGHT = 113;
    constexpr int BULLET = 200;
    constexpr int CRACKED_BRICK = 300;
    
    // Background
    constexpr int MENU_BACKGROUND = 300;
    constexpr int BACKGROUND_LEVEL1_1 = 3000;
	constexpr int BACKGROUND_LEVEL1_2 = 3001;
	constexpr int BACKGROUND_LEVEL1_3 = 3002;
	constexpr int BACKGROUND_LEVEL1_4 = 3003;

    // BIG MARIO
    constexpr int MARIO_BIG_IDLE_RIGHT = 400;
    constexpr int MARIO_BIG_IDLE_LEFT = 401;
    constexpr int MARIO_BIG_WALKING_RIGHT = 500;
    constexpr int MARIO_BIG_WALKING_LEFT = 501;
    constexpr int MARIO_BIG_RUNNING_RIGHT = 600;
    constexpr int MARIO_BIG_RUNNING_LEFT = 601;
    constexpr int MARIO_BIG_JUMP_WALK_RIGHT = 700;
    constexpr int MARIO_BIG_JUMP_WALK_LEFT = 701;
    constexpr int MARIO_BIG_JUMP_RUN_RIGHT = 800;
    constexpr int MARIO_BIG_JUMP_RUN_LEFT = 801;
    constexpr int MARIO_BIG_SIT_RIGHT = 900;
    constexpr int MARIO_BIG_SIT_LEFT = 901;
    constexpr int MARIO_BIG_BRACE_RIGHT = 1000;
    constexpr int MARIO_BIG_BRACE_LEFT = 1001;

    // SMALL MARIO
    constexpr int MARIO_SMALL_IDLE_RIGHT = 1100;
    constexpr int MARIO_SMALL_IDLE_LEFT = 1102;
    constexpr int MARIO_SMALL_WALKING_RIGHT = 1200;
    constexpr int MARIO_SMALL_WALKING_LEFT = 1201;
    constexpr int MARIO_SMALL_RUNNING_RIGHT = 1300;
    constexpr int MARIO_SMALL_RUNNING_LEFT = 1301;
    constexpr int MARIO_SMALL_BRACE_RIGHT = 1400;
    constexpr int MARIO_SMALL_BRACE_LEFT = 1401;
    constexpr int MARIO_SMALL_JUMP_WALK_RIGHT = 1500;
    constexpr int MARIO_SMALL_JUMP_WALK_LEFT = 1501;
    constexpr int MARIO_SMALL_JUMP_RUN_RIGHT = 1600;
    constexpr int MARIO_SMALL_JUMP_RUN_LEFT = 1601;

    constexpr int MARIO_DIE = 999;

    // PIPE
    constexpr int PIPE = 3100;

    // QUESTION BLOCK
    constexpr int QUESTION_BLOCK = 3200;
    constexpr int QUESTION_BLOCK_DELETED = 3201;
}
