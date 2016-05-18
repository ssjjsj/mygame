#include "textureRenderTarget.h"
#include "global.h"
#include "Render.h"


TextureRenderTarget::TextureRenderTarget()
{
}


TextureRenderTarget::~TextureRenderTarget()
{
	if (tex != NULL)
		tex->Release();

	if (resView != NULL)
		resView->Release();

	if (targetView != NULL)
		targetView->Release();
}


void TextureRenderTarget::init(DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = gRender->Device()->width;
	desc.Height = gRender->Device()->height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	gRender->Device()->d3dDevice->CreateTexture2D(&desc, NULL, &tex);


	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = format;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	gRender->Device()->d3dDevice->CreateShaderResourceView(tex, &SRVDesc, &resView);



	D3D11_RENDER_TARGET_VIEW_DESC targetDesc;
	targetDesc.Format = format;
	targetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	targetDesc.Texture2D.MipSlice = 0;
	gRender->Device()->d3dDevice->CreateRenderTargetView(tex, &targetDesc, &targetView);
}



DepthTexture::DepthTexture()
{

}


DepthTexture::~DepthTexture()
{
	if (tex != NULL)
		tex->Release();

	if (resView != NULL)
		resView->Release();

	if (depthView != NULL)
		depthView->Release();
}


void DepthTexture::init(DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = gRender->Device()->width;
	desc.Height = gRender->Device()->height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	gRender->Device()->d3dDevice->CreateTexture2D(&desc, NULL, &tex);


	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D.MipLevels = 1;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	gRender->Device()->d3dDevice->CreateShaderResourceView(tex, &SRVDesc, &resView);



	D3D11_DEPTH_STENCIL_VIEW_DESC  depthViewDes;
	depthViewDes.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDes.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthViewDes.Texture2D.MipSlice = 0;
	depthViewDes.Flags = 0;
	gRender->Device()->d3dDevice->CreateDepthStencilView(tex, &depthViewDes, &depthView);
}