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
    constexpr int CONTROL = 14;
    constexpr int DEATH = 15;
    constexpr int GAME_OVER = 16;
    constexpr int INTRO = 17;
    constexpr int HELP = 18;
    constexpr int LEVEL_SELECT = 19;
}

#define ID_SCENE_HELP   SCENE::HELP
#define ID_SCENE_LEVEL  SCENE::LEVEL_SELECT

namespace GAME_STATE
{
    constexpr int MENU = 0;
    constexpr int PLAY = 1;
    constexpr int PAUSE = 2;
    constexpr int GAME_OVER = 3;
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
	constexpr int CASTLE_BRIDGE = 16;
	constexpr int AXE = 17;
    constexpr int COIN = 18;
	constexpr int MUSHROOM_1UP = 19;
	constexpr int HAMMER = 23;
	constexpr int ITEM_COIN2 = 24;

	constexpr int GOOMBA = 30;
	constexpr int KOOPA = 31;
	constexpr int BUZZY_BEETLE = 32;
	constexpr int HAMMER_BRO = 33;
	constexpr int BLOOPER = 34;
	constexpr int BOWSER = 35;
	constexpr int BOWSER_FIRE = 36;
	constexpr int BOWSER_FIREBALL = 37;
	constexpr int BULLET_BILL = 38;
	constexpr int CANNON = 39;
	constexpr int LAKITU = 40;
	constexpr int PODOBOO = 41;
	constexpr int SPINY = 42;
	constexpr int PIRANHA_PLANT = 43;
	constexpr int ENEMY_TURN_BLOCK = 44;

	constexpr int MENU_OPTIONS = 50;
	constexpr int LIVES_NUMBER = 51;

    constexpr int WIND_PARTICLE = 60;
}

namespace TEXTURE
{
    constexpr int BRICK = 0;
    constexpr int MARIO = 1;
    constexpr int MISC = 2;
    constexpr int MENUBACKGROUND = 3;
    constexpr int LEVEL1_1 = 4;
    constexpr int BBOX = -100;
    constexpr int ENEMIES = 11;

    constexpr int MENU_SCENE = 14;
    constexpr int CONTROL_SCENE = 15;
    constexpr int END_SCENE = 16;
    constexpr int MENU_TEXT = 17;
    constexpr int DEATH_SCENE = 18;
    constexpr int GAMEOVER_SCENE = 19;
    constexpr int NUMS = 20;
    constexpr int INTRO_SCENE = 21;
    constexpr int HELP_SCENE = 22;
    constexpr int LEVEL_SCENE = 23;
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
    constexpr int BULLET_RIGHT = 200;
    constexpr int BULLET_LEFT = 201;
    constexpr int CRACKED_BRICK = 300;

    constexpr int WIND_EFFECT = 310;

    constexpr int MENU_BACKGROUND = 300;
    constexpr int BACKGROUND_LEVEL1_1 = 3000;
	constexpr int BACKGROUND_LEVEL1_2 = 3001;
	constexpr int BACKGROUND_LEVEL1_3 = 3002;
	constexpr int BACKGROUND_LEVEL1_4 = 3003;
	constexpr int BACKGROUND_MENU     = 3004;
	constexpr int BACKGROUND_CONTROL  = 3005;
	constexpr int BACKGROUND_END      = 3006;
	constexpr int BACKGROUND_DEATH    = 3007;
	constexpr int BACKGROUND_GAMEOVER = 3008;
	constexpr int BACKGROUND_INTRO    = 3009;

	constexpr int MENU_START_WHITE    = 3010;
	constexpr int MENU_START_YELLOW   = 3011;
	constexpr int MENU_GUIDE_WHITE    = 3012;
	constexpr int MENU_GUIDE_YELLOW   = 3013;

	constexpr int LIVES_DIGIT_1       = 3020;
	constexpr int LIVES_DIGIT_2       = 3021;
	constexpr int LIVES_DIGIT_3       = 3022;

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
    constexpr int MARIO_BIG_SKID_RIGHT = 1050;
    constexpr int MARIO_BIG_SKID_LEFT = 1051;

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

