#ifndef RENDER_H
#define RENDER_H

#include "RenderDevice.h"
#include "renderAble.h"
#include "Camera.h"
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

private:
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT screenViewport;
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	Camera *camera;
};
#endif