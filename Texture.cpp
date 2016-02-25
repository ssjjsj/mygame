#include "Texture.h"
#include <D3DX11.h>
#include "Render.h"
#include "global.h"

Texture::Texture(string name)
{
	createTexture(name);
}


Texture::~Texture()
{
	if (texture != NULL)
		texture->Release();
}


void Texture::createTexture(string name)
{
	name = TexturePath + name;
	std::wstring stemp = std::wstring(name.begin(), name.end());
	LPCWSTR texName = stemp.c_str();
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(gRender->Device()->d3dDevice, texName, NULL, NULL, &texture, NULL);

	if (FAILED(hr))
	{
		int i = 3;
	}
}