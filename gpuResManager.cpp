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
	Ptr<ShaderResource> result;
	if (shaderResMap.count(path) > 0)
	{
		result = shaderResMap[path];
	}
	else
	{
		Ptr<ShaderResource> res = Ptr<ShaderResource>(new ShaderResource(path));
		shaderResMap[path] = res;
		result = res;
	}

	return result;
}

Ptr<MaterialRes> ResManager::createMatRes(string path)
{
	Ptr<MaterialRes> result;
	if (matResMap.count(path) > 0)
	{
		result = matResMap[path];
	}
	else
	{
		Ptr<MaterialRes> res = Ptr<MaterialRes>(new MaterialRes(path));
		matResMap[path] = res;
		result = res;
	}

	return result;
}