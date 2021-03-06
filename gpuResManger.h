#ifndef GPURESMANAGER
#define GPURESMANAGER
#include <D3D11.h>
#include <map>
#include <string>
#include "shaderResource.h"
#include "sharedPtr.h"
#include "materialRes.h"
#include "singleTon.h"
using namespace std;

class ResManager : public Singleton<ResManager>
{
public:
	ResManager();
	~ResManager();

	Ptr<MaterialRes> createMatRes(string path);
	Ptr<EffectRes> createEffectRes(string path);

private:
	map<string, Ptr<EffectRes>> effectResMap;
	map<string, Ptr<MaterialRes>> matResMap;
};
#endif