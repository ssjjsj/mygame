#include "RenderDevice.h"
#include <D3D11.h>

bool RenderDevice::init(HWND wnd, int width, int height)
{
	// Create the device and device context.

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0,                 // no software device
		createDeviceFlags,
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&d3dDevice,
		&featureLevel,
		&immediateContext);

	if (FAILED(hr))
	{
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		return false;
	}

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	//HR(d3dDevice->CheckMultisampleQualityLevels(
	//	DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality));
	//assert(m4xMsaaQuality > 0);

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	//if (mEnable4xMsaa)
	//{
	//	sd.SampleDesc.Count = 4;
	//	sd.SampleDesc.Quality = m4xMsaaQuality - 1;
	//}
	// No MSAA
	//else
	//{
	//	sd.SampleDesc.Count = 1;
	//	sd.SampleDesc.Quality = 0;
	//}

	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = wnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	if (FAILED(d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice)))
		return false;

	IDXGIAdapter* dxgiAdapter = 0;
	if (FAILED((dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter))))
		return false;

	IDXGIFactory* dxgiFactory = 0;
	if (FAILED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory)))
		return false;

	if (FAILED(dxgiFactory->CreateSwapChain(d3dDevice, &sd, &swapChain)))
		return false;

	dxgiDevice->Release();
	dxgiAdapter->Release();
	dxgiFactory->Release();

	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the OnResize method here to avoid code duplication.

	//OnResize();

	return true;
}


RenderDevice::~RenderDevice()
{
	if (d3dDevice)
		d3dDevice->Release();

	if (immediateContext)
	{
		immediateContext->ClearState();
		immediateContext->Release();
	}

	if (swapChain)
	{
		swapChain->Release();
	}
}


RenderDevice* RenderDevice::Instance()
{
	if (instance == NULL)
	{
		instance = new RenderDevice;
	}

	return instance;
}