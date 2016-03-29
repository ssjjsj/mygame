#ifndef RENDER_H
#define RENDER_H

#include "RenderDevice.h"
#include "renderAble.h"
#include "Camera.h"
#include "gpuResManger.h"
#include "light.h"
#include <map>
//class RenderAble;

class Render
{
	struct SurfaceData
	{
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;
		int pad1, pad2, pad3;
	};

	struct LightData
	{
		XMFLOAT3 pos;
		//XMFLOAT3 direction;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;
		float k0, k1, k2;
		int pad;
	};
public:
	Render() {};
	Render(RenderDevice *device);
	~Render();

private:
	RenderDevice *renderDevice;
	int bufferIndex;

public:
	void draw(vector<RenderAble*> renderAbles, vector<Light*> &lights);
	void preDraw();
	RenderDevice* Device() { return renderDevice; }
	void onReset();
	Camera * getCamera() { return camera; }
	GpuResManager *gpuResManager;

	void setSurfaceData(Material *m);
	void setLightData(Light *l);

private:
	ID3D11Texture2D* depthStencilBuffer;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT screenViewport;
	map<string, ID3D11Buffer*> matrixBufferAry;
	ID3D11Buffer *lightBuff;
	ID3D11Buffer *materialBuffer;
	ID3D11SamplerState* sampleState;
	Camera *camera;
	D3D11_RASTERIZER_DESC rasterDesc;
	ID3D11RasterizerState* rasterState;
	D3D11_DEPTH_STENCIL_DESC depthDesc;
	ID3D11DepthStencilState *depthState;
	RenderStates renderState;
};
#endif