#pragma once

// File này định nghĩa tất cả các ID dùng trong game
// Các ID được nhóm theo namespace để dễ quản lý:
// - SCENE: ID của các scene (menu, level, death, v.v.)
// - GAME_STATE: Trạng thái game tổng quát (menu, play, pause, game over)
// - GAME: Section trong file cấu hình game
// - ASSET: Section trong file cấu hình asset
// - OBJECT: ID của các object type (Mario, enemy, item, v.v.)
// - TEXTURE: ID của các texture file
// - ANIMATION: ID của các animation
// - SFX: ID của các sound effect
// - BGM: ID của các background music
//
// LƯU Ý QUAN TRỌNG:
// - Các ID animation từ 8000 trở lên phải khớp với file .txt trong Application/Objects/
// - Đừng đổi số ID nếu không sửa luôn file .txt tương ứng

// ID của các scene trong game
// Dùng để chuyển đổi giữa các màn chơi/menu
namespace SCENE
{
    // Section ID dùng trong file cấu hình scene
    constexpr int SECTION_UNKNOWN = -1;
    constexpr int SECTION_ASSETS = 1;
    constexpr int SECTION_OBJECTS = 2;
    constexpr int SECTION_MAP = 3;

    // ID của từng scene cụ thể
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
    constexpr int END = 13;      // end / win scene
    constexpr int CONTROL = 14;  // controls / how-to-play scene
    constexpr int DEATH = 15;    // "Mario died / lost a life" screen (lives remain)
    constexpr int GAME_OVER = 16; // out-of-lives screen (Enter -> replay from the start)
    constexpr int INTRO = 17;     // intro screen (WORLD X-X, Mario x lives) before starting a level
    constexpr int HELP = 18;      // help / controls and volume UI
    constexpr int LEVEL_SELECT = 19; // level selection UI
}

// Tên tương thích để chuyển scene UI (để tương thích với code cũ)
// IDs 4 và 5 đã bị chiếm bởi gameplay scenes, nên dùng IDs trống khác
#define ID_SCENE_HELP   SCENE::HELP
#define ID_SCENE_LEVEL  SCENE::LEVEL_SELECT

// Trạng thái game tổng quát (không phụ thuộc scene nào đang load)
// Dùng để quản lý logic game ở mức cao hơn scene
namespace GAME_STATE
{
    constexpr int MENU = 0;       // menu / control / death-continue screens
    constexpr int PLAY = 1;       // actively playing a level
    constexpr int PAUSE = 2;      // gameplay frozen
    constexpr int GAME_OVER = 3;  // game finished (reached the end scene)
}

// Section ID trong file cấu hình game (super-mario-bros.txt)
namespace GAME
{
    constexpr int SECTION_SETTINGS = 1;
    constexpr int SECTION_SCENES = 2;
    constexpr int SECTION_TEXTURES = 3;
}

// Section ID trong file cấu hình asset
namespace ASSET
{
    constexpr int SECTION_UNKNOWN = -1;
    constexpr int SECTION_SPRITES = 1;
    constexpr int SECTION_ANIMATIONS = 3;
}

// ID của các loại object trong game
// Dùng để xác định loại object khi spawn từ map
namespace OBJECT
{
    // ---- PLAYER & ENVIRONMENT ----
    constexpr int MARIO = 0;              // Người chơi
    constexpr int PLATFORM = 1;           // Nền tảng tĩnh
    constexpr int BULLET = 2;             // Đạn Mario bắn
    constexpr int BACKGROUND = 3;         // Background
    constexpr int PIPE = 4;               // Ống dẫn (warp pipe)
    constexpr int SWITCH_SCENE_POINT = 5; // Điểm chuyển scene
	constexpr int BRICK_TEST = 6;         // Gạch test
	constexpr int DYNAMIC_PLATFORM = 7;   // Nền tảng động (di chuyển)
	constexpr int BRICK = 8;               // Gạch thường
	constexpr int DEATH_ZONE = 9;          // Vùng chết (rơi xuống chết)
	constexpr int QUESTION_BLOCK = 10;    // Block dấu chấm hỏi

