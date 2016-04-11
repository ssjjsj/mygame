#ifndef RENDER_H
#define RENDER_H

#include "RenderDevice.h"
#include "renderAble.h"
#include "Camera.h"
#include "gpuResManger.h"
#include "light.h"
#include <map>
#include <string>
#include "textureRenderTarget.h"
#include "updateBufferCommand.h"
#include "postEffect.h"
//class RenderAble;

class Render
{
public:
	Render() {};
	Render(RenderDevice *device);
	~Render();

private:
	RenderDevice *renderDevice;
	int bufferIndex;

public:
	void init();
	void draw(vector<RenderAble*> renderAbles, vector<Light*> &lights);
	void draw(RenderAble *obj);
	void draw(vector<RenderAble*> renderAbles);
	void preDraw();
	void PostDraw();
	RenderDevice* Device() { return renderDevice; }
	void onReset();
	Camera * getCamera() { return camera; }
	GpuResManager *gpuResManager;

private:
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT screenViewport;
	ID3D11SamplerState* sampleState;
	Camera *camera;
	D3D11_RASTERIZER_DESC rasterDesc;
	ID3D11RasterizerState* rasterState;
	D3D11_DEPTH_STENCIL_DESC depthDesc;
	ID3D11DepthStencilState *depthState;
	RenderStates renderState;
	ID3D11BlendState *addBlenderState;
	ID3D11BlendState *oneSrcBlenderState;
	vector<TextureRenderTarget*> textureRTList;
	XMMATRIX view;
	XMMATRIX vp;
	XMMATRIX proj;
	XMMATRIX invPosM;
	map<string, UpdateBufferCommand*> bufferCommandList;
	PostEffect *lightPostEffect;
};
#endif