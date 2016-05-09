#include "gpuResManger.h"
#include "shaderResource.h"


ResManager::ResManager()
{
}

ResManager::~ResManager()
{
}

Ptr<ShaderResource> ResManager::createShaderRes(string path)
{
	if (shaderResMap.count(path) > 0)
		return shaderResMap[path];
	else
	{
		Ptr<ShaderResource> res = Ptr<ShaderResource>(new ShaderResource(path));
		shaderResMap[path] = res;
	}
}

Ptr<MaterialRes> ResManager::createMatRes(string path)
{
	if (matResMap.count(path) > 0)
	{
		return matResMap[path];
	}
	else
	{
		Ptr<MaterialRes> res = Ptr<MaterialRes>(new MaterialRes(path));
		matResMap[path] = res;
	}
}