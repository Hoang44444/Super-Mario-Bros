#include "EarthquakeEffect.h"
#include <cmath>

namespace
{
    constexpr float QUAKE_SHAKE_MS    = 5000.0f;    // mỗi đợt rung kéo dài 5 giây
    constexpr float QUAKE_INTERVAL_MS = 10000.0f;   // nghỉ 10 giây trước đợt rung kế tiếp
    constexpr float QUAKE_AMPLITUDE_Y = 4.0f;       // biên độ rung dọc (px)
    constexpr float QUAKE_AMPLITUDE_X = 2.0f;       // biên độ rung ngang (px)
    constexpr float QUAKE_OMEGA_Y     = 0.060f;     // tần số góc dọc (rad/ms) ~ 9.5 Hz
    constexpr float QUAKE_OMEGA_X     = 0.045f;     // tần số góc ngang (rad/ms) ~ 7.2 Hz
}

void EarthquakeEffect::Start()
{
    // Luôn khởi tạo lại chu kỳ khi vào màn để trạng thái sót lại từ lần trước
    // không làm sai lệch. Bắt đầu ở PHA NGHỈ: vào màn chưa rung, đợi hết một
    // chu kỳ nghỉ (QUAKE_INTERVAL_MS) rồi mới có đợt động đất đầu tiên.
    enabled    = true;
    shaking    = false;   // chưa rung, đang trong pha nghỉ
    phaseTimer = 0;
    elapsed    = 0;
    offsetX    = 0;
    offsetY    = 0;
}

void EarthquakeEffect::Stop()
{
    enabled    = false;
    shaking    = false;
    phaseTimer = 0;
    elapsed    = 0;
    offsetX    = 0;
    offsetY    = 0;
}

void EarthquakeEffect::Update(float dt)
{
    if (!enabled)
    {
        offsetX = offsetY = 0;
        return;
    }

    phaseTimer += dt;

    if (shaking)
    {
        elapsed += dt;

        if (phaseTimer >= QUAKE_SHAKE_MS)
        {
            // hết đợt rung -> chuyển sang nghỉ
            shaking    = false;
            phaseTimer = 0;
            offsetX = offsetY = 0;
        }
        else
        {
            // envelope giảm dần để đợt rung kết thúc êm (không giật về 0 đột ngột)
            float k = 1.0f - phaseTimer / QUAKE_SHAKE_MS;
            offsetY = QUAKE_AMPLITUDE_Y * k * sinf(QUAKE_OMEGA_Y * elapsed);
            offsetX = QUAKE_AMPLITUDE_X * k * sinf(QUAKE_OMEGA_X * elapsed);
        }
    }
    else
    {
        offsetX = offsetY = 0;

        if (phaseTimer >= QUAKE_INTERVAL_MS)
        {
            // hết pha nghỉ -> rung tiếp
            shaking    = true;
            phaseTimer = 0;
            elapsed    = 0;
        }
    }
}
