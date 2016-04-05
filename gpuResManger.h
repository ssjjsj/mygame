#ifndef GPURESMANAGER
#define GPURESMANAGER
#include <D3D11.h>
#include <map>
#include <string>
#include "shader.h"
using namespace std;

class GpuResManager
{
public:
	GpuResManager();
	~GpuResManager();

	map<string, ID3D11VertexShader*> vsShaderMap;
	map<string, ID3D11PixelShader*> psShaderMap;
	map<string, ID3D11ShaderResourceView*> textureMap;
	Shader *lightShader;
	void createLightShader();

};
#endif