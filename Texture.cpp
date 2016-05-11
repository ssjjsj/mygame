#include "Texture.h"
#include <D3DX11.h>
#include "Render.h"
#include "global.h"
//#include "FreeImage.h"

Texture::Texture(string name)
{
	texture = NULL;
	createTexture(name);
}

Texture::Texture(ID3D11ShaderResourceView *tex)
{
	texture = tex;
}


Texture::~Texture()
{
	if (texture != NULL)
		texture->Release();
}


void Texture::createTexture(string fileName)
{
	string name = TexturePath + fileName;
	if (name.substr(name.size() - 3, 3) == "dds")
	{
		std::wstring stemp = std::wstring(name.begin(), name.end());
		LPCWSTR texName = stemp.c_str();
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile(gRender->Device()->d3dDevice, texName, NULL, NULL, &texture, NULL);
	}
	else
	{
		//FIBITMAP *bitmap;
		//if (name.substr(name.size() - 3, 3) == "bmp")
		//	bitmap = FreeImage_Load(FREE_IMAGE_FORMAT::FIF_BMP, name.c_str());
		//else if (name.substr(name.size() - 3, 3) == "jpg")
		//	bitmap = FreeImage_Load(FREE_IMAGE_FORMAT::FIF_JPEG, name.c_str());

		//if (bitmap != NULL)
		//{
		//	int width = FreeImage_GetWidth(bitmap);
		//	int height = FreeImage_GetHeight(bitmap);
		//	BYTE *data = FreeImage_GetBits(bitmap);
		//	int bpp = FreeImage_GetBPP(bitmap) / 8;
		//	BITMAPINFO *info = FreeImage_GetInfo(bitmap);

		//	D3D11_TEXTURE2D_DESC desc;
		//	desc.Width = width;
		//	desc.Height = height;
		//	desc.MipLevels = 1;
		//	desc.ArraySize = 1;
		//	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//	desc.SampleDesc.Count = 1;
		//	desc.SampleDesc.Quality = 0;
		//	desc.Usage = D3D11_USAGE_DEFAULT;
		//	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		//	desc.CPUAccessFlags = 0;
		//	desc.MiscFlags = 0;

		//	D3D11_SUBRESOURCE_DATA sourceData;
		//	sourceData.pSysMem = data;
		//	sourceData.SysMemPitch = bpp * width;
		//	sourceData.SysMemSlicePitch = bpp *width * height;

		//	ID3D11Texture2D *tex;
		//	//D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		//	gRender->Device()->d3dDevice->CreateTexture2D(&desc, &sourceData, &tex);

		//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		//	memset(&srvDesc, 0, sizeof(srvDesc));
		//	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		//	srvDesc.Texture2D.MipLevels = 1;
		//	HRESULT hr = gRender->Device()->d3dDevice->CreateShaderResourceView(tex, &srvDesc, &texture);
		//	if (FAILED(hr))
		//		int a = 3;
		//	FreeImage_Unload(bitmap);
	}
}