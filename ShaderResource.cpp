#include "tinyxml\tinyxml.h"
#include "tinyxml\tinystr.h"
#include <cstdio>
//#include <D3D10.h>
#include <D3DX11.h>
#include "global.h"
#include "shaderResource.h"
#include "Render.h"

ShaderResource::ShaderResource(string name)
{
	renderStates.renderMode = RenderModes::Soild;
	renderStates.cullMode = CullModes::None;
	renderStates.blendMode = BlendModes::Replace;
	renderStates.testMode = TestModes::Always;
	renderStates.zWriteMode = ZWrite::On;
	loadShader(ShaderPath + name);
}

ShaderResource::ShaderResource()
{
}

ShaderResource::~ShaderResource()
{
	//delete[] vsShaderCode.data;
	//delete[] psShaderCode.data;
}

void ShaderResource::loadShader(string name)
{
	TiXmlDocument doc = TiXmlDocument(name.c_str());
	doc.LoadFile();

	TiXmlElement *shaderElement = doc.RootElement();
	vsMainFunction = shaderElement->Attribute("vsMainFunction");
	psMainFunction = shaderElement->Attribute("psMainFunction");

	TiXmlNode *propertyRootNode = shaderElement->FirstChild();
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
			if (data == "Back")
				renderStates.cullMode = CullModes::Back;
			else if (data == "Front")
				renderStates.cullMode = CullModes::Front;
		}
		else if (stateType == "BlendMode")
		{
			if (data == "Add")
				renderStates.blendMode = BlendModes::Add;
			else if (data == "AddBlended")
				renderStates.blendMode = BlendModes::AddBlended;
			else if (data == "Blend")
				renderStates.blendMode = BlendModes::Blend;
			else if (data == "Mult")
				renderStates.blendMode = BlendModes::Mult;
		}
		else if (stateType == "TestMode")
		{
			if (data == "Equal")
				renderStates.testMode = TestModes::Equal;
			else if (data == "Greater")
				renderStates.testMode = TestModes::Greater;
			else if (data == "GreaterEqual")
				renderStates.testMode = TestModes::GreaterEqual;
			else if (data == "Less")
				renderStates.testMode = TestModes::Less;
			else if (data == "LessEqual")
				renderStates.testMode = TestModes::LessEqual;
		}
		else if (stateType == "RenderModes")
		{
			if (data == "Wire")
				renderStates.renderMode = RenderModes::Wire;
		}
		else if (stateType == "ZWrite")
		{
			if (data == "Off")
			{
				renderStates.zWriteMode = ZWrite::Off;
			}
		}
	}

	TiXmlElement *vsProgramElement = (TiXmlElement*)stateRootNode->NextSibling();
	string tVsSourceCode = vsProgramElement->Attribute("program");

	FILE *fp = fopen("temp.hlsl", "w");
	fprintf(fp, "%s", tVsSourceCode.c_str());
	fclose(fp);

	ID3D10Blob* errorMessage = NULL;
	ID3D10Blob* shaderBuffer;
	HRESULT result = D3DX11CompileFromFile(L"temp.hlsl", NULL, NULL, vsMainFunction.c_str(), "vs_5_0", 0, 0, NULL,
		&shaderBuffer, &errorMessage, NULL);

	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (errorMessage != NULL)
	{
		char *compileErrors = (char*)(errorMessage->GetBufferPointer());
		string error = string(compileErrors, errorMessage->GetBufferSize());
		errorMessage->Release();
	}


	ID3D11VertexShader *shader;
	HRESULT hr = gRender->Device()->d3dDevice->CreateInputLayout(vertexDesc, 3, shaderBuffer->GetBufferPointer(),
		shaderBuffer->GetBufferSize(), &inputLayout);


	vsShaderCode.length = shaderBuffer->GetBufferSize();
	vsShaderCode.data = new char[vsShaderCode.length];
	memcpy(vsShaderCode.data, shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize());
	shaderBuffer->Release();

	
	TiXmlElement *psProgramElement = (TiXmlElement*)vsProgramElement->NextSibling();
	string tPsSourceCode = psProgramElement->Attribute("program");

	fp = fopen("temp.hlsl", "w");
	fprintf(fp, "%s", tPsSourceCode.c_str());
	fclose(fp);
	

	errorMessage = NULL;
	result = D3DX11CompileFromFile(L"temp.hlsl", NULL, NULL, psMainFunction.c_str(), "ps_5_0", 0, 0, NULL,
		&shaderBuffer, &errorMessage, NULL);

	if (errorMessage != NULL)
	{
		char *compileErrors = (char*)(errorMessage->GetBufferPointer());
		string error = string(compileErrors, errorMessage->GetBufferSize());
		errorMessage->Release();
	}

	psShaderCode.length = shaderBuffer->GetBufferSize();
	psShaderCode.data = new char[psShaderCode.length];
	memcpy(psShaderCode.data, shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize());
	shaderBuffer->Release();
}

