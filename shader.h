#ifndef SHADER_H
#define SHADER_H

#include "global.h"
#include "Render.h"
#include "shaderResource.h"

class Shader
{
public:
	Shader(ShaderResource res);
private:
	//bool compileShader(string sourceCode);
	bool createShader();

public:
	ID3D11VertexShader *getVsShader() {return vsShader;}
	ID3D11PixelShader *getPsShader() { return psShader; }
private:
	ShaderResource shaderRes;
	ID3D11VertexShader *vsShader;
	ID3D11PixelShader *psShader;
};



#endif // !SHADER_H
