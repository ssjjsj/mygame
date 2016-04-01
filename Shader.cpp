#include "shader.h"
#include "Render.h"


Shader::Shader(ShaderResource *res)
{
	createShader(res);
}


Shader::~Shader()
{
	//if (vsShader != NULL)
	//	vsShader->Release();

	//if (psShader != NULL)
	//	psShader->Release();
}


bool Shader::createShader(ShaderResource *res)
{
	isLighted = res->IsLighted();
	states = res->getRenderStates();
	properties = res->getProperties();
	inputLayout = res->getInputLayout();

	if (gRender->gpuResManager->psShaderMap.count(res->getShaderName()) > 0)
	{
		vsShader = gRender->gpuResManager->vsShaderMap[res->getShaderName()];
		psShader = gRender->gpuResManager->psShaderMap[res->getShaderName()];
	}
	else
	{
		SourceCode& sourceCode = res->getVsShaderCode();

		HRESULT hr = gRender->Device()->d3dDevice->CreateVertexShader(
			sourceCode.data,
			sourceCode.length,
			nullptr,
			&vsShader
			);

		if (FAILED(hr))
			return false;

		sourceCode = res->getPsShaderCode();
		hr = gRender->Device()->d3dDevice->CreatePixelShader(
			sourceCode.data,
			sourceCode.length,
			nullptr,
			&psShader
			);

		gRender->gpuResManager->vsShaderMap[res->getShaderName()] = vsShader;
		gRender->gpuResManager->psShaderMap[res->getShaderName()] = psShader;

		return !FAILED(hr);
	}
}


bool Shader::constainProperty(string tProperty)
{
	for (int i = 0; i < properties.size(); i++)
	{
		ShaderPropery &p = properties[i];
		if (p.variableName == tProperty)
		{
			return true;
		}
	}

	return false;
}


int Shader::getPropertySlot(string tProperty)
{
	for (int i = 0; i < properties.size(); i++)
	{
		ShaderPropery &p = properties[i];
		if (p.variableName == tProperty)
		{
			return p.slot;
		}
	}

	return -1;
}