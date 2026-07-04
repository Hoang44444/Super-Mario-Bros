#pragma once
#include "AnimationFrame.h"
#include <vector>
using namespace std;

// Animation quản lý chuỗi các frame để tạo hiệu ứng chuyển động
// Animation tự động chuyển frame dựa trên thời gian (ms)
// Cách dùng:
//   1. Tạo Animation
//   2. Add() từng frame với sprite và thời gian hiển thị
//   3. Render() mỗi frame - animation tự động chuyển frame
class Animation
{
    vector<AnimationFrame*> frames;  // Danh sách các frame

    int currentFrame;    // Index của frame đang hiển thị
    DWORD lastFrameTime; // Thời điểm frame cuối được render

public:
    Animation()
    {
        currentFrame = -1;  // -1 = chưa render lần nào
        lastFrameTime = 0;
    }

    // Thêm một frame vào animation
    // sprite: sprite của frame
    // time: thời gian hiển thị frame (ms)
    void Add(LPSPRITE sprite, DWORD time);
    
    // Render frame hiện tại tại vị trí (x, y, z)
    // Animation tự động chuyển frame nếu đủ thời gian
    void Render(float x, float y, float z);
    
    // Render frame hiện tại với flipX (lật ngang)
    void Render(float x, float y, float z, bool flipX);
    
    // Render frame hiện tại với kích thước tùy chỉnh (scale)
    // Dùng cho background toàn màn hình cần scale để fit logical screen
    void RenderScaled(float x, float y, float z, float destWidth, float destHeight);

    ~Animation(){}
};
typedef Animation* LPANIMATION;