    // ---- ITEMS (Vật phẩm) ----
	constexpr int MUSHROOM = 11;          // Nấm tăng size
	constexpr int FIRE_FLOWER = 12;       // Hoa lửa
	constexpr int POISON_MUSHROOM = 13;    // Nấm độc
	constexpr int SUPER_STAR = 14;         // Ngôi sao vô hình
	constexpr int CASTLE_BRIDGE = 16;      // Cầu ở castle
	constexpr int AXE = 17;                // Rìu (cắt cầu)
    constexpr int COIN = 18;              // Xu
	constexpr int MUSHROOM_1UP = 19;      // Nấm 1UP
	constexpr int HAMMER = 23;             // Búa
	constexpr int ITEM_COIN2 = 24;        // Xu (variant khác)

	// ---- ENEMIES (Kẻ thù) ----
	constexpr int GOOMBA = 30;             // Nấm nâu
	constexpr int KOOPA = 31;              // Rùa xanh
	constexpr int BUZZY_BEETLE = 32;       // Bọ cánh cứng
	constexpr int HAMMER_BRO = 33;         // Kẻ ném búa
	constexpr int BLOOPER = 34;            // Mực nước
	constexpr int BOWSER = 35;             // Boss cuối game
	constexpr int BOWSER_FIRE = 36;        // Lửa Bowser
	constexpr int BOWSER_FIREBALL = 37;   // Quả cầu lửa Bowser
	constexpr int BULLET_BILL = 38;        // Đạn côn
	constexpr int CANNON = 39;             // Súng đạn côn
	constexpr int LAKITU = 40;             // Rùa bay ném Spiny
	constexpr int PODOBOO = 41;            // Quỷ lửa nhảy
	constexpr int SPINY = 42;              // Rùa gai
	constexpr int PIRANHA_PLANT = 43;     // Cây ăn thịt trong ống
	constexpr int ENEMY_TURN_BLOCK = 44;   // Block vô hình làm kẻ thù quay đầu

	// ---- UI (Giao diện) ----
	constexpr int MENU_OPTIONS = 50;      // Tùy chọn menu ("Bắt đầu" / "Hướng dẫn")
	constexpr int LIVES_NUMBER = 51;      // Số mạng còn lại (death screen)

    // ---- EFFECTS ----
    constexpr int WIND_PARTICLE = 60;     // Hiệu ứng gió
}

// ID của các texture file (ảnh)
// Dùng bởi TextureManager để load texture từ file
namespace TEXTURE
{
    // Texture cơ bản
    constexpr int BRICK = 0;
    constexpr int MARIO = 1;
    constexpr int MISC = 2;
    constexpr int MENUBACKGROUND = 3;
    constexpr int LEVEL1_1 = 4;
    constexpr int BBOX = -100;          // Bounding box debug
    constexpr int ENEMIES = 11;

    // Background toàn màn hình cho các scene (khớp với [TEXTURES] trong super-mario-bros.txt)
    constexpr int MENU_SCENE = 14;
    constexpr int CONTROL_SCENE = 15;
    constexpr int END_SCENE = 16;
    constexpr int MENU_TEXT = 17;       // menu_text.png: text "Bắt đầu" & "Hướng dẫn"
    constexpr int DEATH_SCENE = 18;     // death_scene.png
    constexpr int GAMEOVER_SCENE = 19;  // gameover_scene.png
    constexpr int NUMS = 20;             // nums_123.png (chữ số 1,2,3)
    constexpr int INTRO_SCENE = 21;     // intro_scene.png
    constexpr int HELP_SCENE = 22;      // help_scene.png
    constexpr int LEVEL_SCENE = 23;     // level_scene.png
}

// ID của các animation
// Dùng bởi AnimationManager để load animation từ file .txt
// Animation định nghĩa chuỗi sprite frames cho một hành động
namespace ANIMATION
{
    // ---- BRICK & PLATFORM ----
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
    
    // ---- EFFECTS ----
    constexpr int WIND_EFFECT = 310;    // Hiệu ứng gió
    
    // ---- BACKGROUND ANIMATIONS ----
    constexpr int MENU_BACKGROUND = 300;
    constexpr int BACKGROUND_LEVEL1_1 = 3000;
	constexpr int BACKGROUND_LEVEL1_2 = 3001;
	constexpr int BACKGROUND_LEVEL1_3 = 3002;
	constexpr int BACKGROUND_LEVEL1_4 = 3003;
	constexpr int BACKGROUND_MENU     = 3004; // menu_scene.png
	constexpr int BACKGROUND_CONTROL  = 3005; // controls.png
	constexpr int BACKGROUND_END      = 3006; // end_scene.png
	constexpr int BACKGROUND_DEATH    = 3007; // death_scene.png
	constexpr int BACKGROUND_GAMEOVER = 3008; // gameover_scene.png
	constexpr int BACKGROUND_INTRO    = 3009; // intro_scene.png

