#ifndef SHADER_H
#define SHADER_H

#include "global.h"
#include "shaderResource.h"
#include <d3d11.h>

class Shader
{
public:
	Shader(ShaderResource *res);
	~Shader();
private:
	//bool compileShader(string sourceCode);
	bool createShader(ShaderResource *res);

public:
	ID3D11VertexShader *getVsShader() {return vsShader;}
	ID3D11PixelShader *getPsShader() { return psShader; }
	ID3D11InputLayout* getInputLayout() { return inputLayout; }
	void setInputLayout(ID3D11InputLayout* layout) { inputLayout = layout; }
	RenderStates getRenderState() { return states; }
private:
	ID3D11VertexShader *vsShader;
	ID3D11PixelShader *psShader;
	ID3D11InputLayout* inputLayout;
	RenderStates states;
};



#endif // !SHADER_H
