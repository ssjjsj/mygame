#include "Render.h"

Render::Render(RenderDevice *device)
{
	renderDevice = device;
}


void Render::draw(vector<RenderAble*> renderAbles)
{
	for (int i = 0; i < renderAbles.size(); i++)
	{
		RenderAble *renderAble = renderAbles[i];

		Geometry *g = renderAble->getGeometry();
		Material *m = renderAble->getMaterial();

		ID3D11Device* d3dDevice = renderDevice->d3dDevice;
		ID3D11DeviceContext* immediateContext = renderDevice->immediateContext;
		IDXGISwapChain* swapChain = renderDevice->swapChain;

		float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		immediateContext->ClearRenderTargetView(renderTargetView, color);
		immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//set constant

		UINT stride = (UINT)Geometry::getVertexSize(g->getVertexType());
		UINT offset = 0;
		ID3D11Buffer *vsBuffer = g->getVertexBuffer();
		ID3D11Buffer *psBuffer = g->getIndexBuffer();
		immediateContext->IASetVertexBuffers(0, 1, &vsBuffer, &stride, &offset);
		immediateContext->IASetIndexBuffer(psBuffer, DXGI_FORMAT_R32_UINT, 0);

		immediateContext->IASetInputLayout(g->getLayout());
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		immediateContext->VSSetShader(m->getShader()->getVsShader(), NULL, 0);
		immediateContext->PSSetShader(m->getShader()->getPsShader(), NULL, 0);


		immediateContext->DrawIndexed(g->getIndexCount(), 0, 0);

		swapChain->Present(0, 0);
	}
}



void Render::onReset()
{
	renderTargetView->Release();
	depthStencilBuffer->Release();
	depthStencilView->Release();

	ID3D11Device* d3dDevice = renderDevice->d3dDevice;
	ID3D11DeviceContext* immediateContext = renderDevice->immediateContext;
	IDXGISwapChain* swapChain = renderDevice->swapChain;
	int width = renderDevice->width;
	int height = renderDevice->height;

	// Resize the swap chain and recreate the render target view.

	swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* backBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	d3dDevice->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
	backBuffer->Release();

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width     = width;
	depthStencilDesc.Height    = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//// Use 4X MSAA? --must match swap chain MSAA values.
	//if( mEnable4xMsaa )
	//{
	//	depthStencilDesc.SampleDesc.Count   = 4;
	//	depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality-1;
	//}
	//// No MSAA
	//else
	//{
	//	depthStencilDesc.SampleDesc.Count   = 1;
	//	depthStencilDesc.SampleDesc.Quality = 0;
	//}

	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);
	d3dDevice->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView);


	// Bind the render target view and depth/stencil view to the pipeline.

	immediateContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	

	// Set the viewport transform.

	screenViewport.TopLeftX = 0;
	screenViewport.TopLeftY = 0;
	screenViewport.Width = static_cast<float>(width);
	screenViewport.Height = static_cast<float>(height);
	screenViewport.MinDepth = 0.0f;
	screenViewport.MaxDepth = 1.0f;

	immediateContext->RSSetViewports(1, &screenViewport);
}



