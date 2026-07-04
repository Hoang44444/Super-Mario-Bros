#include <Windows.h>
#include <d3d10.h>
#include <d3dx10.h>

#include "TextureManager.h"
#include "Renderer.h"
#include "debug.h"

// Khởi tạo instance singleton tĩnh
TextureManager* TextureManager::__instance = NULL;

TextureManager::TextureManager()
{
	// Constructor - map textures được khởi tạo mặc định
}

TextureManager* TextureManager::GetInstance()
{
	if (__instance == NULL) __instance = new TextureManager();
	return __instance;
}

void TextureManager::Add(int id, LPCWSTR filePath)
{
	// Load texture từ file và lưu với ID cho trước
	textures[id] = LoadTexture(filePath);
}

LPTEXTURE TextureManager::Get(unsigned int i)
{
	LPTEXTURE t = textures[i];
	if (t == NULL)
		DebugOut(L"[ERROR] Texture Id %d not found !\n", i);

	return t;
}

void TextureManager::Clear()
{
	// Xóa tất cả đối tượng Texture để giải phóng bộ nhớ GPU
	for (auto x : textures)
	{
		LPTEXTURE tex = x.second;
		if (tex != NULL) delete tex;
	}

	textures.clear();
}

LPTEXTURE TextureManager::LoadTexture(LPCWSTR texturePath)
{
	ID3D10Resource* pD3D10Resource = NULL;
	ID3D10Texture2D* tex = NULL;

	// Bước 1: Lấy thông tin ảnh (chiều rộng, chiều cao, định dạng, v.v.)
	D3DX10_IMAGE_INFO imageInfo;
	HRESULT hr = D3DX10GetImageInfoFromFile(texturePath, NULL, &imageInfo, NULL);
	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] D3DX10GetImageInfoFromFile failed for  file: %s with error: %d\n", texturePath, hr);
		return NULL;
	}

	// Bước 2: Cấu hình cách load texture
	D3DX10_IMAGE_LOAD_INFO info;
	ZeroMemory(&info, sizeof(D3DX10_IMAGE_LOAD_INFO));
	info.Width = imageInfo.Width;
	info.Height = imageInfo.Height;
	info.Depth = imageInfo.Depth;
	info.FirstMipLevel = 0;
	info.MipLevels = 1;  // Không dùng mipmapping cho đơn giản
	info.Usage = D3D10_USAGE_DEFAULT;
	info.BindFlags = D3D10_BIND_SHADER_RESOURCE;  // Cho phép bind vào shaders
	info.CpuAccessFlags = 0;
	info.MiscFlags = 0;
	info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // Định dạng RGBA chuẩn
	info.Filter = D3DX10_FILTER_NONE;
	info.MipFilter = D3DX10_FILTER_NONE;
	info.pSrcInfo = &imageInfo;

	// Bước 3: Load file texture vào bộ nhớ GPU
	ID3D10Device* device = Renderer::GetInstance()->GetDevice();
	hr = D3DX10CreateTextureFromFile(device,
		texturePath,
		&info,
		NULL,
		&pD3D10Resource,
		NULL);

	if (FAILED(hr))
	{
		DebugOut(L"[ERROR] Failed to load texture file: %s with error: %d\n", texturePath, hr);
		return NULL;
	}

	// Bước 4: Chuyển resource generic sang Texture2D
	pD3D10Resource->QueryInterface(__uuidof(ID3D10Texture2D), (LPVOID*)&tex);
	pD3D10Resource->Release();

	if (!tex) {
		DebugOut(L"[ERROR] Failed to convert from ID3D10Resource to ID3D10Texture2D \n");
		return NULL;
	}

	// Bước 5: Tạo shader resource view (cần thiết để render)
	D3D10_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);

	D3D10_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	ZeroMemory(&SRVDesc, sizeof(SRVDesc));
	SRVDesc.Format = desc.Format;
	SRVDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = desc.MipLevels;

	ID3D10ShaderResourceView* gSpriteTextureRV = NULL;
	device->CreateShaderResourceView(tex, &SRVDesc, &gSpriteTextureRV);

	// Bước 6: Bọc trong đối tượng Texture và trả về
	return new Texture(tex, gSpriteTextureRV);
}
