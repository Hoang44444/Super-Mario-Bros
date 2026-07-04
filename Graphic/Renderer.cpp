#include "Renderer.h"
#include "Camera.h"
#include <cwchar>
#include "debug.h"

Renderer* Renderer::__instance = NULL;

Renderer* Renderer::GetInstance()
{
    if (__instance == NULL) __instance = new Renderer();
    return __instance;
}

void Renderer::Init(HWND hWnd, HINSTANCE hInstance)
{
	OutputDebugString(L"[INIT] Renderer::Init called!\n");

	// Bước 1: Lấy kích thước client area của cửa sổ
	RECT r;
	GetClientRect(hWnd, &r);

	backBufferWidth = r.right - r.left;
	backBufferHeight = r.bottom - r.top;

	// Bước 2: Tính global scale (tỷ lệ giữa actual resolution và logical resolution)
	// Scale dựa trên chiều cao để đảm bảo aspect ratio đúng
	globalScale = (float)backBufferHeight / (float)INTERNAL_SCREEN_HEIGHT;

	// Bước 3: Cấu hình swap chain (để present frame lên màn hình)
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Fill các thông số swap chain
	swapChainDesc.BufferCount = 1;  // Số buffer (double buffering = 2, single = 1)
	swapChainDesc.BufferDesc.Width = backBufferWidth;
	swapChainDesc.BufferDesc.Height = backBufferHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // RGBA 8-bit
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;  // 60 FPS
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // Dùng để render
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;  // No multisampling
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;  // Windowed mode (không fullscreen)

	// Bước 4: Tạo D3D10 device và swap chain
	// D3D10_DRIVER_TYPE_HARDWARE: dùng GPU (nếu không có sẽ dùng software renderer)
	HRESULT hr = D3D10CreateDeviceAndSwapChain(NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		0,  // No debug layer
		D3D10_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pD3DDevice);

	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] D3D10CreateDeviceAndSwapChain has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// Bước 5: Lấy back buffer từ swap chain
	ID3D10Texture2D* pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] pSwapChain->GetBuffer has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// Bước 6: Tạo render target view từ back buffer
	// Render target view cho phép D3D render vào back buffer
	hr = pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	pBackBuffer->Release();  // Release back buffer sau khi tạo RTV
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] CreateRenderTargetView has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// Bước 7: Set render target (nơi sẽ render vào)
	pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// Bước 8: Tạo và set viewport (vùng hiển thị trên màn hình)
	D3D10_VIEWPORT viewPort;
	viewPort.Width = backBufferWidth;
	viewPort.Height = backBufferHeight;
	viewPort.MinDepth = 0.0f;  // Depth buffer min
	viewPort.MaxDepth = 1.0f;  // Depth buffer max
	viewPort.TopLeftX = 0;     // Vị trí X của viewport
	viewPort.TopLeftY = 0;     // Vị trí Y của viewport
	pD3DDevice->RSSetViewports(1, &viewPort);

	DebugOut(L"[INFO] Viewport set: %d x %d | Global Scale: %f\n", backBufferWidth, backBufferHeight, globalScale);

	// Bước 9: Tạo sampler state cho texture sampling
	// Sampler state quyết định cách texture được sampled (filter, wrap mode, v.v.)
	D3D10_SAMPLER_DESC desc;
	desc.Filter = D3D10_FILTER_MIN_MAG_POINT_MIP_LINEAR;  // Point filter cho pixel art look
	desc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;  // Clamp texture coordinate
	desc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D10_COMPARISON_NEVER;
	desc.BorderColor[0] = 1.0f;
	desc.BorderColor[1] = 1.0f;
	desc.BorderColor[2] = 1.0f;
	desc.BorderColor[3] = 1.0f;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;

	pD3DDevice->CreateSamplerState(&desc, &this->pPointSamplerState);

	// Bước 10: Tạo sprite object (D3DX10 helper để vẽ sprite)
	// Sprite object đơn giản hóa việc vẽ 2D sprites
	hr = D3DX10CreateSprite(pD3DDevice, 0, &spriteObject);

	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] D3DX10CreateSprite has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// Bước 11: Tạo projection matrix cho sprite object
	// Projection matrix chuyển đổi từ logical resolution sang screen space
	// Divide by globalScale để tạo "Virtual Resolution" (zoom in)
	D3DXMATRIX matProjection;
	D3DXMatrixOrthoOffCenterLH(&matProjection,
		(float)viewPort.TopLeftX,
		(float)viewPort.Width / globalScale,
		(float)viewPort.TopLeftY,  // Bottom = 0
		(float)viewPort.Height / globalScale,  // Top = Height
		0.1f,  // Near plane
		10);   // Far plane
	hr = spriteObject->SetProjectionTransform(&matProjection);

	// Bước 12: Set view transform (identity = không có camera transformation ở đây)
	// Camera transformation được xử lý riêng trong Draw()
	D3DXMATRIX matView;
	D3DXMatrixIdentity(&matView);
	spriteObject->SetViewTransform(&matView);

	// Bước 13: Tạo blend state cho alpha blending (để vẽ sprite trong suốt)
	// Alpha blending cho phép sprite có độ trong suốt (transparency)
	D3D10_BLEND_DESC StateDesc;
	ZeroMemory(&StateDesc, sizeof(D3D10_BLEND_DESC));
	StateDesc.AlphaToCoverageEnable = FALSE;
	StateDesc.BlendEnable[0] = TRUE;  // Enable blending cho render target 0
	StateDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;  // Source * srcAlpha
	StateDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;  // Destination * (1 - srcAlpha)
	StateDesc.BlendOp = D3D10_BLEND_OP_ADD;  // Source + Destination
	StateDesc.SrcBlendAlpha = D3D10_BLEND_ONE;
	StateDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	StateDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	StateDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;  // Write tất cả channels
	pD3DDevice->CreateBlendState(&StateDesc, &this->pBlendStateAlpha);

	DebugOut((wchar_t*)L"[INFO] InitDirectX has been successful\n");

	return;
}

