#include "gpuResManger.h"
#include "shaderResource.h"


GpuResManager::GpuResManager()
{
}

GpuResManager::~GpuResManager()
{
	for (map<string, ID3D11VertexShader*>::iterator it = vsShaderMap.begin(); it != vsShaderMap.end(); it++)
	{
		it->second->Release();
	}

	for (map<string, ID3D11PixelShader*>::iterator it = psShaderMap.begin(); it != psShaderMap.end(); it++)
	{
		it->second->Release();
	}

	for (map<string, ID3D11ShaderResourceView*>::iterator it = textureMap.begin(); it != textureMap.end(); it++)
	{
		it->second->Release();
	}
}


void GpuResManager::createLightShader()
{
	ShaderResource *res = new ShaderResource("light.shader.xml");
	lightShader = new Shader(res);
}