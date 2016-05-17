#include "Render.h"
#include "MathHelp.h"
#include <comdef.h>

Render::Render(RenderDevice *device)
{
	bufferIndex = 0;
	renderDevice = device;
	renderTargetView = NULL;
	depthStencilBuffer = NULL;
	depthStencilView = NULL;
	sampleState = NULL;
	rasterState = NULL;
	depthState = NULL;
	oneSrcBlenderState = NULL;
	addBlenderState = NULL;
	gpuResManager = new ResManager();

	renderState.blendMode = BlendModes::Replace;
	renderState.cullMode = CullModes::Back;
	renderState.renderMode = RenderModes::Soild;
	renderState.testMode = TestModes::Less;
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


	D3D11_RENDER_TARGET_BLEND_DESC rendertargetBlenderDesc;
	rendertargetBlenderDesc.BlendEnable = true;
	rendertargetBlenderDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rendertargetBlenderDesc.SrcBlend = D3D11_BLEND_ONE;
	rendertargetBlenderDesc.DestBlend = D3D11_BLEND_ONE;
	rendertargetBlenderDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rendertargetBlenderDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rendertargetBlenderDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	rendertargetBlenderDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	D3D11_BLEND_DESC blenderDesc;
	blenderDesc.AlphaToCoverageEnable = false;
	blenderDesc.IndependentBlendEnable = false;
	blenderDesc.RenderTarget[0] = rendertargetBlenderDesc;

	HRESULT hr = renderDevice->d3dDevice->CreateBlendState(&blenderDesc, &addBlenderState);

	rendertargetBlenderDesc.BlendEnable = false;
	rendertargetBlenderDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rendertargetBlenderDesc.SrcBlend = D3D11_BLEND_ONE;
	rendertargetBlenderDesc.DestBlend = D3D11_BLEND_ZERO;
	rendertargetBlenderDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rendertargetBlenderDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rendertargetBlenderDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	rendertargetBlenderDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blenderDesc.AlphaToCoverageEnable = false;
	blenderDesc.IndependentBlendEnable = false;
	blenderDesc.RenderTarget[0] = rendertargetBlenderDesc;

	renderDevice->d3dDevice->CreateBlendState(&blenderDesc, &oneSrcBlenderState);
}

Render::~Render()
{
	delete renderDevice;

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

	if (oneSrcBlenderState != NULL)
		oneSrcBlenderState->Release();

	if (addBlenderState != NULL)
		addBlenderState->Release();

	for (map<string, UpdateBufferCommand*>::iterator it = bufferCommandList.begin();
		it != bufferCommandList.end(); it++)
	{
		delete it->second;
	}

	//delete lightPostEffect;
}