void Renderer::BeginRender()
{
    // Bước 1: Clear render target với màu nền xám
    float color[4] = { 0.2f, 0.2f, 0.2f, 1.0f };  // RGBA (xám, alpha = 1.0f)
    pD3DDevice->ClearRenderTargetView(pRenderTargetView, color);

    // Bước 2: Set blend state cho alpha blending
    pD3DDevice->OMSetBlendState(pBlendStateAlpha, NULL, 0xffffffff);
    
    // Bước 3: Set sampler state cho texture sampling
    pD3DDevice->PSSetSamplers(0, 1, &this->pPointSamplerState);
    
    // Bước 4: Bắt đầu vẽ sprite (sort theo depth để đúng thứ tự)
    spriteObject->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);
}

void Renderer::Draw(float x, float y, float z, LPTEXTURE tex, RECT* rect, float alpha, bool flipX)
{
	// Delegate cho DrawScaled với kích thước gốc của texture/rect
	if (tex == nullptr) return;

	// Tính toán kích thước từ rect hoặc toàn bộ texture
	float width = (rect != NULL) ? (float)(rect->right - rect->left) : (float)tex->getWidth();
	float height = (rect != NULL) ? (float)(rect->bottom - rect->top) : (float)tex->getHeight();

	DrawScaled(x, y, z, tex, width, height, rect, alpha, flipX);
}

void Renderer::DrawScaled(float x, float y, float z, LPTEXTURE tex, float dest_width, float dest_height, RECT* rect, float alpha, bool flipX)
{
	if (tex == NULL) {
		DebugOut(L"[ERROR] DrawScaled called with NULL texture\n");
		return;
	}

	// Bước 1: Chuyển từ world coordinate sang screen coordinate (trừ camera offset)
	float screen_x = x - Camera::GetInstance()->GetX();
	float screen_y = y - Camera::GetInstance()->GetY();

	// Bước 2: Tạo sprite structure
	D3DX10_SPRITE sprite;
	ZeroMemory(&sprite, sizeof(D3DX10_SPRITE));

	sprite.pTexture = tex->getShaderResourceView();  // Texture để vẽ
	sprite.TextureIndex = 0;

	// Bước 3: Tính toán texture coordinate (UV)
	// Nếu có rect, chỉ vẽ vùng đó; nếu không, vẽ toàn bộ texture
	if (rect != NULL)
	{
		sprite.TexCoord.x = (float)rect->left / tex->getWidth();
		sprite.TexCoord.y = (float)rect->top / tex->getHeight();
		sprite.TexSize.x = (float)(rect->right - rect->left) / tex->getWidth();
		sprite.TexSize.y = (float)(rect->bottom - rect->top) / tex->getHeight();
	}
	else
	{
		sprite.TexCoord = D3DXVECTOR2(0, 0);  // Top-left của texture
		sprite.TexSize = D3DXVECTOR2(1, 1);  // Toàn bộ texture
	}

	// Bước 4: Set color modulate (RGBA)
	// RGB = 1.0f (giữ nguyên màu gốc), Alpha = parameter
	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);

	// Bước 5: Tính toán world matrix (position + scale)
	D3DXMATRIX matWorld, matTranslation, matScale;
	
	// Tính toán vị trí vẽ (center của sprite)
	float draw_x = screen_x + dest_width / 2.0f;
	// Y coordinate: flip vì D3D có origin ở bottom-left, game có origin ở top-left
	float logicalHeight = (float)backBufferHeight / globalScale;
	float draw_y = logicalHeight - (screen_y + dest_height / 2.0f);

	// Tạo translation và scale matrix
	D3DXMatrixTranslation(&matTranslation, draw_x, draw_y, z);
	D3DXMatrixScaling(&matScale, flipX ? -dest_width : dest_width, dest_height, 1.0f);

	// Combine: Scale trước, Translation sau
	matWorld = matScale * matTranslation;
	sprite.matWorld = matWorld;

	// Bước 6: Vẽ sprite
	spriteObject->DrawSpritesBuffered(&sprite, 1);
}

