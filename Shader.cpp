#include "shader.h"
#include "Render.h"


Shader::Shader(ShaderResource *res)
{
	createShader(res);
}


Shader::~Shader()
{
}


bool Shader::createShader(ShaderResource *res)
{
	isLighted = res->IsLighted();
	states = res->getRenderStates();
	properties = res->getProperties();
	inputLayout = res->getInputLayout();


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

	return !FAILED(hr);
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