#pragma once
#include "Texture.h"
class Sprite
{
    int id;
    int left, top, right, bottom;
    LPTEXTURE texture;

public:
    Sprite(int id, int left, int top, int right, int bottom, LPTEXTURE tex) {
        this->id = id;
        this->left = left;
        this->top = top;
        this->right = right;
        this->bottom = bottom;
        this->texture = tex;
    }
    LPTEXTURE GetTexture() { return texture; }
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