void Renderer::EndRender()
{
    // Kết thúc vẽ sprite (flush tất cả sprite đã được buffered)
    spriteObject->End();
    
    // Present frame lên màn hình (swap back buffer với front buffer)
    // Parameter 0, 0: không v-sync (present ngay lập tức)
    pSwapChain->Present(0, 0);
}

LPTEXTURE Renderer::GetTexture(LPCWSTR filePath)
{
    // Load texture từ file (không dùng TextureManager, dùng trực tiếp)
    // Phương thức này ít được dùng - nên dùng TextureManager thay thế
    if (filePath == NULL || pD3DDevice == NULL)
    {
        return NULL;
    }

    ID3D10Resource* pD3D10Resource = NULL;
    ID3D10Texture2D* tex = NULL;

    // Bước 1: Lấy thông tin ảnh từ file
    D3DX10_IMAGE_INFO imageInfo;
    HRESULT hr = D3DX10GetImageInfoFromFile(filePath, NULL, &imageInfo, NULL);
    if (FAILED(hr))
    {
        DebugOut(L"[ERROR] D3DX10GetImageInfoFromFile failed for file: %s\n", filePath);
        return NULL;
    }

    // Bước 2: Cấu hình cách load texture
    D3DX10_IMAGE_LOAD_INFO info;
    ZeroMemory(&info, sizeof(D3DX10_IMAGE_LOAD_INFO));
    info.Width = imageInfo.Width;
    info.Height = imageInfo.Height;
    info.Depth = imageInfo.Depth;
    info.FirstMipLevel = 0;
    info.MipLevels = imageInfo.MipLevels;
    info.Usage = D3D10_USAGE_DEFAULT;
    info.BindFlags = D3D10_BIND_SHADER_RESOURCE;  // Cho phép bind vào shaders
    info.CpuAccessFlags = 0;
    info.MiscFlags = 0;

    // Bước 3: Load texture từ file vào bộ nhớ GPU
    hr = D3DX10CreateTextureFromFile(pD3DDevice, filePath, &info, NULL, &pD3D10Resource, NULL);
    if (FAILED(hr))
    {
        DebugOut(L"[ERROR] D3DX10CreateTextureFromFile failed for file: %s\n", filePath);
        return NULL;
    }

    // Bước 4: Chuyển resource generic sang Texture2D
    hr = pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&tex);
    if (FAILED(hr))
    {
        DebugOut(L"[ERROR] QueryInterface failed for texture: %s\n", filePath);
        pD3D10Resource->Release();
        return NULL;
    }

    // Bước 5: Tạo shader resource view (cần thiết để render)
    ID3D10ShaderResourceView* spriteResourceView = NULL;
    D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory(&SRVDesc, sizeof(SRVDesc));
    SRVDesc.Format = imageInfo.Format;
    SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MostDetailedMip = 0;
    SRVDesc.Texture2D.MipLevels = imageInfo.MipLevels;

    hr = pD3DDevice->CreateShaderResourceView(tex, &SRVDesc, &spriteResourceView);
    pD3D10Resource->Release();

    if (FAILED(hr))
    {
        DebugOut(L"[ERROR] CreateShaderResourceView failed for texture: %s\n", filePath);
        tex->Release();
        return NULL;
    }

    // Bước 6: Bọc trong đối tượng Texture và trả về
    LPTEXTURE texture = new Texture(tex, spriteResourceView);
    return texture;
}

Renderer::~Renderer()
{
    // Release tất cả Direct3D resources
    if (spriteObject) spriteObject->Release();
    if (pRenderTargetView) pRenderTargetView->Release();
    if (pSwapChain) pSwapChain->Release();
    if (pD3DDevice) pD3DDevice->Release();
}