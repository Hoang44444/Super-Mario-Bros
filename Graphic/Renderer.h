#pragma once
#include <d3d10.h>
#include <d3dx10.h>
#include "Texture.h"

class Renderer
{
    static Renderer* __instance;

    ID3D10Device* pD3DDevice = NULL;
    IDXGISwapChain* pSwapChain = NULL;
    ID3D10RenderTargetView* pRenderTargetView = NULL;
    ID3D10BlendState* pBlendStateAlpha = NULL;
    ID3D10SamplerState* pPointSamplerState = NULL;
    ID3DX10Sprite* spriteObject = NULL;

    int backBufferWidth = 0;
    int backBufferHeight = 0;

    float globalScale = 1.0f;

public:
    static const int INTERNAL_SCREEN_WIDTH = 440; 
    static const int INTERNAL_SCREEN_HEIGHT = 240; 

    static Renderer* GetInstance();

    void Init(HWND hWnd, HINSTANCE hInstance);

    float GetGlobalScale() { return globalScale; }

    // Các hàm vẽ
    void BeginRender();
    void EndRender();
    void Draw(float x, float y, LPTEXTURE tex, RECT* rect = nullptr, float alpha = 1.0f);
    void DrawScaled(float x, float y, LPTEXTURE tex, float dest_width, float dest_height, RECT* rect = nullptr, float alpha = 1.0f);

    // Getters
    ID3D10Device* GetDevice() { return pD3DDevice; }
    ID3DX10Sprite* GetSpriteHandler() { return spriteObject; }
    int GetBackBufferWidth() { return backBufferWidth; }
    int GetBackBufferHeight() { return backBufferHeight; }
    int GetInternalWidth() { return INTERNAL_SCREEN_WIDTH; }
    int GetInternalHeight() { return INTERNAL_SCREEN_HEIGHT; }
    LPTEXTURE GetTexture(LPCWSTR filePath);
    ~Renderer();
};
