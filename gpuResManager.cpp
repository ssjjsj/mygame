#include "gpuResManger.h"
#include "shaderResource.h"

ResManager::ResManager()
{
}

ResManager::~ResManager()
{
}

Ptr<EffectRes> ResManager::createEffectRes(string path)
{
	Ptr<EffectRes> result;
	if (effectResMap.count(path) > 0)
	{
		result = effectResMap[path];
	}
	else
	{
		Ptr<EffectRes> res = Ptr<EffectRes>(new EffectRes(path));
		effectResMap[path] = res;
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