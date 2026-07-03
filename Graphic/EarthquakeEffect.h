#pragma once

// Hiệu ứng động đất: rung camera theo từng đợt (rung -> nghỉ -> rung -> ...).
// Toàn bộ logic chu kỳ rung được gói gọn ở đây; Camera chỉ cộng thêm offset
// mà class này tính ra vào vị trí của nó.
class EarthquakeEffect
{
private:
    bool  enabled    = false;  // có đang được lên lịch rung cho màn này không
    bool  shaking    = false;  // đang trong pha rung hay pha nghỉ
    float phaseTimer = 0;      // ms đã trôi qua trong pha hiện tại (rung hoặc nghỉ)
    float elapsed    = 0;      // ms trôi qua trong đợt rung hiện tại (dùng cho hàm sin)
    float offsetX    = 0;      // độ lệch camera do rung (px)
    float offsetY    = 0;

public:
    // Bắt đầu chu kỳ rung. Vào màn ở PHA NGHỈ: chưa rung ngay, đợi hết một chu kỳ
    // nghỉ rồi mới có đợt động đất đầu tiên.
    void Start();

    // Dừng hẳn, đưa offset về 0.
    void Stop();

    // Cập nhật chu kỳ rung mỗi frame (dt tính bằng ms).
    void Update(float dt);

    bool  IsActive()   const { return enabled; }
    bool  IsShaking()  const { return shaking; }
    float GetOffsetX() const { return offsetX; }
    float GetOffsetY() const { return offsetY; }
};
