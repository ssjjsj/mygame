#ifndef SHADERRESOURCE_H
#define SHADERRESOURCE_H
using namespace std;
#include <string>
#include <vector>
#include <map>
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
};

class ShaderResource
{
public:
	ShaderResource(string name);
	ShaderResource();
	~ShaderResource();
private:
	vector<ShaderPropery> properties;
	SourceCode vsShaderCode;
	SourceCode psShaderCode;
	string vsMainFunction;
	string psMainFunction;
	RenderStates renderStates;

	ID3D11InputLayout* inputLayout;

	string shaderName;
	void loadShader(string name);

public:
	string getShaderName() { return shaderName; }
	string getPsMainFunction() { return psMainFunction; }
	string getVsMainFunction() { return vsMainFunction; }
	SourceCode& getPsShaderCode() { return psShaderCode; }
	SourceCode& getVsShaderCode() { return vsShaderCode; }
	vector<ShaderPropery>& getProperties() { return properties; }
	RenderStates getRenderStates() { return renderStates; }
	ID3D11InputLayout* getInputLayout() { return inputLayout; }
};

#endif