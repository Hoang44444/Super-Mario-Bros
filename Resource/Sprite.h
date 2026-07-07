#pragma once
#include "Texture.h"

// Đại diện cho một vùng hình chữ nhật (sprite) trên texture
// Sprite dùng để định nghĩa các frame hoặc đối tượng riêng từ sprite sheet
// SpriteManager lưu và quản lý tất cả sprite
// Cách dùng: SpriteManager::GetInstance()->Add(id, left, top, right, bottom, texture);
class Sprite
{
    int id;
    int left, top, right, bottom;  // Biên vùng trên texture (pixel) (left, top bao gồm, right, bottom loại trừ)
    LPTEXTURE texture;             // Texture nguồn mà sprite này tham chiếu

public:
    Sprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex) {
        this->id = id;
        this->left = left;
        this->top = top;
        this->right = right;
        this->bottom = bottom;
        this->texture = tex;
    }

    // Lấy texture nguồn cho sprite này
    LPTEXTURE GetTexture() { return texture; }

    // Lấy biên hình chữ nhật dưới dạng struct RECT (dùng bởi D3D rendering)
    RECT GetRect() {
        RECT r;
        r.left = left;
        r.top = top;
        r.right = right;
        r.bottom = bottom;
        return r;
    };
};
typedef Sprite* LPSPRITE;

