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

public:
    static Renderer* GetInstance();

    void Init(HWND hWnd, HINSTANCE hInstance);

    // Các hàm vẽ
    void BeginRender();
    void EndRender();
    void Draw(float x, float y, LPTEXTURE tex, RECT* rect = NULL, float alpha = 1.0f);

    // Getters
    ID3D10Device* GetDevice() { return pD3DDevice; }
    ID3DX10Sprite* GetSpriteHandler() { return spriteObject; }
    int GetBackBufferWidth() { return backBufferWidth; }
    int GetBackBufferHeight() { return backBufferHeight; }
    ~Renderer();
};