	// ---- MENU TEXT (trắng = chưa chọn, vàng = đã chọn) ----
	constexpr int MENU_START_WHITE    = 3010; // "Bắt đầu" (trắng)
	constexpr int MENU_START_YELLOW   = 3011; // "Bắt đầu" (vàng)
	constexpr int MENU_GUIDE_WHITE    = 3012; // "Hướng dẫn" (trắng)
	constexpr int MENU_GUIDE_YELLOW   = 3013; // "Hướng dẫn" (vàng)

	// ---- LIVES DIGITS (death screen) ----
	// Phải liên tiếp: digit n -> LIVES_DIGIT_1 + (n-1)
	constexpr int LIVES_DIGIT_1       = 3020;
	constexpr int LIVES_DIGIT_2       = 3021;
	constexpr int LIVES_DIGIT_3       = 3022;

    // ---- MARIO ANIMATIONS ----
    // MARIO BIG (Mario lớn)
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

    // MARIO RACCOON (Mario Raccoon)
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

    // MARIO TANOOKI (Mario Tanooki)
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

    // MARIO HAMMER (Mario Hammer)
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

    // MARIO SMALL (Mario nhỏ)
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

    // ---- PIPE ANIMATIONS ----
    constexpr int PIPE_OVERWORLD = 3100;
    constexpr int PIPE_UNDERWORLD = 3101;
    constexpr int PIPE_CASTLE = 3102;

    // ---- QUESTION BLOCK ----
    constexpr int QUESTION_BLOCK = 3200;
    constexpr int QUESTION_BLOCK_DELETED = 3201;

    // ---- ITEM ANIMATIONS ----
    // Items cơ bản - dời sang 5050-5054 để tránh trùng KOOPA (5000-5003)
    // CASTLE_BRIDGE/AXE giữ 5007/5008
    // (khớp với mushroom.txt / fire_flower.txt / poison_mushroom.txt / super_star.txt / frog_suit.txt)
    constexpr int MUSHROOM = 5050;
    constexpr int FIRE_FLOWER = 5051;
    constexpr int POISON_MUSHROOM = 5052;
    constexpr int SUPER_STAR = 5053;
    constexpr int FROG_SUIT = 5054;
    constexpr int CASTLE_BRIDGE = 5007;
    constexpr int AXE = 5008;

    // MARIO FIRE (Mario lửa) - range 2100-2301
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

    // MARIO FROG (Mario ếch) - range 5400-5601 (tránh RACCOON 2400-3001)
    constexpr int MARIO_FROG_IDLE_RIGHT = 5400;
    constexpr int MARIO_FROG_IDLE_LEFT = 5401;
    constexpr int MARIO_FROG_WALKING_RIGHT = 5500;
    constexpr int MARIO_FROG_WALKING_LEFT = 5501;
    constexpr int MARIO_FROG_JUMP_RIGHT = 5600;
    constexpr int MARIO_FROG_JUMP_LEFT = 5601;

    // ============================================================
    //  CÁC ID DƯỚI ĐÂY PHẢI KHỚP VỚI FILE .txt TRONG Application/Objects/
    //  (đừng đổi số nếu không sửa luôn file .txt tương ứng)
    // ============================================================

    // ---- ITEM ANIMATIONS (item2.txt) ----
    constexpr int ITEM_COIN          = 8000;
    constexpr int ITEM_MUSHROOM_1UP  = 8001;
    constexpr int ITEM_SUPER_LEAF    = 8002;
    constexpr int ITEM_TANOOKI_SUIT  = 8003;
    constexpr int ITEM_HAMMER_SUIT   = 8004;

    // ---- ENEMY ANIMATIONS ----
    // Goomba (Goomba.txt: 4000-4001)
    constexpr int GOOMBA_WALK        = 4000;
    constexpr int GOOMBA_DIE         = 4001;

    // Koopa (Koopa.txt: 5000-5003)
    constexpr int KOOPA_WALK_LEFT    = 5000;
    constexpr int KOOPA_WALK_RIGHT   = 5001;
    constexpr int KOOPA_SHELL        = 5002;
    constexpr int KOOPA_SPINNING     = 5003;

    // BuzzyBeetle (BuzzyBeetle.txt: 6000-6003)
    constexpr int BUZZY_WALK_LEFT    = 6000;
    constexpr int BUZZY_WALK_RIGHT   = 6001;
    constexpr int BUZZY_SHELL        = 6002;
    constexpr int BUZZY_SPINNING     = 6003;

