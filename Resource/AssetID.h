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
	constexpr int MUSHROOM = 11;
	constexpr int FIRE_FLOWER = 12;
	constexpr int POISON_MUSHROOM = 13;
	constexpr int SUPER_STAR = 14;
	constexpr int FROG_SUIT = 15;
	constexpr int CASTLE_BRIDGE = 16;
	constexpr int AXE = 17;
}

namespace TEXTURE
{
    constexpr int BRICK = 0;
    constexpr int MARIO = 1;
    constexpr int MISC = 2;
    constexpr int MENUBACKGROUND = 3;
    constexpr int LEVEL1_1 = 4;
    constexpr int BBOX = -100;
    constexpr int ENEMIES = 8;
}

namespace ANIMATION
{
    constexpr int BRICK_OVERWORLD = 100;
    constexpr int BRICK_UNDERWORLD = 101;
    constexpr int DYNAMIC_PLATFORM = 102;
    constexpr int BRICK_FRAGMENT_OVERWORLD_UP_LEFT = 110;
    constexpr int BRICK_FRAGMENT_OVERWORLD_UP_RIGHT = 111;
    constexpr int BRICK_FRAGMENT_OVERWORLD_BOTTOM_LEFT = 112;
    constexpr int BRICK_FRAGMENT_OVERWORLD_BOTTOM_RIGHT = 113;
    constexpr int BRICK_FRAGMENT_UNDERWORLD_UP_LEFT = 120;
    constexpr int BRICK_FRAGMENT_UNDERWORLD_UP_RIGHT = 121;
    constexpr int BRICK_FRAGMENT_UNDERWORLD_BOTTOM_LEFT = 122;
    constexpr int BRICK_FRAGMENT_UNDERWORLD_BOTTOM_RIGHT = 123;
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

    // RACCOON MARIO
    constexpr int MARIO_RACCOON_IDLE_RIGHT = 2400;
    constexpr int MARIO_RACCOON_IDLE_LEFT = 2401;
    constexpr int MARIO_RACCOON_WALKING_RIGHT = 2500;
    constexpr int MARIO_RACCOON_WALKING_LEFT = 2501;
    constexpr int MARIO_RACCOON_RUNNING_RIGHT = 2600;
    constexpr int MARIO_RACCOON_RUNNING_LEFT = 2601;
    constexpr int MARIO_RACCOON_JUMP_WALK_RIGHT = 2700;
    constexpr int MARIO_RACCOON_JUMP_WALK_LEFT = 2701;
    constexpr int MARIO_RACCOON_JUMP_RUN_RIGHT = 2800;
    constexpr int MARIO_RACCOON_JUMP_RUN_LEFT = 2801;
    constexpr int MARIO_RACCOON_SIT_RIGHT = 2900;
    constexpr int MARIO_RACCOON_SIT_LEFT = 2901;
    constexpr int MARIO_RACCOON_BRACE_RIGHT = 3000;
    constexpr int MARIO_RACCOON_BRACE_LEFT = 3001;

    // TANOOKI MARIO
    constexpr int MARIO_TANOOKI_IDLE_RIGHT = 3400;
    constexpr int MARIO_TANOOKI_IDLE_LEFT = 3401;
    constexpr int MARIO_TANOOKI_WALKING_RIGHT = 3500;
    constexpr int MARIO_TANOOKI_WALKING_LEFT = 3501;
    constexpr int MARIO_TANOOKI_RUNNING_RIGHT = 3600;
    constexpr int MARIO_TANOOKI_RUNNING_LEFT = 3601;
    constexpr int MARIO_TANOOKI_JUMP_WALK_RIGHT = 3700;
    constexpr int MARIO_TANOOKI_JUMP_WALK_LEFT = 3701;
    constexpr int MARIO_TANOOKI_JUMP_RUN_RIGHT = 3800;
    constexpr int MARIO_TANOOKI_JUMP_RUN_LEFT = 3801;
    constexpr int MARIO_TANOOKI_SIT_RIGHT = 3900;
    constexpr int MARIO_TANOOKI_SIT_LEFT = 3901;
    constexpr int MARIO_TANOOKI_BRACE_RIGHT = 4000;
    constexpr int MARIO_TANOOKI_BRACE_LEFT = 4001;

    // HAMMER MARIO
    constexpr int MARIO_HAMMER_IDLE_RIGHT = 4400;
    constexpr int MARIO_HAMMER_IDLE_LEFT = 4401;
    constexpr int MARIO_HAMMER_WALKING_RIGHT = 4500;
    constexpr int MARIO_HAMMER_WALKING_LEFT = 4501;
    constexpr int MARIO_HAMMER_RUNNING_RIGHT = 4600;
    constexpr int MARIO_HAMMER_RUNNING_LEFT = 4601;
    constexpr int MARIO_HAMMER_JUMP_WALK_RIGHT = 4700;
    constexpr int MARIO_HAMMER_JUMP_WALK_LEFT = 4701;
    constexpr int MARIO_HAMMER_JUMP_RUN_RIGHT = 4800;
    constexpr int MARIO_HAMMER_JUMP_RUN_LEFT = 4801;
    constexpr int MARIO_HAMMER_SIT_RIGHT = 4900;
    constexpr int MARIO_HAMMER_SIT_LEFT = 4901;
    constexpr int MARIO_HAMMER_BRACE_RIGHT = 5000;
    constexpr int MARIO_HAMMER_BRACE_LEFT = 5001;

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
    constexpr int PIPE_OVERWORLD = 3100;
    constexpr int PIPE_UNDERWORLD = 3101;
    constexpr int PIPE_CASTLE = 3102;

    // QUESTION BLOCK
    constexpr int QUESTION_BLOCK = 3200;
    constexpr int QUESTION_BLOCK_DELETED = 3201;

    // ITEMS (5002-5008 để tránh clash với TANOOKI 4000-4001 và HAMMER 4400-5001)
    constexpr int MUSHROOM = 5002;
    constexpr int FIRE_FLOWER = 5003;
    constexpr int POISON_MUSHROOM = 5004;
    constexpr int SUPER_STAR = 5005;
    constexpr int FROG_SUIT = 5006;
    constexpr int CASTLE_BRIDGE = 5007;
    constexpr int AXE = 5008;

    // FIRE MARIO (2100-2301, safe)
    constexpr int MARIO_FIRE_IDLE_RIGHT = 2100;
    constexpr int MARIO_FIRE_IDLE_LEFT = 2101;
    constexpr int MARIO_FIRE_WALKING_RIGHT = 2200;
    constexpr int MARIO_FIRE_WALKING_LEFT = 2201;
    constexpr int MARIO_FIRE_JUMP_WALK_RIGHT = 2300;
    constexpr int MARIO_FIRE_JUMP_WALK_LEFT = 2301;

    // FROG MARIO (5400-5601, safe — tránh RACCOON 2400-3001)
    constexpr int MARIO_FROG_IDLE_RIGHT = 5400;
    constexpr int MARIO_FROG_IDLE_LEFT = 5401;
    constexpr int MARIO_FROG_WALKING_RIGHT = 5500;
    constexpr int MARIO_FROG_WALKING_LEFT = 5501;
    constexpr int MARIO_FROG_JUMP_RIGHT = 5600;
    constexpr int MARIO_FROG_JUMP_LEFT = 5601;
}
