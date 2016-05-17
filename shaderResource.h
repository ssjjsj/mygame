#ifndef SHADERRESOURCE_H
#define SHADERRESOURCE_H
using namespace std;
#include <string>
#include <vector>
#include <map>
#include <D3D11.h>
#include "RefCount.h"
#include "tinyxml\tinyxml.h"
#include "tinyxml\tinystr.h"

class  Shader;

struct ShaderPropery
{
	enum PropertyType
	{
		VS = 0,
		PS = 1,
	};

	string type;
	string variableName;
	int slot;
};


enum RenderModes
{
	Wire = 0,
	Soild,
};



enum BlendModes
{
	Replace,
	Blend,
	Add,
	AddBlended,
	Mult
};


enum TestModes
{
	LessEqual,
	Less,
	Equal,
	Greater,
	GreaterEqual,
	Always
};

enum ZWrite
{
	On,
	Off,
};

enum CullModes
{
	Back,
	Front,
	None
};


struct SourceCode
{
	SIZE_T length;
	char* data;
};

struct RenderStates
{
	CullModes cullMode;
	TestModes testMode;
	RenderModes renderMode;
	BlendModes blendMode;
	ZWrite zWriteMode;
};

class ShaderResource : public RefCount
{
public:
	ShaderResource();
	~ShaderResource();
private:
	vector<ShaderPropery> properties;
	SourceCode vsShaderCode;
	SourceCode psShaderCode;
	string vsMainFunction;
	string psMainFunction;
	RenderStates renderStates;
	bool isLightOn;

	ID3D11InputLayout* inputLayout;

	string shaderName;
	Shader *shaderInstance;

public:
	string getShaderName() { return shaderName; }
	string getPsMainFunction() { return psMainFunction; }
	string getVsMainFunction() { return vsMainFunction; }
	SourceCode& getPsShaderCode() { return psShaderCode; }
	SourceCode& getVsShaderCode() { return vsShaderCode; }
	vector<ShaderPropery>& getProperties() { return properties; }
	RenderStates getRenderStates() { return renderStates; }
	ID3D11InputLayout* getInputLayout() { return inputLayout; }
	bool IsLighted() { return isLightOn; }
	void loadShader(TiXmlElement *shaderElement);
	Shader *createInstance();
};

#endif