    // Podoboo (Podoboo.txt: 7000-7001)
    constexpr int PODOBOO_UP         = 7000;
    constexpr int PODOBOO_DOWN       = 7001;

    // Spiny (Spiny.txt: 8500-8502)
    constexpr int SPINY_WALK_LEFT    = 8500;
    constexpr int SPINY_WALK_RIGHT   = 8501;
    constexpr int SPINY_EGG_SPIN     = 8502;

    // Lakitu (Lakitu.txt: 9000-9001)
    constexpr int LAKITU_FLY_LEFT    = 9000;
    constexpr int LAKITU_FLY_RIGHT   = 9001;

    // PiranhaPlant (piranha_plant.txt: 3250)
    constexpr int PIRANHA_PLANT_IDLE = 3250;

    // Blooper (blooper.txt: 3300-3303)
    constexpr int BLOOPER_IDLE       = 3300;
    constexpr int BLOOPER_RISING     = 3301;
    constexpr int BLOOPER_SINKING    = 3302;

    // BulletBill (bulletbill.txt: 3400)
    constexpr int BULLET_BILL        = 3400;

    // HammerBro (hammerbro.txt: 3500-3502) + Hammer projectile (hammer.txt: 3550)
    constexpr int HAMMER_BRO_WALK_LEFT  = 3500;   // đi/nhìn sang trái (hàng trên)
    constexpr int HAMMER_BRO_WALK_RIGHT = 3501;   // đi/nhìn sang phải (hàng dưới)
    constexpr int HAMMER_BRO_JUMP       = 3502;   // đang nhảy
    constexpr int HAMMER             = 3550;

    // Bowser (bowser.txt: 3600-3602)
    constexpr int BOWSER_WALK_RIGHT  = 3600;
    constexpr int BOWSER_WALK_LEFT   = 3601;
    constexpr int BOWSER_ATTACK_RIGHT = 3602;
    constexpr int BOWSER_ATTACK_LEFT  = 3602;
    // Ngọn lửa Bowser phun ra: trỏ về animation trong BowserFire.txt (không phải thân Bowser)
    constexpr int BOWSER_FIRE_RIGHT  = 10005;   // BowserFire.txt: FIRE RIGHT
    constexpr int BOWSER_FIRE_LEFT   = 10004;   // BowserFire.txt: FIRE LEFT

    // BowserFire projectile (BowserFire.txt: 3650)
    constexpr int BOWSER_FIRE        = 3650;

    // Cannon (cannon.txt: 3700)
    constexpr int CANNON             = 3700;
}

// ---- SOUND EFFECTS (SFX) ----
// ID của các hiệu ứng âm thanh (short sound effects)
// Dùng bởi SoundManager::LoadSFX() và PlaySFX()
namespace SFX
{
    // ---- MARIO SOUNDS ----
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

    // ---- ENEMY SOUNDS ----
    constexpr int GOOMBA_STOMP       = 2000;
    constexpr int KOOPA_STOMP        = 2001;
    constexpr int KOOPA_KICK         = 2002;
    constexpr int BOWSER_ROAR        = 2003;
    constexpr int BOWSER_FIRE        = 2004;
    constexpr int BOWSER_FALLS       = 2005;

    // ---- ENVIRONMENT SOUNDS ----
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

    // ---- UI SOUNDS ----
    constexpr int MENU_SELECT        = 4000;
    constexpr int MENU_MOVE          = 4001;
    constexpr int PAUSE              = 4002;
    constexpr int GAME_OVER          = 4003;
}

// ---- BACKGROUND MUSIC (BGM) ----
// ID của các nhạc nền (background music)
// Dùng bởi SoundManager::LoadBGM() và PlayBGM()
namespace BGM
{
    constexpr int OVERWORLD_THEME    = 5000;  // Nhạc overworld (level thường)
    constexpr int UNDERWORLD_THEME  = 5001;
    constexpr int WATER_THEME        = 5002;
    constexpr int CASTLE_THEME       = 5003;
    constexpr int STAR_THEME         = 5004;
    constexpr int MENU_THEME         = 5005;
    constexpr int GAME_OVER_THEME    = 5006;
    constexpr int WARNING_THEME     = 5007;  // Time warning (100s remaining)
    constexpr int STAGE_CLEAR_THEME  = 5008;  // Stage clear BGM
}
