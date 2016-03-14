#ifndef RENDER_H
#define RENDER_H

#include "RenderDevice.h"
#include "renderAble.h"
#include "Camera.h"
#include <map>
//class RenderAble;

class Render
{
public:
	Render() {};
	Render(RenderDevice *device);
	~Render();

private:
	RenderDevice *renderDevice;

public:
	void draw(vector<RenderAble*> renderAbles);
	RenderDevice* Device() { return renderDevice; }
	void onReset();
	Camera * getCamera() { return camera; }

private:
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT screenViewport;
	map<string, ID3D11Buffer*> matrixBufferAry;
	ID3D11SamplerState* sampleState;
	Camera *camera;
	D3D11_RASTERIZER_DESC rasterDesc;
	ID3D11RasterizerState* rasterState;
	D3D11_DEPTH_STENCIL_DESC depthDesc;
	ID3D11DepthStencilState *depthState;
	RenderStates renderState;
};
#endif