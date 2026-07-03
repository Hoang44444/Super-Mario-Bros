# Mario Scoring System Documentation

## Overview
The scoring system has been completely redesigned to match the original NES Super Mario Bros. behavior while maintaining clean, modular, and extensible code.

## Files Modified

### New Files Created
1. **GamePlay/Core/ScoreManager.h** - Centralized score management system header
2. **GamePlay/Core/ScoreManager.cpp** - Centralized score management system implementation

### Modified Files
1. **GamePlay/Objects/Player/Mario.h** - Updated to use ScoreManager API
2. **GamePlay/Objects/Player/Mario.cpp** - Added ground state tracking for combo system
3. **GamePlay/Objects/Enemy/Enemy.h** - Added OnHitByStar method for star power kills
4. **GamePlay/Goomba.cpp** - Integrated combo scoring for stomps
5. **GamePlay/Koopa.cpp** - Integrated combo scoring for stomps
6. **GamePlay/BuzzyBeetle.cpp** - Integrated combo scoring for stomps
7. **GamePlay/Spiny.cpp** - Added shell kill scoring
8. **GamePlay/Lakitu.cpp** - Integrated combo scoring for stomps
9. **GamePlay/Coin.cpp** - Removed hardcoded score, uses ScoreManager
10. **GamePlay/Objects/Static/QuestionBlock.cpp** - Removed hardcoded score, uses ScoreManager
11. **GamePlay/Objects/Static/Brick.cpp** - Added brick destruction scoring
12. **GamePlay/Objects/Item/Mushroom.cpp** - Added Super Mushroom scoring
13. **GamePlay/Objects/Item/FireFlower.cpp** - Added Fire Flower scoring
14. **GamePlay/Objects/Item/SuperStar.cpp** - Added Starman scoring
15. **GamePlay/Scenes/HUD.cpp** - Updated to read from ScoreManager
16. **GamePlay/GamePlay.vcxproj** - Added ScoreManager files to project

## Architecture Changes

### Centralized Score Management
The new `ScoreManager` class is a singleton that manages all scoring logic:
- Stores score, coins, and lives in `PlayerData` (existing singleton)
- Provides clean API: `AddScore()`, `GetScore()`, `ResetScore()`
- Manages combo state and ground detection
- Handles coin-to-1UP conversion (100 coins = 1 extra life)
- Caps score at 999,999 to prevent overflow

### Score Constants
All score values are defined as constants in `SCORE_VALUES` namespace:
- `GOOMBA_STOMP = 100`
- `KOOPA_STOMP = 100`
- `COIN = 200`
- `BRICK_DESTROYED = 50`
- `SUPER_MUSHROOM = 1000`
- `FIRE_FLOWER = 1000`
- `STARMAN = 1000`
- `SHELL_KICK = 100`
- `FIREBALL_KILL = 100`
- `STAR_KILL = 100`
- `QUESTION_BLOCK_COIN = 200`

### Combo System
The combo system follows the classic Mario progression:
- Level 1: 100 points
- Level 2: 200 points
- Level 3: 400 points
- Level 4: 800 points
- Level 5: 1,000 points
- Level 6: 2,000 points
- Level 7: 4,000 points
- Level 8: 8,000 points
- Level 9+: 1UP instead of points

Combo increments on:
- Enemy stomps (Goomba, Koopa, BuzzyBeetle, Lakitu)
- Star power kills

Combo resets when:
- Mario lands on the ground
- Tracked via `ScoreManager::SetOnGround()` called from Mario's Update()

### Star Power Combo
Enemies defeated while Starman is active use the same combo progression as stomps.
- Implemented via `Enemy::OnHitByStar(Mario*)` method
- Awards combo points or 1UP at max combo
- Combo state is shared between stomps and star kills

### Coin System
Each collected coin:
- Adds 1 coin count
- Awards 200 points automatically
- At 100 coins: subtracts 100 coins and awards 1 extra life

### Maximum Score
Score is capped at 999,999. Additional points do not overflow the score.

## How Combo State is Managed

1. **Ground Detection**: Mario's `Update()` method tracks `isOnGround` state
2. **State Change Notification**: When ground state changes, Mario notifies `ScoreManager` via `SetOnGround()`
3. **Combo Reset**: `ScoreManager` resets combo when transitioning from air to ground
4. **Combo Increment**: Enemies call `ScoreManager::IncrementCombo()` on kill
5. **Score Calculation**: `ScoreManager::GetComboScore()` returns appropriate score based on combo level
6. **Max Combo Handling**: At max combo (8), kills award 1UP instead of points

## How to Add New Score Events

To add a new scoring event:

1. **Define the score constant** in `ScoreManager.h` under `SCORE_VALUES` namespace:
```cpp
namespace SCORE_VALUES
{
    constexpr int NEW_EVENT_SCORE = 500;
    // ... existing constants
}
```

2. **Use the ScoreManager API** in your game object:
```cpp
#include "Core/ScoreManager.h"

void YourObject::OnSomeEvent()
{
    ScoreManager::Get().AddScore(SCORE_VALUES::NEW_EVENT_SCORE);
}
```

3. **For combo-based scoring** (enemy kills):
```cpp
ScoreManager& scoreMgr = ScoreManager::Get();
scoreMgr.IncrementCombo();

if (scoreMgr.IsMaxCombo())
{
    mario->AddLife(1);
}
else
{
    mario->AddScore(scoreMgr.GetComboScore());
}
```

4. **For non-combo scoring** (items, environmental events):
```cpp
ScoreManager::Get().AddScore(SCORE_VALUES::NEW_EVENT_SCORE);
```

## Integration Summary

All gameplay events now use the centralized scoring system:
- **Enemy stomps**: Combo scoring (Goomba, Koopa, BuzzyBeetle, Lakitu)
- **Shell kicks**: Fixed 100 points (no combo)
- **Fireball kills**: Fixed 100 points (no combo)
- **Star kills**: Combo scoring (same progression as stomps)
- **Coin collection**: 200 points + coin count
- **Brick destruction**: 50 points
- **Power-up collection**: 1,000 points (Mushroom, Fire Flower, Starman)
- **Question block coins**: 200 points (handled by coin system)

## Code Quality Improvements

- **Eliminated hardcoded scores** scattered throughout codebase
- **Single source of truth** for all score values
- **Consistent API** across all game objects
- **Separated concerns**: ScoreManager handles logic, game objects trigger events
- **No global variables**: Uses existing singleton pattern (ScoreManager + PlayerData)
- **Maintainable**: Easy to adjust score values in one location
- **Extensible**: Simple pattern for adding new score events

## Testing Recommendations

1. **Combo System**: Test stomping multiple enemies without touching ground
2. **Coin System**: Collect 100 coins and verify 1UP is awarded
3. **Score Cap**: Verify score stops at 999,999
4. **Star Power**: Test star kills use combo system
5. **Shell Kills**: Verify shell kicks award fixed 100 points
6. **HUD**: Verify score, coins, and lives display correctly
7. **Ground Reset**: Verify combo resets when landing

## Backward Compatibility

The implementation maintains backward compatibility:
- `PlayerData` still stores the actual values (score, coins, lives)
- Mario's `AddScore()`, `AddCoin()`, `AddLife()` methods still work
- HUD reads from same data source (via ScoreManager wrapper)
- Existing game save/load logic should continue to work
