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

	// retrieve client area width & height so that we can create backbuffer height & width accordingly 
	RECT r;
	GetClientRect(hWnd, &r);

	backBufferWidth = r.right - r.left;
	backBufferHeight = r.bottom - r.top;

	// Create & clear the DXGI_SWAP_CHAIN_DESC structure
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Fill in the needed values
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = backBufferWidth;
	swapChainDesc.BufferDesc.Height = backBufferHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	// Create the D3D device and the swap chain
	HRESULT hr = D3D10CreateDeviceAndSwapChain(NULL,
		D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		D3D10_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pD3DDevice);

	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] D3D10CreateDeviceAndSwapChain has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// Get the back buffer from the swapchain
	ID3D10Texture2D* pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] pSwapChain->GetBuffer has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// create the render target view
	hr = pD3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);

	pBackBuffer->Release();
	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] CreateRenderTargetView has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	// set the render target
	pD3DDevice->OMSetRenderTargets(1, &pRenderTargetView, NULL);

	// create and set the viewport
	D3D10_VIEWPORT viewPort;
	viewPort.Width = backBufferWidth;
	viewPort.Height = backBufferHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	pD3DDevice->RSSetViewports(1, &viewPort);

	//
	//
	//

	D3D10_SAMPLER_DESC desc;
	desc.Filter = D3D10_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	desc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
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

	// create the sprite object to handle sprite drawing 
	hr = D3DX10CreateSprite(pD3DDevice, 0, &spriteObject);

	if (hr != S_OK)
	{
		DebugOut((wchar_t*)L"[ERROR] D3DX10CreateSprite has failed %s %d", _W(__FILE__), __LINE__);
		return;
	}

	D3DXMATRIX matProjection;

	// Create the projection matrix using the values in the viewport
	// Fix: swapped TopLeftY and Height to make Y increase downwards (top-left is 0,0)
	D3DXMatrixOrthoOffCenterLH(&matProjection,
		(float)viewPort.TopLeftX,
		(float)viewPort.Width,
		(float)viewPort.Height,
		(float)viewPort.TopLeftY,
		0.1f,
		10);
	hr = spriteObject->SetProjectionTransform(&matProjection);

	// Initialize the blend state for alpha drawing
	D3D10_BLEND_DESC StateDesc;
	ZeroMemory(&StateDesc, sizeof(D3D10_BLEND_DESC));
	StateDesc.AlphaToCoverageEnable = FALSE;
	StateDesc.BlendEnable[0] = TRUE;
	StateDesc.SrcBlend = D3D10_BLEND_SRC_ALPHA;
	StateDesc.DestBlend = D3D10_BLEND_INV_SRC_ALPHA;
	StateDesc.BlendOp = D3D10_BLEND_OP_ADD;
	StateDesc.SrcBlendAlpha = D3D10_BLEND_ZERO;
	StateDesc.DestBlendAlpha = D3D10_BLEND_ZERO;
	StateDesc.BlendOpAlpha = D3D10_BLEND_OP_ADD;
	StateDesc.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;
	pD3DDevice->CreateBlendState(&StateDesc, &this->pBlendStateAlpha);

	DebugOut((wchar_t*)L"[INFO] InitDirectX has been successful\n");

	return;
}

void Renderer::BeginRender()
{
    float color[4] = { 0.2f, 0.2f, 0.2f, 0.2f }; // Màu nền đen
    pD3DDevice->ClearRenderTargetView(pRenderTargetView, color);

    pD3DDevice->OMSetBlendState(pBlendStateAlpha, NULL, 0xffffffff);
    spriteObject->Begin(D3DX10_SPRITE_SORT_DEPTH_BACK_TO_FRONT);
}

void Renderer::Draw(float x, float y, LPTEXTURE tex, RECT* rect, float alpha)
{
	if (tex == NULL) {
		OutputDebugString(L"[ERROR] Draw called with NULL texture\n");
		return;
	}

	wchar_t log[256];
	float width = (rect != NULL) ? (float)(rect->right - rect->left) : (float)tex->getWidth();
	float height = (rect != NULL) ? (float)(rect->bottom - rect->top) : (float)tex->getHeight();
	float screen_x = x - Camera::GetInstance()->GetX();
	float screen_y = y - Camera::GetInstance()->GetY();

	swprintf_s(log, L"[DRAW] screen_x=%.1f screen_y=%.1f w=%.1f h=%.1f bufH=%d\n",
		screen_x, screen_y, width, height, backBufferHeight);
	OutputDebugString(log);

	Camera* cam = Camera::GetInstance();

	D3DX10_SPRITE sprite;
	ZeroMemory(&sprite, sizeof(D3DX10_SPRITE));

	sprite.pTexture = tex->getShaderResourceView();
	sprite.TextureIndex = 0;

	// Calculate texture coordinate and size
	if (rect != NULL)
	{
		sprite.TexCoord.x = (float)rect->left / tex->getWidth();
		sprite.TexCoord.y = (float)rect->top / tex->getHeight();
		sprite.TexSize.x = (float)(rect->right - rect->left) / tex->getWidth();
		sprite.TexSize.y = (float)(rect->bottom - rect->top) / tex->getHeight();
	}
	else
	{
		sprite.TexCoord = D3DXVECTOR2(0, 0);
		sprite.TexSize = D3DXVECTOR2(1, 1);
	}

	sprite.ColorModulate = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);

	// Set the world matrix for the sprite
	D3DXMATRIX matWorld, matTranslation, matScale;
	D3DXMatrixTranslation(&matTranslation, screen_x, screen_y, 1.0f);

	D3DXMatrixScaling(&matScale, width, height, 1.0f);

	matWorld = matScale * matTranslation;

	sprite.matWorld = matWorld;

	spriteObject->DrawSpritesBuffered(&sprite, 1);
}

void Renderer::EndRender()
{
    spriteObject->End();
    pSwapChain->Present(0, 0);
}

Renderer::~Renderer()
{
    if (spriteObject) spriteObject->Release();
    if (pRenderTargetView) pRenderTargetView->Release();
    if (pSwapChain) pSwapChain->Release();
    if (pD3DDevice) pD3DDevice->Release();
}