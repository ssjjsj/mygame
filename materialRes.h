#ifndef MATERIALRES_H
#define MATERIALRES_H

#include "shaderResource.h"
#include "global.h"
#include <map>
using namespace std;

class MaterialRes
{
public:
	struct MaterialData
	{
		ShaderResource *res;
		string texName;
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