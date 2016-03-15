#include "Render.h"
#include "MathHelp.h"

Render::Render(RenderDevice *device)
{
	renderDevice = device;
	renderTargetView = NULL;
	depthStencilBuffer = NULL;
	depthStencilView = NULL;
	sampleState = NULL;
	rasterState = NULL;
	depthState = NULL;
	camera = new Camera;

	renderState.blendMode = BlendModes::Replace;
	renderState.cullMode = CullModes::Back;
	renderState.renderMode = RenderModes::Soild;
	renderState.testMode = TestModes::Always;
	renderState.zWriteMode = ZWrite::On;

	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	renderDevice->d3dDevice->CreateRasterizerState(&rasterDesc, &rasterState);


	ZeroMemory(&depthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthDesc.DepthEnable = true;
	depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDesc.StencilEnable = false;
	depthDesc.StencilReadMask = 0xff;
	depthDesc.StencilWriteMask = 0xff;

	renderDevice->d3dDevice->CreateDepthStencilState(&depthDesc, &depthState);


	D3D11_BUFFER_DESC matrixBufferDesc;
	ZeroMemory(&matrixBufferDesc, sizeof(D3D11_BUFFER_DESC));
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(XMFLOAT4X4);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	ID3D11Buffer *matrixBuffer;
	ID3D11Buffer *invPosMatrixBuffer;
	renderDevice->d3dDevice->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
	renderDevice->d3dDevice->CreateBuffer(&matrixBufferDesc, NULL, &invPosMatrixBuffer);

	matrixBufferAry["gWorldViewProj"] = matrixBuffer;
	matrixBufferAry["invViewPosition"] = invPosMatrixBuffer;

	
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	renderDevice->d3dDevice->CreateSamplerState(&samplerDesc, &sampleState);

	onReset();
}

Render::~Render()
{
	delete renderDevice;

	for (map<string, ID3D11Buffer*>::iterator it = matrixBufferAry.begin(); it != matrixBufferAry.end(); it++)
	{
		it->second->Release();
	}

	if (rasterState != NULL)
		rasterState->Release();

	if (sampleState != NULL)
		sampleState->Release();


	if (renderTargetView != NULL)
		renderTargetView->Release();


	if (depthStencilBuffer != NULL)
		depthStencilBuffer->Release();


	if (sampleState != NULL)
		depthStencilView->Release();

	if (depthState != NULL)
		depthState->Release();
}


void Render::draw(vector<RenderAble*> renderAbles)
{
	ID3D11Device* d3dDevice = renderDevice->d3dDevice;
	ID3D11DeviceContext* immediateContext = renderDevice->immediateContext;
	IDXGISwapChain* swapChain = renderDevice->swapChain;

	XMMATRIX vp = camera->ViewProj();
	
	XMMATRIX view = camera->View();
	view._41 = 0.0f;
	view._42 = 0.0f;
	view._43 = 0.0f;
	XMMATRIX proj = camera->Proj();
	XMMATRIX invPosM = view * proj;
	invPosM = XMMatrixTranspose(invPosM);

	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	immediateContext->ClearRenderTargetView(renderTargetView, color);
	immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (int i = 0; i < renderAbles.size(); i++)
	{
		RenderAble *renderAble = renderAbles[i];

		Geometry *g = renderAble->getGeometry();
		Material *m = renderAble->getMaterial();


		XMMATRIX local = XMLoadFloat4x4(&renderAble->localMatrix);
		XMMATRIX matrix = local*vp;
		matrix = XMMatrixTranspose(matrix);
		
		UINT stride = (UINT)Geometry::getVertexSize(g->getVertexType());
		UINT offset = 0;
		ID3D11Buffer *vertexBuffer = g->getVertexBuffer();
		ID3D11Buffer *indexBuffer = g->getIndexBuffer();
		immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		immediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		immediateContext->IASetInputLayout(m->getShader()->getInputLayout());
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Shader *shader = m->getShader();
		RenderModes renderMode = shader->getRenderState().renderMode;
		if (renderMode != renderState.renderMode)
		{
			renderState.renderMode = renderMode;
			if (renderMode == RenderModes::Soild)
				rasterDesc.FillMode = D3D11_FILL_SOLID;
			else if (renderMode == RenderModes::Wire)
				rasterDesc.FillMode = D3D11_FILL_WIREFRAME;

			rasterState->Release();
			d3dDevice->CreateRasterizerState(&rasterDesc, &rasterState);
			immediateContext->RSSetState(rasterState);
		}

		ZWrite zMode = shader->getRenderState().zWriteMode;
		if (zMode != renderState.zWriteMode)
		{
			if (zMode == ZWrite::Off)
			{
				depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			}
			else
			{
				depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			}
			renderState.zWriteMode = zMode;

			depthState->Release();
			d3dDevice->CreateDepthStencilState(&depthDesc, &depthState);
			immediateContext->OMSetDepthStencilState(depthState, 0);
		}

		CullModes cullMode = shader->getRenderState().cullMode;
		if (cullMode != renderState.cullMode)
		{
			renderState.cullMode = cullMode;
			if (cullMode == CullModes::Front)
				rasterDesc.CullMode = D3D11_CULL_FRONT;
			else if (cullMode == CullModes::Back)
				rasterDesc.CullMode = D3D11_CULL_BACK;

			rasterState->Release();
			d3dDevice->CreateRasterizerState(&rasterDesc, &rasterState);
			immediateContext->RSSetState(rasterState);
		}


		immediateContext->VSSetShader(shader->getVsShader(), NULL, 0);
		immediateContext->PSSetShader(shader->getPsShader(), NULL, 0);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (shader->constainProperty("gWorldViewProj", ShaderPropery::PropertyType::Matrix))
		{
			ID3D11Buffer *matrixBuffer = matrixBufferAry["gWorldViewProj"];
			immediateContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			XMFLOAT4X4* dataPtr = (XMFLOAT4X4*)mappedResource.pData;
			XMFLOAT4X4 data;
			XMStoreFloat4x4(&data, matrix);
			memcpy(dataPtr, &data, sizeof(XMFLOAT4X4));
			immediateContext->Unmap(matrixBuffer, 0);
			immediateContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
		}

		if (shader->constainProperty("invViewPosition", ShaderPropery::PropertyType::Matrix))
		{
			ID3D11Buffer *invPosMatrixBuffer = matrixBufferAry["invViewPosition"];
			immediateContext->Map(invPosMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			XMFLOAT4X4* dataPtr = (XMFLOAT4X4*)mappedResource.pData;
			XMFLOAT4X4 data;
			XMStoreFloat4x4(&data, invPosM);
			memcpy(dataPtr, &data, sizeof(XMFLOAT4X4));
			immediateContext->Unmap(invPosMatrixBuffer, 0);
			immediateContext->VSSetConstantBuffers(0, 1, &invPosMatrixBuffer);
		}


		vector<Texture*> textures = m->getTextures();
		int num = textures.size();
		for (int i = 0; i < textures.size(); i++)
		{
			Texture *tex = textures[i];
			ID3D11ShaderResourceView* texRes = tex->getTexture();

			immediateContext->PSSetShaderResources(i, 1, &texRes);
			immediateContext->PSSetSamplers(i, 1, &sampleState);
		}

		//immediateContext->OMSetDepthStencilState()


		immediateContext->DrawIndexed(g->getIndexCount(), 0, 0);
	}

	renderDevice->swapChain->Present(0, 0);
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
	HRESULT hr = d3dDevice->CreateRenderTargetView(backBuffer, 0, &renderTargetView);
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

	camera->SetLens(3.14, (float)width / (float)height, 1.0f, 1000.0f);

	immediateContext->RSSetViewports(1, &screenViewport);
}