    constexpr int MARIO_SMALL_IDLE_RIGHT = 1100;
    constexpr int MARIO_SMALL_IDLE_LEFT = 1102;
    constexpr int MARIO_SMALL_WALKING_RIGHT = 1200;
    constexpr int MARIO_SMALL_WALKING_LEFT = 1201;
    constexpr int MARIO_SMALL_RUNNING_RIGHT = 1300;
    constexpr int MARIO_SMALL_RUNNING_LEFT = 1301;
    constexpr int MARIO_SMALL_BRACE_RIGHT = 1400;
    constexpr int MARIO_SMALL_BRACE_LEFT = 1401;
    constexpr int MARIO_SMALL_SKID_RIGHT = 1700;
    constexpr int MARIO_SMALL_SKID_LEFT = 1701;
    constexpr int MARIO_SMALL_JUMP_WALK_RIGHT = 1500;
    constexpr int MARIO_SMALL_JUMP_WALK_LEFT = 1501;
    constexpr int MARIO_SMALL_JUMP_RUN_RIGHT = 1600;
    constexpr int MARIO_SMALL_JUMP_RUN_LEFT = 1601;

    constexpr int MARIO_DIE = 999;

    constexpr int PIPE_OVERWORLD = 3100;
    constexpr int PIPE_UNDERWORLD = 3101;
    constexpr int PIPE_CASTLE = 3102;

    constexpr int QUESTION_BLOCK = 3200;
    constexpr int QUESTION_BLOCK_DELETED = 3201;

    constexpr int MUSHROOM = 5050;
    constexpr int FIRE_FLOWER = 5051;
    constexpr int POISON_MUSHROOM = 5052;
    constexpr int SUPER_STAR = 5053;
    constexpr int FROG_SUIT = 5054;
    constexpr int CASTLE_BRIDGE = 5007;
    constexpr int AXE = 5008;

    constexpr int MARIO_FIRE_IDLE_RIGHT = 2100;
    constexpr int MARIO_FIRE_IDLE_LEFT = 2101;
    constexpr int MARIO_FIRE_WALKING_RIGHT = 2200;
    constexpr int MARIO_FIRE_WALKING_LEFT = 2201;
    constexpr int MARIO_FIRE_RUNNING_RIGHT = 2250;
    constexpr int MARIO_FIRE_RUNNING_LEFT = 2251;
    constexpr int MARIO_FIRE_JUMP_WALK_RIGHT = 2300;
    constexpr int MARIO_FIRE_JUMP_WALK_LEFT = 2301;
    constexpr int MARIO_FIRE_SKID_RIGHT = 2400;
    constexpr int MARIO_FIRE_SKID_LEFT = 2401;

    constexpr int MARIO_FROG_IDLE_RIGHT = 5400;
    constexpr int MARIO_FROG_IDLE_LEFT = 5401;
    constexpr int MARIO_FROG_WALKING_RIGHT = 5500;
    constexpr int MARIO_FROG_WALKING_LEFT = 5501;
    constexpr int MARIO_FROG_JUMP_RIGHT = 5600;
    constexpr int MARIO_FROG_JUMP_LEFT = 5601;


    constexpr int ITEM_COIN          = 8000;
    constexpr int ITEM_MUSHROOM_1UP  = 8001;
    constexpr int ITEM_SUPER_LEAF    = 8002;
    constexpr int ITEM_TANOOKI_SUIT  = 8003;
    constexpr int ITEM_HAMMER_SUIT   = 8004;

    constexpr int GOOMBA_WALK        = 4000;
    constexpr int GOOMBA_DIE         = 4001;

    constexpr int KOOPA_WALK_LEFT    = 5000;
    constexpr int KOOPA_WALK_RIGHT   = 5001;
    constexpr int KOOPA_SHELL        = 5002;
    constexpr int KOOPA_SPINNING     = 5003;