void Render::preDraw()
{
	ID3D11Device* d3dDevice = renderDevice->d3dDevice;
	ID3D11DeviceContext* immediateContext = renderDevice->immediateContext;
	IDXGISwapChain* swapChain = renderDevice->swapChain;

	XMMATRIX vp, view, invPosM, proj;

	vp = camera->ViewProj();

	view = camera->View();
	view._41 = 0.0f;
	view._42 = 0.0f;
	view._43 = 0.0f;
	proj = camera->Proj();
	invPosM = view * proj;
	invPosM = XMMatrixTranspose(invPosM);

	XMFLOAT4X4 data;
	XMStoreFloat4x4(&data, invPosM);
	((UpdateMatrixBufferCommand*)bufferCommandList["invViewPosition"])->updateData(data);

	XMStoreFloat4x4(&data, view);
	((UpdateMatrixBufferCommand*)bufferCommandList["viewMatrix"])->updateData(data);

	XMStoreFloat4x4(&vpData, vp);
	XMStoreFloat4x4(&viewData, view);

	float color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	immediateContext->ClearRenderTargetView(renderTargetView, color);
	immediateContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Render::PostDraw()
{
	renderDevice->swapChain->Present(0, 0);
}

void Render::draw(RenderAble *renderAble, int passIndex)
{
	ID3D11Device* d3dDevice = renderDevice->d3dDevice;
	ID3D11DeviceContext* immediateContext = renderDevice->immediateContext;
	IDXGISwapChain* swapChain = renderDevice->swapChain;

	Geometry *g = renderAble->getGeometry();
	Material *m = renderAble->getMaterial();


	XMMATRIX local = XMLoadFloat4x4(&renderAble->localMatrix);
	XMMATRIX vp = XMLoadFloat4x4(&vpData);
	XMMATRIX matrix = local*vp;
	matrix = XMMatrixTranspose(matrix);

	XMFLOAT4X4 data;
	XMStoreFloat4x4(&data, matrix);
	((UpdateMatrixBufferCommand*)bufferCommandList["gWorldViewProj"])->updateData(data);


	XMMATRIX view = XMLoadFloat4x4(&viewData);
	XMMATRIX modelView = local*view;
	modelView = XMMatrixTranspose(modelView);
	XMStoreFloat4x4(&data, modelView);
	((UpdateMatrixBufferCommand*)bufferCommandList["modelViewMatrix"])->updateData(data);

	vector<ShaderPropery>& properties = m->getShader(passIndex)->getProperties();
	for (int i = 0; i < properties.size(); i++)
	{
		ShaderPropery &p = properties[i];
		UpdateBufferCommand *c = bufferCommandList[p.variableName];
		c->setSlot(p.slot);
		c->update();
		if (p.type == "VS")
			c->bindToVs();
		else if (p.type == "PS")
			c->bindToPs();
	}

	UINT stride = (UINT)Geometry::getVertexSize(g->getVertexType());
	UINT offset = 0;
	ID3D11Buffer *vertexBuffer = g->getVertexBuffer();
	ID3D11Buffer *indexBuffer = g->getIndexBuffer();
	immediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	immediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	immediateContext->IASetInputLayout(m->getShader(passIndex)->getInputLayout());
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Shader *shader = m->getShader(passIndex);
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

	TestModes testMode = shader->getRenderState().testMode;
	if (testMode != renderState.testMode)
	{
		renderState.testMode = testMode;
		if (testMode == TestModes::Always)
			depthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		else if (testMode == TestModes::Less)
			depthDesc.DepthFunc = D3D11_COMPARISON_LESS;

		depthState->Release();
		d3dDevice->CreateDepthStencilState(&depthDesc, &depthState);
		immediateContext->OMSetDepthStencilState(depthState, 0);
	}


	immediateContext->VSSetShader(shader->getVsShader(), NULL, 0);
	immediateContext->PSSetShader(shader->getPsShader(), NULL, 0);

	vector<Texture*> textures = m->getTextures();
	int num = textures.size();
	for (int i = 0; i < textures.size(); i++)
	{
		Texture *tex = textures[i];
		ID3D11ShaderResourceView* texRes = tex->getTexture();
		if (texRes != NULL)
		{
			immediateContext->PSSetShaderResources(i, 1, &texRes);
			immediateContext->PSSetSamplers(i, 1, &sampleState);
		}
	}

	immediateContext->DrawIndexed(g->getIndexCount(), 0, 0);
}


void Render::draw(vector<RenderAble*> renderAbles)
{
	for (int i = 0; i < renderAbles.size(); i++)
	{
		draw(renderAbles[i], 0);
	}
}

void Render::draw(vector<RenderAble*> renderAbles, vector<Light*> &lights)
{
	setMultipleRenderTarget();
	float color[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	for (int i = 0; i < textureRTList.size(); i++)
	{
		renderDevice->immediateContext->ClearRenderTargetView(textureRTList[i]->getTargetView(), color);
	}

	for (int i = 0; i < renderAbles.size(); i++)
	{
		RenderAble *obj = renderAbles[i];
		((UpdateSurfaceBufferCommand*)bufferCommandList["surface"])->updateSurfaceData(obj->getMaterial());
		draw(obj,0);
	}

	setMainRenderTarget();
	//float color1[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	renderDevice->immediateContext->ClearRenderTargetView(renderTargetView, color);
	lightPostEffect->Render();
	//renderDevice->immediateContext->OMSetBlendState(addBlenderState, NULL, 0xffffffff);
	//for (int lightIndex = 0; lightIndex < lights.size(); lightIndex++)
	//{
	//	((UpdateLightBufferCommand*)bufferCommandList["light"])->updateLightData(lights[lightIndex]);
	//	lightPostEffect->Render();
	//}
	//renderDevice->immediateContext->OMSetBlendState(oneSrcBlenderState, NULL, 0xffffffff);

	ID3D11ShaderResourceView*    pSRV[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	renderDevice->immediateContext->PSSetShaderResources(0, 8, pSRV);
}


void Render::drawShadow(vector<RenderAble*> renderAbles, Camera *lightCamera, Camera *mainCamera)
{
	SetCamera(lightCamera);
	preDraw();

	ID3D11RenderTargetView *renderTarget = depthTexture->getTargetView();
	renderDevice->immediateContext->OMSetRenderTargets(1, &renderTarget, NULL);
	for (int i = 0; i < renderAbles.size(); i++)
	{
		draw(renderAbles[i], 0);
	}

	renderDevice->immediateContext->OMSetRenderTargets(1, &renderTargetView, NULL);
	XMMATRIX m1 = lightCamera->ViewProj();
	XMMATRIX m2 = mainCamera->ViewProj();
	XMMATRIX m3 = XMMatrixInverse(&XMMatrixDeterminant(m2), m1);
	XMMATRIX m = m3*m1;
	XMFLOAT4X4 data;
	XMStoreFloat4x4(&data, m);
	((UpdateMatrixBufferCommand*)bufferCommandList["lightWorldProj"])->updateData(data);
	SetCamera(mainCamera);
	preDraw();
	ID3D11ShaderResourceView* texRes = depthTexture->getResView();
	renderDevice->immediateContext->PSSetShaderResources(1, 1, &texRes);

	for (int i = 0; i < renderAbles.size(); i++)
		draw(renderAbles[i], 1);

	ID3D11ShaderResourceView*    pSRV[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	renderDevice->immediateContext->PSSetShaderResources(0, 8, pSRV);
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
	depthStencilDesc.SampleDesc.Count   = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	d3dDevice->CreateTexture2D(&depthStencilDesc, 0, &depthStencilBuffer);
	d3dDevice->CreateDepthStencilView(depthStencilBuffer, 0, &depthStencilView);
	
	setMainRenderTarget();

	// Set the viewport transform.

	screenViewport.TopLeftX = 0;
	screenViewport.TopLeftY = 0;
	screenViewport.Width = static_cast<float>(width);
	screenViewport.Height = static_cast<float>(height);
	screenViewport.MinDepth = 0.0f;
	screenViewport.MaxDepth = 1.0f;

	immediateContext->RSSetViewports(1, &screenViewport);
}


void Render::init()
{
	bufferCommandList["gWorldViewProj"] = new UpdateMatrixBufferCommand;
	bufferCommandList["invViewPosition"] = new UpdateMatrixBufferCommand;
	bufferCommandList["viewMatrix"] = new UpdateMatrixBufferCommand;
	bufferCommandList["normalMatrix"] = new UpdateMatrixBufferCommand; 
	bufferCommandList["modelViewMatrix"] = new UpdateMatrixBufferCommand;
	bufferCommandList["surface"] = new UpdateSurfaceBufferCommand;
	bufferCommandList["light"] = new UpdateLightBufferCommand;
	bufferCommandList["lightWorldProj"] = new UpdateMatrixBufferCommand;

	for (map<string, UpdateBufferCommand*>::iterator it = bufferCommandList.begin();
		it != bufferCommandList.end(); it++)
	{
		it->second->init();
	}

	TextureRenderTarget *target = new TextureRenderTarget;
	target->init(DXGI_FORMAT_R8G8B8A8_UNORM);
	textureRTList.push_back(target);

	target = new TextureRenderTarget;
	target->init(DXGI_FORMAT_R8G8B8A8_UNORM);
	textureRTList.push_back(target);

	target = new TextureRenderTarget;
	target->init(DXGI_FORMAT_R32G32B32A32_FLOAT);
	textureRTList.push_back(target);

	target = new TextureRenderTarget;
	target->init(DXGI_FORMAT_R32G32B32A32_FLOAT);
	textureRTList.push_back(target);

	depthTexture = new TextureRenderTarget;
	depthTexture->init(DXGI_FORMAT_R32G32B32A32_FLOAT);

	//lightPostEffect = new PostEffect("lightPost.material.xml");
	//Material *m = lightPostEffect->getMaterial();
	//for (int i = 0; i < textureRTList.size(); i++)
	//{
	//	TextureRenderTarget *t = textureRTList[i];
	//	m->addTexture(new Texture(t->getResView()));
	//}

	onReset();
}


void Render::setMultipleRenderTarget()
{
	ID3D11RenderTargetView* renderTargets[5];
	renderTargets[0] = renderTargetView;
	for (int i = 1; i < 5; i++)
	{
		renderTargets[i] = textureRTList[i-1]->getTargetView();
	}
	Device()->immediateContext->OMSetRenderTargets(5, renderTargets, depthStencilView);
}

void Render::setMainRenderTarget()
{
	ID3D11RenderTargetView* renderTargets[5] = { renderTargetView , NULL, NULL, NULL, NULL};
	Device()->immediateContext->OMSetRenderTargets(5, renderTargets, depthStencilView);
}



