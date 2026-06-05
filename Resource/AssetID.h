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
    constexpr int BRICK = 1;
    constexpr int BULLET = 2;
    constexpr int BACKGROUND = 3;
    constexpr int PIPE = 4;
    constexpr int SWITCH_SCENE_POINT = 5;
	constexpr int BRICK_TEST = 6;

    // Enemies
    constexpr int PIRANHA_PLANT = 7;
    constexpr int BLOOPER = 8;
    constexpr int BULLET_BILL = 9;
    constexpr int CANNON = 10;
    constexpr int GOOMBA = 11;
    constexpr int KOOPA = 12;
    constexpr int BUZZY = 13;
    constexpr int PODOBOO = 14;
    constexpr int SPINY = 15;
    constexpr int LAKITU = 16;
    constexpr int BOWSER = 17;
    constexpr int BOWSER_FIRE = 18;
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
    constexpr int BRICK = 100;
    constexpr int BRICK_TEST = 101;
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
    constexpr int PIPE_HEAD = 3100;
    constexpr int PIPE_BODY = 3101;

    // PIRANHA PLANT
    constexpr int PIRANHA_PLANT_IDLE = 3200;

    // BLOOPER
	constexpr int BLOOPER_IDLE = 3300;
	constexpr int BLOOPER_SWIM = 3301;
	constexpr int BLOOPER_RISING = 3302;
	constexpr int BLOOPER_SINKING = 3303;

    // BULLET BILL
    constexpr int BULLET_BILL = 3400;

    // HAMMER BRO
    constexpr int HAMMER_BRO_IDLE = 3500;
    constexpr int HAMMER_BRO_WALK = 3501;
    constexpr int HAMMER = 3550;

    // BOWSER
    constexpr int BOWSER_IDLE = 3600;
    constexpr int BOWSER_WALK = 3601;
    constexpr int BOWSER_FIRE = 3602;
    constexpr int BOWSER_FIREBALL = 3650;

    // CANNON
    constexpr int CANNON = 3700;
    //GOOMBA
    constexpr int GOOMBA_WALK = 4000;
    constexpr int GOOMBA_DIE = 4001;

	// KOOPA TROOPA
    constexpr int KOOPA_WALK_LEFT = 5000;
    constexpr int KOOPA_WALK_RIGHT = 5001;
    constexpr int KOOPA_SHELL = 5002;
    constexpr int KOOPA_SPINNING = 5003;

	// BUZZY BEETLE
    constexpr int BUZZY_WALK_LEFT = 6000;
    constexpr int BUZZY_WALK_RIGHT = 6001;
    constexpr int BUZZY_SHELL = 6002;
    constexpr int BUZZY_SPINNING = 6003;

	// PODOBOO
    constexpr int PODOBOO_UP = 7000;
    constexpr int PODOBOO_DOWN = 7001;

	// SPINY
    constexpr int SPINY_WALK_LEFT = 8000;
    constexpr int SPINY_WALK_RIGHT = 8001;
    constexpr int SPINY_EGG_SPIN = 8002;

	// LAKITU
    constexpr int LAKITU_FLY_LEFT = 9000;
    constexpr int LAKITU_FLY_RIGHT = 9001;

	// BOWSER
    constexpr int BOWSER_WALK_LEFT = 10000;
    constexpr int BOWSER_WALK_RIGHT = 10001;
    constexpr int BOWSER_ATTACK_LEFT = 10002;
    constexpr int BOWSER_ATTACK_RIGHT = 10003;

    constexpr int BOWSER_FIRE_LEFT = 10004;
    constexpr int BOWSER_FIRE_RIGHT = 10005;
}
