#include "SkyBox.h"


void SkyBox::createSkyBox()
{
	MaterialRes res = MaterialRes("skyBox.material.xml");
	MyVertex::ModelData frontModelData;
	//1
	MyVertex::Vertex v;
	v.Pos = XMFLOAT3(-50.0f, -50.0f, -50.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	frontModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(50.0f, -50.0f, -50.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	frontModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-50.0f, 50.0f, -50.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	frontModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(50.0f, 50.0f, -50.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	frontModelData.vertexs.push_back(v);

	frontModelData.indexs.push_back(0);
	frontModelData.indexs.push_back(1);
	frontModelData.indexs.push_back(3);
	frontModelData.indexs.push_back(3);
	frontModelData.indexs.push_back(2);
	frontModelData.indexs.push_back(0);

	Geometry *g = new Geometry();
	Material *m = new Material(res.getMaterialData("back"));
	g->setVertexData(frontModelData.vertexs);
	g->setIndexData(frontModelData.indexs);
	RenderAble *renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);


	MyVertex::ModelData backModelData;
	//2
	v.Pos = XMFLOAT3(-50.0f, -50.0f, 50.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	backModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(50.0f, -50.0f, 50.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	backModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-50.0f, 50.0f, 50.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	backModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(50.0f, 50.0f, 50.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	backModelData.vertexs.push_back(v);

	backModelData.indexs.push_back(0);
	backModelData.indexs.push_back(2);
	backModelData.indexs.push_back(3);
	backModelData.indexs.push_back(3);
	backModelData.indexs.push_back(1);
	backModelData.indexs.push_back(0);


	g = new Geometry();
	m = new Material(res.getMaterialData("front"));
	g->setVertexData(backModelData.vertexs);
	g->setIndexData(backModelData.indexs);
	renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);


	MyVertex::ModelData leftModelData;
	//3
	v.Pos = XMFLOAT3(-50.0f, -50.0f, -50.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	leftModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-50.0f, -50.0f, 50.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	leftModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-50.0f, 50.0f, -50.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	leftModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-50.0f, 50.0f, 50.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	leftModelData.vertexs.push_back(v);


	leftModelData.indexs.push_back(0);
	leftModelData.indexs.push_back(2);
	leftModelData.indexs.push_back(3);
	leftModelData.indexs.push_back(3);
	leftModelData.indexs.push_back(1);
	leftModelData.indexs.push_back(0);

	g = new Geometry();
	m = new Material(res.getMaterialData("left"));
	g->setVertexData(leftModelData.vertexs);
	g->setIndexData(leftModelData.indexs);
	renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);


	//4
	MyVertex::ModelData rightModelData;
	v.Pos = XMFLOAT3(50.0f, -50.0f, -50.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	rightModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(50.0f, -50.0f, 50.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	rightModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(50.0f, 50.0f, -50.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	rightModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(50.0f, 50.0f, 50.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	rightModelData.vertexs.push_back(v);


	rightModelData.indexs.push_back(0);
	rightModelData.indexs.push_back(1);
	rightModelData.indexs.push_back(3);
	rightModelData.indexs.push_back(3);
	rightModelData.indexs.push_back(2);
	rightModelData.indexs.push_back(0);

	g = new Geometry();
	m = new Material(res.getMaterialData("right"));
	g->setVertexData(rightModelData.vertexs);
	g->setIndexData(rightModelData.indexs);
	renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);

	//5
	MyVertex::ModelData topModelData;
	v.Pos = XMFLOAT3(-50.0f, 50.0f, -50.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	topModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(50.0f, 50.0f, -50.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	topModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-50.0f, 50.0f, 50.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	topModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(50.0f, 50.0f, 50.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	topModelData.vertexs.push_back(v);


	topModelData.indexs.push_back(0);
	topModelData.indexs.push_back(1);
	topModelData.indexs.push_back(3);
	topModelData.indexs.push_back(3);
	topModelData.indexs.push_back(2);
	topModelData.indexs.push_back(0);

	g = new Geometry();
	m = new Material(res.getMaterialData("top"));
	g->setVertexData(topModelData.vertexs);
	g->setIndexData(topModelData.indexs);
	renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);

	//6
	MyVertex::ModelData buttomModelData;
	v.Pos = XMFLOAT3(-50.0f, -50.0f, -50.0f);
	v.UV = XMFLOAT2(1.0f, 0.0f);
	buttomModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(50.0f, -50.0f, -50.0f);
	v.UV = XMFLOAT2(1.0f, 1.0f);
	buttomModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(-50.0f, -50.0f, 50.0f);
	v.UV = XMFLOAT2(0.0f, 0.0f);
	buttomModelData.vertexs.push_back(v);

	v.Pos = XMFLOAT3(50.0f, -50.0f, 50.0f);
	v.UV = XMFLOAT2(0.0f, 1.0f);
	buttomModelData.vertexs.push_back(v);


	buttomModelData.indexs.push_back(0);
	buttomModelData.indexs.push_back(2);
	buttomModelData.indexs.push_back(3);
	buttomModelData.indexs.push_back(3);
	buttomModelData.indexs.push_back(1);
	buttomModelData.indexs.push_back(0);

	g = new Geometry();
	m = new Material(res.getMaterialData("buttom"));
	g->setVertexData(buttomModelData.vertexs);
	g->setIndexData(buttomModelData.indexs);
	renderAble = new RenderAble(g, m);
	renderAbles.push_back(renderAble);
}