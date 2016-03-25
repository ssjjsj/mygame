#ifndef MATERIALRES_H
#define MATERIALRES_H

#include "shaderResource.h"
#include "global.h"
#include "MathHelp.h"
#include <map>
#include <string>
#include <memory>
using namespace std;

class MaterialRes
{
public:
	struct MaterialData
	{
		shared_ptr<ShaderResource> res;
		vector<string> texName;
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;

		MaterialData()
		{
			ambient = XMFLOAT3(1.0f, 1.0f, 1.0f);
			specular = XMFLOAT3(1.0f, 1.0f, 1.0f);
			diffuse = XMFLOAT3(1.0f, 1.0f, 1.0f);
		}
	};
public:
	MaterialRes(string resName);
	~MaterialRes();

private:
	void load(string name);
	map<string, MaterialData> dataMap;
public:
	MaterialData& getMaterialData(string name) { return dataMap[name]; }
};

#endif