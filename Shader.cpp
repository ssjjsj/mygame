#include "shader.h"


Shader::Shader(ShaderResource res)
{
	shaderRes = res;
	createShader();
}


bool Shader::createShader()
{
	SourceCode& sourceCode = shaderRes.getVsShaderCode();

	HRESULT hr = gRender->Device()->d3dDevice->CreateVertexShader(
		sourceCode.data,
		sourceCode.length,
		nullptr,
		&vsShader
		);

	if (FAILED(hr))
		return false;

	sourceCode = shaderRes.getVsShaderCode();
	hr = gRender->Device()->d3dDevice->CreatePixelShader(
		sourceCode.data,
		sourceCode.length,
		nullptr,
		&psShader
		);

	return !FAILED(hr);
}