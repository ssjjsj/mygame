#ifndef TEXTURERENDERTARGET_H
#define TEXTURERENDERTARGET_H
#include <D3D11.h>

class TextureRenderTarget
{
public:
	TextureRenderTarget();
	~TextureRenderTarget();

	void init(DXGI_FORMAT format);
	ID3D11RenderTargetView *getTargetView() { return targetView; }
	ID3D11ShaderResourceView *getResView() { return resView; }

private:
	ID3D11Texture2D *tex;
	ID3D11ShaderResourceView *resView;
	ID3D11RenderTargetView *targetView;
};

#endif