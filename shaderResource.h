#ifndef SHADERRESOURCE_H
#define SHADERRESOURCE_H
using namespace std;
#include <string>
#include <vector>

struct ShaderPropery
{
	enum List
	{
		PTMatrix = 0,
		PTVector4 = 1,
		PTColor = 2,
		PTFloat = 3,
	};

	float *data;
	string texName;
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

enum CullModes
{
	Back,
	Front,
	None
};

class ShaderResource
{
public:
	void load(string name);
private:
	CullModes cullMode;
	TestModes testMode;
	BlendModes blendMode;
	vector<ShaderPropery> properties;
	string vsShaderCode;
	string psShaderCode;
	string vsMainFunction;
	string psMainFunction;

	string shaderName;

public:
	string getShaderName() { return shaderName; }
	string getPsMainFunction() { return psMainFunction; }
	string getVsMainFunction() { return vsMainFunction; }
	string getPsShaderCode() { return psShaderCode; }
	string getVsShaderCode() { return vsShaderCode; }
	vector<ShaderPropery>& getProperties() { return properties; }
	BlendModes getBlendMode() { return blendMode; }
	TestModes getTestMode() { return testMode; }
	CullModes getCullMode() { return cullMode; }
};

#endif