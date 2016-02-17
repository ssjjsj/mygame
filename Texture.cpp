#include "Texture.h"
#include <D3DX11.h>
#include "Render.h"

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
	std::wstring stemp = std::wstring(name.begin(), name.end());
	LPCWSTR texName = stemp.c_str();
	D3DX11CreateShaderResourceViewFromFile(gRender->Device()->d3dDevice, texName, NULL, NULL, &texture, NULL);
}