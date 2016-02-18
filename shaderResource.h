#ifndef SHADERRESOURCE_H
#define SHADERRESOURCE_H
using namespace std;
#include <string>
#include <vector>
#include <D3D11.h>

struct ShaderPropery
{
	enum PropertyType
	{
		Matrix = 0,
		Vector4 = 1,
		Color = 2,
		Float = 3,
		Texture = 4,
	};

	PropertyType type;
	string variableName;

	static PropertyType getType(string type)
	{
		if (type == "Matrix")
			return  PropertyType::Matrix;
		else if (type == "Vector4")
			return  PropertyType::Vector4;
		else if (type == "Color")
			return  PropertyType::Color;
		else if (type == "Float")
			return  PropertyType::Float;
	}
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


struct SourceCode
{
	SIZE_T length;
	char* data;
};

class ShaderResource
{
public:
	ShaderResource(string name);
	ShaderResource(){};
	~ShaderResource(){};
	void load(string name);
private:
	CullModes cullMode;
	TestModes testMode;
	BlendModes blendMode;
	vector<ShaderPropery> properties;
	SourceCode vsShaderCode;
	SourceCode psShaderCode;
	string vsMainFunction;
	string psMainFunction;

	string shaderName;

public:
	string getShaderName() { return shaderName; }
	string getPsMainFunction() { return psMainFunction; }
	string getVsMainFunction() { return vsMainFunction; }
	SourceCode& getPsShaderCode() { return psShaderCode; }
	SourceCode& getVsShaderCode() { return vsShaderCode; }
	vector<ShaderPropery>& getProperties() { return properties; }
	BlendModes getBlendMode() { return blendMode; }
	TestModes getTestMode() { return testMode; }
	CullModes getCullMode() { return cullMode; }
};

#endif