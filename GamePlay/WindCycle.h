#pragma once
#include <Windows.h>

// Trạng thái chu kỳ gió dùng CHUNG cho cả hiệu ứng lá (WindEffect) lẫn lực đẩy
// Mario, để hình ảnh và vật lý luôn đồng pha. Tính theo thời điểm vào màn:
// vào màn rơi vào pha NGHỈ trước, hết một chu kỳ nghỉ mới có đợt gió đầu tiên.
class WindCycle
{
private:
    static WindCycle* __instance;

    bool      enabled   = false;   // màn hiện tại có gió hay không
    ULONGLONG startTime = 0;       // thời điểm bắt đầu chu kỳ (lúc vào màn)

    WindCycle() {}

public:
    static WindCycle* GetInstance();

    void Start();           // gọi khi vào màn có gió: bật và reset chu kỳ
    void Stop();            // tắt gió (các màn không có gió)
    bool IsActive() const;  // gió ĐANG thổi hay không (đồng bộ ở mọi nơi gọi)
};
