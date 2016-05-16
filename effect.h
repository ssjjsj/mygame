#ifndef EFFECTRES_H
#define EFFECTRES_H
#include "shaderResource.h"
#include "RefCount.h"
class EffectRes : RefCount 
{
public:
	EffectRes(string path);
	EffectRes();
	~EffectRes();
private:
	vector<ShaderResource*> shaderRes;
};
#endif // !EFFECT_H
