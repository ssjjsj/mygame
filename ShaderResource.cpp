#include "shaderResource.h"
#include "tinyxml\tinyxml.h"
#include "tinyxml\tinystr.h"
#include <cstdio>
#include <D3D10.h>
#include <D3DX11.h>
#include "global.h"

ShaderResource::ShaderResource(string name)
{
	load(ShaderPath + name);
}

void ShaderResource::load(string name)
{
	TiXmlDocument doc = TiXmlDocument(name.c_str());
	doc.LoadFile();

	TiXmlElement *shaderElement = doc.RootElement;
	vsMainFunction = shaderElement->Attribute("vsMainFunction");
	psMainFunction = shaderElement->Attribute("psMainFunction");

	TiXmlNode *propertyRootNode = shaderElement->NextSibling();
	for (TiXmlNode *curPropertyNode = propertyRootNode->FirstChild(); curPropertyNode != NULL; curPropertyNode = curPropertyNode->NextSibling())
	{
		TiXmlElement *propertyElement = (TiXmlElement*)curPropertyNode;
		ShaderPropery p;
		string sType = propertyElement->Attribute("type");
		p.type = ShaderPropery::getType(sType);
		p.variableName = propertyElement->Attribute("name");

		properties.push_back(p);
	}

	TiXmlNode *stateRootNode = propertyRootNode->NextSibling();
	for (TiXmlNode *curStateNode = stateRootNode->FirstChild(); curStateNode != NULL; curStateNode = curStateNode->NextSibling())
	{
		TiXmlElement *stateElement = (TiXmlElement*)curStateNode;
		string stateType = stateElement->Attribute("type");
		string data = stateElement->Attribute("data");

		if (stateType == "CullMode")
		{
			cullMode = CullModes::None;
			if (data == "Back")
				cullMode = CullModes::Back;
			else if (data == "Front")
				cullMode = CullModes::Front;
		}
		else if (stateType == "BlendMode")
		{
			blendMode = BlendModes::Replace;
			if (data == "Add")
				blendMode = BlendModes::Add;
			else if (data == "AddBlended")
				blendMode = BlendModes::AddBlended;
			else if (data == "Blend")
				blendMode = BlendModes::Blend;
			else if (data == "Mult")
				blendMode = BlendModes::Mult;
		}
		else if (stateType == "TestMode")
		{
			testMode = TestModes::Always;
			if (data == "Equal")
				testMode = TestModes::Equal;
			else if (data == "Greater")
				testMode = TestModes::Greater;
			else if (data == "GreaterEqual")
				testMode = TestModes::GreaterEqual;
			else if (data == "Less")
				testMode = TestModes::Less;
			else if (data == "LessEqual")
				testMode = TestModes::LessEqual;
		}
	}

	TiXmlElement *vsProgramElement = (TiXmlElement*)stateRootNode->NextSibling();
	string tVsSourceCode = vsProgramElement->Attribute("program");

	FILE *fp = fopen("temp.hlsl", "w");
	fprintf(fp, "%s", tVsSourceCode.c_str());
	fclose(fp);

	ID3D10Blob* shaderBuffer;
	HRESULT result = D3DX11CompileFromFile(L"temp.hlsl", NULL, NULL, vsMainFunction.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&shaderBuffer, NULL, NULL);

	vsShaderCode.length = shaderBuffer->GetBufferSize();
	vsShaderCode.data = new char[vsShaderCode.length];
	memcpy(vsShaderCode.data, shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize());
	shaderBuffer->Release();

	
	TiXmlElement *psProgramElement = (TiXmlElement*)vsProgramElement->NextSibling();
	string tVsSourceCode = psProgramElement->Attribute("program");

	fp = fopen("temp.hlsl", "w");
	fprintf(fp, "%s", tVsSourceCode.c_str());
	fclose(fp);

	HRESULT result = D3DX11CompileFromFile(L"temp.hlsl", NULL, NULL, psMainFunction.c_str(), "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL,
		&shaderBuffer, NULL, NULL);

	psShaderCode.length = shaderBuffer->GetBufferSize();
	psShaderCode.data = new char[vsShaderCode.length];
	memcpy(psShaderCode.data, shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize());
	shaderBuffer->Release();
}

