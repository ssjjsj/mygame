#include "shader.h"
#include "Render.h"


Shader::Shader(ShaderResource *res)
{
	createShader(res);
}


Shader::~Shader()
{
	if (vsShader != NULL)
		vsShader->Release();

	if (psShader != NULL)
		psShader->Release();
}


bool Shader::createShader(ShaderResource *res)
{
	states = res->getRenderStates();
	properties = res->getProperties();
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

	inputLayout = res->getInputLayout();

	return !FAILED(hr);
}


bool Shader::constainProperty(string tProperty, ShaderPropery::PropertyType t)
{
	for (int i = 0; i < properties.size(); i++)
	{
		ShaderPropery &p = properties[i];
		if (p.variableName == tProperty && p.type == t)
		{
			return true;
		}
	}

	return false;
}