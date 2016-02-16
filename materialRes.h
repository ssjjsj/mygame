#ifndef MATERIALRES_H
#define MATERIALRES_H

#include "shaderResource.h"
#include "global.h"

class MaterialRes
{
public:
	MaterialRes(string resName);
	~MaterialRes();

private:
	ShaderResource *res;
	void load();

public:
	ShaderResource *getShaderRes() { return res; }
};
#endif