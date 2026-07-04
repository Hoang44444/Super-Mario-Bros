#pragma once
#include <d3d10.h>
#include <d3dx10.h>
#include "Texture.h"

// Renderer quản lý Direct3D 10 rendering pipeline
// Đây là singleton class xử lý tất cả các thao tác vẽ (draw)
// Cách dùng:
//   1. Gọi Init() khi khởi tạo game để setup Direct3D
//   2. Mỗi frame: BeginRender() -> Draw() các object -> EndRender()
//   3. GetInstance() để lấy singleton instance
class Renderer
{
    static Renderer* __instance;  // Instance singleton

    // Direct3D 10 objects
    ID3D10Device* pD3DDevice = NULL;              // D3D10 device (chính)
    IDXGISwapChain* pSwapChain = NULL;          // Swap chain (để present frame)
    ID3D10RenderTargetView* pRenderTargetView = NULL;  // Render target view (back buffer)
    ID3D10BlendState* pBlendStateAlpha = NULL;  // Blend state cho alpha blending
    ID3D10SamplerState* pPointSamplerState = NULL;  // Sampler state cho texture sampling
    ID3DX10Sprite* spriteObject = NULL;          // Sprite helper object (D3DX10)

    int backBufferWidth = 0;   // Chiều rộng back buffer (pixel)
    int backBufferHeight = 0;  // Chiều cao back buffer (pixel)

    float globalScale = 1.0f;  // Scale factor để scale từ logical resolution sang actual resolution

public:
    // Logical resolution của game (NES resolution)
    // Tất cả coordinate trong game dùng resolution này
    static const int INTERNAL_SCREEN_WIDTH = 440; 
    static const int INTERNAL_SCREEN_HEIGHT = 240; 

    // Lấy instance singleton - tạo mới nếu chưa tồn tại
    static Renderer* GetInstance();

    // Khởi tạo Direct3D 10 - phải gọi trước khi render
    void Init(HWND hWnd, HINSTANCE hInstance);

    // Lấy global scale (tính từ actual resolution / logical resolution)
    float GetGlobalScale() { return globalScale; }

    // --- RENDERING PIPELINE ---
    // Bắt đầu render frame (clear screen, setup blend state)
    void BeginRender();
    
    // Kết thúc render frame (present to screen)
    void EndRender();
    
    // Vẽ sprite tại vị trí (x, y, z) với kích thước gốc của texture/rect
    // rect: vùng trên texture để vẽ (NULL = vẽ toàn bộ texture)
    // alpha: độ trong suốt (0.0 = trong suốt, 1.0 = hoàn toàn)
    // flipX: lật ngang (true = lật)
    void Draw(float x, float y, float z, LPTEXTURE tex, RECT* rect = nullptr, float alpha = 1.0f, bool flipX = false);
    
    // Vẽ sprite với kích thước tùy chỉnh (scale)
    // dest_width/dest_height: kích thước đích (pixel)
    void DrawScaled(float x, float y, float z, LPTEXTURE tex, float dest_width, float dest_height, RECT* rect = nullptr, float alpha = 1.0f, bool flipX = false);

    // --- GETTERS ---
    ID3D10Device* GetDevice() { return pD3DDevice; }
    ID3DX10Sprite* GetSpriteHandler() { return spriteObject; }
    int GetBackBufferWidth() { return backBufferWidth; }
    int GetBackBufferHeight() { return backBufferHeight; }
    int GetInternalWidth() { return INTERNAL_SCREEN_WIDTH; }
    int GetInternalHeight() { return INTERNAL_SCREEN_HEIGHT; }
    
    // Load texture từ file (không dùng TextureManager, dùng trực tiếp)
    LPTEXTURE GetTexture(LPCWSTR filePath);
    
    // Destructor - release Direct3D resources
    ~Renderer();
};
