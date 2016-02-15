#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

#include <d3d11.h>

class RenderDevice
{
public:
	RenderDevice(){};
	bool init(HWND wnd, int width, int height);
	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;

	~RenderDevice();
};

#endif
