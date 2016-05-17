#ifndef EFFECTRES_H
#define EFFECTRES_H
#include "shaderResource.h"
#include "RefCount.h"
class EffectRes : public RefCount 
{
	struct Pass
	{
		string shaderName;
	};
public:
	EffectRes(string path);
	EffectRes();
	~EffectRes();

public:
	int getShaderResCount() { return shaderRes.size(); }
	ShaderResource* getShaderRes(int i) { return shaderRes[i]; }

private:
	void createEffect(string path);
private:
	vector<ShaderResource*> shaderRes;
};
#endif // !EFFECT_H
