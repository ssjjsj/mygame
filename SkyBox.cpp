#include "SkyBox.h"


void SkyBox::createSkyBox()
{
	MaterialRes res = MaterialRes("skyBox.material.xml");
	MyVertex::ModelData frontModelData;
	//1
	MyVertex::Vertex v;
	v.Pos = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	frontModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(1.0f, -1.0f, -1.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	frontModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	frontModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(1.0f, 1.0f, -1.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	frontModelData.vertexs.push_back(v);

	int i = 0;
	frontModelData.indexs.push_back(i * 4);
	frontModelData.indexs.push_back(i * 4 + 2);
	frontModelData.indexs.push_back(i * 4 + 3);
	frontModelData.indexs.push_back(i * 4 + 3);
	frontModelData.indexs.push_back(i * 4 + 1);
	frontModelData.indexs.push_back(i * 4 + 2);

	Geometry *g = new Geometry();
	Material *m = new Material(res.getMaterialData("front"));
	g->setVertexData(frontModelData.vertexs);
	g->setIndexData(frontModelData.indexs);
	RenderAble *renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);


	MyVertex::ModelData backModelData;
	//2
	v.Pos = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	backModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(1.0f, -1.0f, -1.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	backModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	backModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(1.0f, 1.0f, -1.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	backModelData.vertexs.push_back(v);

	i = 1;
	backModelData.indexs.push_back(i * 4);
	backModelData.indexs.push_back(i * 4 + 2);
	backModelData.indexs.push_back(i * 4 + 3);
	backModelData.indexs.push_back(i * 4 + 3);
	backModelData.indexs.push_back(i * 4 + 1);
	backModelData.indexs.push_back(i * 4 + 2);


	g = new Geometry();
	m = new Material(res.getMaterialData("back"));
	g->setVertexData(backModelData.vertexs);
	g->setIndexData(backModelData.indexs);
	renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);


	MyVertex::ModelData leftModelData;
	//3
	v.Pos = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	leftModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	leftModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	leftModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	leftModelData.vertexs.push_back(v);

	i = 2;
	leftModelData.indexs.push_back(i * 4);
	leftModelData.indexs.push_back(i * 4 + 2);
	leftModelData.indexs.push_back(i * 4 + 3);
	leftModelData.indexs.push_back(i * 4 + 3);
	leftModelData.indexs.push_back(i * 4 + 1);
	leftModelData.indexs.push_back(i * 4 + 2);

	g = new Geometry();
	m = new Material(res.getMaterialData("left"));
	g->setVertexData(leftModelData.vertexs);
	g->setIndexData(leftModelData.indexs);
	renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);


	//4
	MyVertex::ModelData rightModelData;
	v.Pos = XMFLOAT3(1.0f, -1.0f, -1.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	rightModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(1.0f, -1.0f, 1.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	rightModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(1.0f, 1.0f, -1.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	rightModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	rightModelData.vertexs.push_back(v);

	i = 3;
	rightModelData.indexs.push_back(i * 4);
	rightModelData.indexs.push_back(i * 4 + 2);
	rightModelData.indexs.push_back(i * 4 + 3);
	rightModelData.indexs.push_back(i * 4 + 3);
	rightModelData.indexs.push_back(i * 4 + 1);
	rightModelData.indexs.push_back(i * 4 + 2);

	g = new Geometry();
	m = new Material(res.getMaterialData("right"));
	g->setVertexData(rightModelData.vertexs);
	g->setIndexData(rightModelData.indexs);
	renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);

	//5
	MyVertex::ModelData topModelData;
	v.Pos = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	topModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(1.0f, 1.0f, -1.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	topModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	topModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(1.0f, 1.0f, 1.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	topModelData.vertexs.push_back(v);

	i = 4;
	topModelData.indexs.push_back(i * 4);
	topModelData.indexs.push_back(i * 4 + 2);
	topModelData.indexs.push_back(i * 4 + 3);
	topModelData.indexs.push_back(i * 4 + 3);
	topModelData.indexs.push_back(i * 4 + 1);
	topModelData.indexs.push_back(i * 4 + 2);

	g = new Geometry();
	m = new Material(res.getMaterialData("top"));
	g->setVertexData(topModelData.vertexs);
	g->setIndexData(topModelData.indexs);
	renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);

	//6
	MyVertex::ModelData buttomModelData;
	v.Pos = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	buttomModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(1.0f, -1.0f, -1.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	buttomModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	buttomModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(1.0f, -1.0f, 1.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	buttomModelData.vertexs.push_back(v);

	i = 5;
	buttomModelData.indexs.push_back(i * 4);
	buttomModelData.indexs.push_back(i * 4 + 2);
	buttomModelData.indexs.push_back(i * 4 + 3);
	buttomModelData.indexs.push_back(i * 4 + 3);
	buttomModelData.indexs.push_back(i * 4 + 1);
	buttomModelData.indexs.push_back(i * 4 + 2);

	g = new Geometry();
	m = new Material(res.getMaterialData("buttom"));
	g->setVertexData(buttomModelData.vertexs);
	g->setIndexData(buttomModelData.indexs);
	renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);
}