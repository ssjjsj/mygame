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
		int pad1;
		XMFLOAT3 diffuse;
		int pad2;
		XMFLOAT3 specular;
		int pad3;
	};

	struct LightData
	{
		XMFLOAT3 pos;
		int pad1;
		//XMFLOAT3 direction;
		XMFLOAT3 ambient;
		int pad2;
		XMFLOAT3 diffuse;
		int pad3;
		XMFLOAT3 specular;
		int pad4;
		XMFLOAT3 k;
		int pad5;
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

	void setSurfaceData(Material *m, int slot);
	void setLightData(Light *l, int slot);
	void setMatrixData(string name, int slot, XMFLOAT4X4 matrix);

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
	ID3D11BlendState *addBlenderState;
	ID3D11BlendState *oneSrcBlenderState;
};
#endif