    constexpr int BUZZY_WALK_LEFT    = 6000;
    constexpr int BUZZY_WALK_RIGHT   = 6001;
    constexpr int BUZZY_SHELL        = 6002;
    constexpr int BUZZY_SPINNING     = 6003;

    constexpr int PODOBOO_UP         = 7000;
    constexpr int PODOBOO_DOWN       = 7001;

    constexpr int SPINY_WALK_LEFT    = 8500;
    constexpr int SPINY_WALK_RIGHT   = 8501;
    constexpr int SPINY_EGG_SPIN     = 8502;

    constexpr int LAKITU_FLY_LEFT    = 9000;
    constexpr int LAKITU_FLY_RIGHT   = 9001;

    constexpr int PIRANHA_PLANT_IDLE = 3250;

    constexpr int BLOOPER_IDLE       = 3300;
    constexpr int BLOOPER_RISING     = 3301;
    constexpr int BLOOPER_SINKING    = 3302;

    constexpr int BULLET_BILL        = 3400;

    constexpr int HAMMER_BRO_WALK_LEFT  = 3500;
    constexpr int HAMMER_BRO_WALK_RIGHT = 3501;
    constexpr int HAMMER_BRO_JUMP       = 3502;
    constexpr int HAMMER             = 3550;

    constexpr int BOWSER_WALK_RIGHT  = 3600;
    constexpr int BOWSER_WALK_LEFT   = 3601;
    constexpr int BOWSER_ATTACK_RIGHT = 3602;
    constexpr int BOWSER_ATTACK_LEFT  = 3602;
    constexpr int BOWSER_FIRE_RIGHT  = 10005;
    constexpr int BOWSER_FIRE_LEFT   = 10004;

    constexpr int BOWSER_FIRE        = 3650;

    constexpr int CANNON             = 3700;
}

namespace SFX
{
    constexpr int JUMP               = 1000;
    constexpr int JUMP_SMALL         = 1001;
    constexpr int STOMP              = 1002;
    constexpr int COIN              = 1003;
    constexpr int POWERUP           = 1004;
    constexpr int POWERUP_APPEARS    = 1005;
    constexpr int FIREBALL           = 1006;
    constexpr int SHRINK             = 1007;
    constexpr int DIE                = 1008;
    constexpr int ONE_UP             = 1009;

    constexpr int GOOMBA_STOMP       = 2000;
    constexpr int KOOPA_STOMP        = 2001;
    constexpr int KOOPA_KICK         = 2002;
    constexpr int BOWSER_ROAR        = 2003;
    constexpr int BOWSER_FIRE        = 2004;
    constexpr int BOWSER_FALLS       = 2005;

    constexpr int BRICK_BREAK        = 3000;
    constexpr int BUMP               = 3001;
    constexpr int QUESTION_BLOCK      = 3002;
    constexpr int PIPE_WARP           = 3003;
    constexpr int FLAGPOLE            = 3004;
    constexpr int CASTLE_CLEAR       = 3005;
    constexpr int LEVEL_CLEAR        = 3006;
    constexpr int SMB_WINDY          = 3007;
    constexpr int SMB_SHAKE          = 3009;
    constexpr int FIREWORKS           = 3008;

    constexpr int MENU_SELECT        = 4000;
    constexpr int MENU_MOVE          = 4001;
    constexpr int PAUSE              = 4002;
    constexpr int GAME_OVER          = 4003;
}

namespace BGM
{
    constexpr int OVERWORLD_THEME    = 5000;
    constexpr int UNDERWORLD_THEME  = 5001;
    constexpr int WATER_THEME        = 5002;
    constexpr int CASTLE_THEME       = 5003;
    constexpr int STAR_THEME         = 5004;
    constexpr int MENU_THEME         = 5005;
    constexpr int GAME_OVER_THEME    = 5006;
    constexpr int WARNING_THEME     = 5007;
    constexpr int STAGE_CLEAR_THEME  = 5008;
}
