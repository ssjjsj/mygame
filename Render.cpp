#include "Render.h"
#include "MathHelp.h"

Render::Render(RenderDevice *device)
{
	renderDevice = device;
	renderTargetView = NULL;
	depthStencilBuffer = NULL;
	depthStencilView = NULL;
	matrixBuffer = NULL;
	camera = new Camera;

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderDevice->d3dDevice->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	onReset();
}

Render::~Render()
{
	delete renderDevice;

	if (matrixBuffer != NULL)
		matrixBuffer->Release();
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

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		immediateContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		XMFLOAT4X4* dataPtr = (XMFLOAT4X4*)mappedResource.pData;
		//XMMATRIX vpoj = camera->ViewProj();
		XMMATRIX vpoj = XMMatrixIdentity();
		XMStoreFloat4x4(dataPtr, vpoj);
		immediateContext->Unmap(matrixBuffer, 0);
		immediateContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

		

		UINT stride = (UINT)Geometry::getVertexSize(g->getVertexType());
		UINT offset = 0;
		ID3D11Buffer *vsBuffer = g->getVertexBuffer();
		ID3D11Buffer *psBuffer = g->getIndexBuffer();
		immediateContext->IASetVertexBuffers(0, 1, &vsBuffer, &stride, &offset);
		immediateContext->IASetIndexBuffer(psBuffer, DXGI_FORMAT_R32_UINT, 0);

		immediateContext->IASetInputLayout(m->getShader()->getInputLayout());
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		immediateContext->VSSetShader(m->getShader()->getVsShader(), NULL, 0);
		immediateContext->PSSetShader(m->getShader()->getPsShader(), NULL, 0);

		vector<Texture*> textures = m->getTextures();
		Texture *tex = textures[0];
		ID3D11ShaderResourceView* texRes = tex->getTexture();

		immediateContext->PSSetShaderResources(0, 1, &texRes);

		//immediateContext->OMSetDepthStencilState()


		immediateContext->DrawIndexed(g->getIndexCount(), 0, 0);

		swapChain->Present(0, 0);
	}
}



void Render::onReset()
{
	if (renderTargetView != NULL)
		renderTargetView->Release();
	if (depthStencilBuffer != NULL)
		depthStencilBuffer->Release();
	if (depthStencilView != NULL)
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
	UINT m4xMsaaQuality = 0;
	if( true )
	{
		depthStencilDesc.SampleDesc.Count   = 4;
		depthStencilDesc.SampleDesc.Quality = 16;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

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



