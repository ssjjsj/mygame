#include "postEffect.h"
#include "global.h"
#include "Render.h"
#include "gpuResManger.h"

PostEffect::~PostEffect()
{
	delete obj;
}

PostEffect::PostEffect(string name)
{
	Geometry *g = new Geometry;

	vector<MyVertex::Vertex> vList;
	MyVertex::Vertex v;
	v.Pos = XMFLOAT3(-1.0f, -1.0f, 0.001f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	vList.push_back(v);

	v.Pos = XMFLOAT3(1.0f, -1.0f, 0.001f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	vList.push_back(v);

	v.Pos = XMFLOAT3(1.0f, 1.0f, 0.001f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	vList.push_back(v);

	v.Pos = XMFLOAT3(-1.0f, 1.0f, 0.001f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	vList.push_back(v);

	vector<UINT> iList;
	iList.push_back(0);
	iList.push_back(3);
	iList.push_back(2);
	iList.push_back(2);
	iList.push_back(1);
	iList.push_back(0);
	g->setIndexData(iList);
	g->setVertexData(vList);

	Ptr<MaterialRes> res = ResManager::Instance().createMatRes(name);
	Material *m = new Material(res->getMaterialData("postEffect"));

	obj = new RenderAble(g, m);
}

void PostEffect::Render()
{
	gRender->draw(obj, 0);
}