#pragma once

#include <d3d10.h>
#include <d3dx10.h>

// Wrapper cho texture Direct3D 10
// Được SpriteManager dùng để load và lưu ảnh texture
// TextureManager quản lý vòng đời của các đối tượng Texture
class Texture
{
protected:
	ID3D10Texture2D* _tex;              // Resource texture D3D10
	ID3D10ShaderResourceView* _rsview;  // View để bind texture vào shaders
	int _width;                         // Chiều rộng texture (pixel)
	int _height;                        // Chiều cao texture (pixel)
public:
	Texture()
	{
		_tex = NULL;
		_rsview = NULL;
		_width = -1;
		_height = -1;
	}

	Texture(ID3D10Texture2D* tex, ID3D10ShaderResourceView* rsview)
	{
		this->_tex = tex;
		this->_rsview = rsview;

		D3D10_TEXTURE2D_DESC desc;
		this->_tex->GetDesc(&desc);
		this->_width = desc.Width;
		this->_height = desc.Height;
	}

	// Lấy shader resource view - cần thiết để render
	ID3D10ShaderResourceView* getShaderResourceView() { return this->_rsview; }

	int getWidth() { return this->_width; }
	int getHeight() { return this->_height; }

	~Texture()
	{
		if (_rsview != NULL) this->_rsview->Release();
		if (_tex != NULL) this->_tex->Release();
	}
};

typedef Texture* LPTEXTURE;

